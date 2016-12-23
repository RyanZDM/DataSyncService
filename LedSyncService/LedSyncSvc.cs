using System;
using System.Linq;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using NLog;
using System.Data.SqlClient;

namespace LedSyncService
{
	public class LedSyncSvc
	{
		private readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

		const int LedWidth = 160;
		const int LedHeight = 80;
		/// <summary>
		/// The height of title for showing factory name
		/// </summary>
		const int TitleHeight = 32;

		private string factoryName = "上海浦东环保发展有限公司\r\n黎明沼气发电厂";

		private string infoTemplate = "总发电量:        {0,10} KW\r\n总沼气消耗量:      {1,10} M³\r\n当前工班发电量:    {2,10} KW\r\n当前工班沼气消耗量:\t{3,10} M³";

		private SqlConnection connection;

		private bool ledInitialized;

		private bool keepWorking;

		private bool programsCreated;

		private string ledIP = "";

		private int hProgram = 0;

		private LedDll.COMMUNICATIONINFO communicationInfo;

		private int intervalForUpdatingLed = 2000;      // 2 seconds		

		public void Shutdown()
		{
			logger.Info("Shutting down computer...");
			Stop();
		}

		public bool Stop()
		{
			keepWorking = false;
			Cleanup();

			logger.Info("The LedSyncService stopped.");
			return true;
		}

		public bool Start()
		{
			logger.Info("THe LedSyncService starting...");

			Task.Run(() =>
				{
					logger.Info("THe LedSyncService started.");
					DoWork();
				});

			return true;
		}

		private void DoWork()
		{
			keepWorking = true;
			while (keepWorking)     // It will be set to false outside
			{
				try
				{
					if (connection == null)
					{
						connection = DbFactory.GetConnection();
						logger.Info("Database connected.");
					}

					if (!InitLed())
						continue;

					if (!CreatePrograms())
						continue;

					if (!UpdateTitle())
						continue;

					UpdateLedInfo();
				}
				catch (Exception ex)
				{
					logger.Error(ex, "Error occurred in DoWork.");
				}
				finally
				{
					Thread.Sleep(intervalForUpdatingLed);
				}
			}

			logger.Info("Exit DoWork.");
		}

		private void Cleanup()
		{
			try
			{
				ledInitialized = false;
				if (hProgram != 0)
				{
					LedDll.LV_DeleteProgram(hProgram);
					hProgram = 0;
				}

				if (connection != null)
				{
					try { connection.Close(); } catch (Exception ex) { logger.Error(ex, "Error occurred while closing DB connection."); }
					try { connection.Dispose(); } catch (Exception ex) { logger.Error(ex, "Error occurred whle disposing connection object."); }

					connection = null;
				}
			}
			catch (Exception ex)
			{
				logger.Error(ex, "Error occurred in Cleanup method.");
			}
		}

		private bool InitLed()
		{
			if (ledInitialized)
				return ledInitialized;

			var dal = new Dal(connection);
			var parameters = dal.GetGeneralParameters();
			var intervalParam = parameters.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase)
															&& string.Equals(p.Name, "IntervalForUpdatingLed", StringComparison.OrdinalIgnoreCase));
			// Update the retry interval
			if (intervalParam != null)
			{
				intervalForUpdatingLed = int.Parse(intervalParam.Value);
				logger.Info("Found the setting of interval for updating LED in database. {0}", intervalForUpdatingLed);
			}

			var titleParam = parameters.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase)
														&& string.Equals(p.Name, "LedTitle", StringComparison.OrdinalIgnoreCase));
			if (titleParam != null)
			{
				factoryName = titleParam.Value;
				logger.Info("Found the setting of LedTitle in database. {0}", factoryName);
			}

			var infoParam = parameters.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase)
														&& string.Equals(p.Name, "LedInfoTemplate", StringComparison.OrdinalIgnoreCase));
			if (infoParam != null)
			{
				infoTemplate = infoParam.Value;
				logger.Info("Found the setting of LED info template in database. {0}", infoTemplate);
			}

			var param = parameters.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase)
													&& string.Equals(p.Name, "LedIP", StringComparison.OrdinalIgnoreCase));
			if (param == null)
			{
				logger.Error("Cannot find the settings of LED IP from database.");
				return false;
			}

			IPAddress ip;
			if (!IPAddress.TryParse(param.Value, out ip))
			{
				logger.Error("The IP of LED is invalid. {0}", param.Value);
				return false;
			}

			ledIP = param.Value;
			logger.Info("The IP of LED is: {0}.", param.Value);

			//定义一通讯参数结构体变量用于对设定的LED通讯，具体对此结构体元素赋值说明见COMMUNICATIONINFO结构体定义部份注示
			communicationInfo = new LedDll.COMMUNICATIONINFO();
			//ZeroMemory(&CommunicationInfo,sizeof(COMMUNICATIONINFO));
			//TCP通讯********************************************************************************
			communicationInfo.SendType = 0;		//设为固定IP通讯模式，即TCP通讯
			communicationInfo.IpStr = ledIP;	//给IpStr赋值LED控制卡的IP
			communicationInfo.LedNumber = 1;	//LED屏号为1，注意socket通讯和232通讯不识别屏号，默认赋1就行了，485必需根据屏的实际屏号进行赋值

			var result = LedDll.LV_SetBasicInfo(ref communicationInfo, 2, 64, 32);      //设置屏参，屏的颜色为2即为双基色，64为屏宽点数，32为屏高点数，具体函数参数说明见函数声明注示
			if (result != 0)                                                            //如果失败则可以调用LV_GetError获取中文错误信息
			{
				var errMsg = LedDll.LS_GetError(result);
				logger.Error("Failed to initialize LED, call LV_SetBasiceInfo failed. SendType=0,IpStr={0},LedNumber=1. {1}", ledIP, errMsg);
				return false;
			}

			ledInitialized = true;

			logger.Info("LED was initialized successfully.");

			return ledInitialized;
		}

		private bool CreatePrograms()
		{
			if (programsCreated)
				return true;

			if (hProgram == 0)
			{
				//根据传的参数创建节目句柄，屏宽点数，屏高点数，2是屏的颜色，注意此处屏宽高及颜色参数必需与设置屏参的屏宽高及颜色一致，否则发送时会提示错误
				hProgram = LedDll.LV_CreateProgram(LedWidth, LedHeight, 2);
				if (hProgram == 0)
				{
					logger.Error("Failed to create program handle, parameters: width:{0},height:{1},color:2");
					return false;
				}
			}

			programsCreated = true;
			logger.Debug("Program handle [{0}] created.", hProgram);

			return programsCreated;
		}

		private bool UpdateTitle()
		{
			var result = LedDll.LV_AddProgram(hProgram, 1, 0, 1);   //添加一个节目，显示厂名
			if (result != 0)
			{
				var errMsg = LedDll.LS_GetError(result);
				logger.Error("Failed to add a program. {0}", errMsg);
				return false;
			}

			logger.Debug("Added first program.");

			var areaRect = new LedDll.AREARECT();   //区域坐标属性结构体变量
			areaRect.left = 0;
			areaRect.top = 0;
			areaRect.width = LedWidth;
			areaRect.height = TitleHeight;

			result = LedDll.LV_AddImageTextArea(hProgram, 1, 1, ref areaRect, 0);
			if (result != 0)
			{
				var errMsg = LedDll.LS_GetError(result);
				logger.Error("Failed to add ImageTextArea to program 1. {0}", errMsg);
				return false;
			}

			logger.Debug("ImageTextArea #1 was added to program 1. L/T/W/H:{0}/{1]/{2}/{3}.", 
							areaRect.left, areaRect.top, areaRect.width, areaRect.height);

			var fontProp = new LedDll.FONTPROP();//文字属性
			fontProp.FontName = "黑体";       // 宋体
			fontProp.FontSize = 16;
			fontProp.FontColor = LedDll.COLOR_RED;
			fontProp.FontBold = 0;

			var playProp = new LedDll.PLAYPROP();
			playProp.InStyle = 0;
			playProp.DelayTime = 3;
			playProp.Speed = 4;

			result = LedDll.LV_AddMultiLineTextToImageTextArea(hProgram, 1, 1, LedDll.ADDTYPE_STRING, factoryName, ref fontProp, ref playProp, 0, 0);//通过字符串添加一个多行文本到图文区
			result = LedDll.LV_Send(ref communicationInfo, hProgram);
			if (result != 0)
			{
				var errMsg = LedDll.LS_GetError(result);
				logger.Error("Failed to add mutliple line text to ImageTextArea 1. {0}", errMsg);
				return false;
			}

			logger.Info("ImageTextArea created for showing title.");

			return true;
		}

		private bool UpdateLedInfo()
		{
			var areaRect = new LedDll.AREARECT();   //区域坐标属性结构体变量
			areaRect.left = 0;
			areaRect.top = TitleHeight;
			areaRect.width = LedWidth;
			areaRect.height = LedHeight - TitleHeight;

			LedDll.LV_AddImageTextArea(hProgram, 1, 2, ref areaRect, 0);
			logger.Debug("ImageTextArea #2 was added to program 1. L/T/W/H:{0}/{1]/{2}/{3}.",
							areaRect.left, areaRect.top, areaRect.width, areaRect.height);

			var fontProp = new LedDll.FONTPROP();//文字属性
			fontProp.FontName = "宋体";       // 宋体
			fontProp.FontSize = 16;
			fontProp.FontColor = LedDll.COLOR_RED;
			fontProp.FontBold = 0;

			var playProp = new LedDll.PLAYPROP();
			playProp.InStyle = 0;
			playProp.DelayTime = 3;
			playProp.Speed = 4;

			// Get real data from database
			var dal = new Dal(connection);
			var statData = dal.GetStatData();
			if (statData == null)
			{
				logger.Error("Cannot get stat data.");
				return false;
			}

			var info = string.Format(infoTemplate
									, statData.TotalEnergyGenerated
									, statData.TotalBiogasUsed
									, statData.CurrentEnergyGenerated
									, statData.CurrentBiogasUsed);

			logger.Debug("Will add the message [{0}] to Area #2.", info);

			var result = LedDll.LV_AddMultiLineTextToImageTextArea(hProgram, 1, 2, LedDll.ADDTYPE_STRING, info, ref fontProp, ref playProp, 0, 0);//通过字符串添加一个多行文本到图文区
			result = LedDll.LV_Send(ref communicationInfo, hProgram);
			if (result != 0)
			{
				var errMsg = LedDll.LS_GetError(result);
				logger.Error("Failed to add multiple line text to ImageTextArea 2. Text=[{0}], Error: {1}.", info, errMsg);
				return false;
			}

			logger.Debug("Successfully added the mesasge to ImageTextArea 2.");
			return true;
		}
	}
}
