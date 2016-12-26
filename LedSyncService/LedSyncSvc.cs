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
		private readonly Logger logger = LogManager.GetCurrentClassLogger();

		const int LedWidth = 160;
		const int LedHeight = 80;
		/// <summary>
		/// The height of title for showing factory name
		/// </summary>
		const int TitleHeight = 24;

		private string factoryName = "      浦发环保      黎明沼气发电厂";

		private string infoTemplate = "总发电:   {0,9} KWh\r\n总沼气消耗:  {1,7} M³\r\n工班发电: {2,9} KWh\r\n工班沼气消耗:{3,7} M³";

		private SqlConnection connection;

		private bool ledInitialized;

		private bool keepWorking;
		
		private string ledIP = "";

		private LedDll.COMMUNICATIONINFO communicationInfo;

		private int intervalForUpdatingLed = 30000;      // 30 seconds		

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
			var errorOccurred = false;

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

					UpdateLed();

					errorOccurred = false;
				}
				catch (Exception ex)
				{
					errorOccurred = true;
					logger.Error(ex, "Error occurred in DoWork.");
				}
				finally
				{
					Thread.Sleep(errorOccurred ? 10 : intervalForUpdatingLed);
				}
			}

			logger.Info("Exit DoWork.");
		}

		private void Cleanup()
		{
			try
			{
				ledInitialized = false;

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
			communicationInfo.SendType = 0;     //设为固定IP通讯模式，即TCP通讯
			communicationInfo.IpStr = ledIP;    //给IpStr赋值LED控制卡的IP
			communicationInfo.LedNumber = 1;    //LED屏号为1，注意socket通讯和232通讯不识别屏号，默认赋1就行了，485必需根据屏的实际屏号进行赋值

			var result = LedDll.LV_SetBasicInfo(ref communicationInfo, 2, LedWidth, LedHeight);      //设置屏参，屏的颜色为2即为双基色，64为屏宽点数，32为屏高点数，具体函数参数说明见函数声明注示
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

		private int CreateProgram()
		{
			//根据传的参数创建节目句柄，屏宽点数，屏高点数，2是屏的颜色，注意此处屏宽高及颜色参数必需与设置屏参的屏宽高及颜色一致，否则发送时会提示错误
			var hProgram = LedDll.LV_CreateProgram(LedWidth, LedHeight, 2);
			if (hProgram > 0)
			{
				logger.Debug("Program handle [{0}] created.", hProgram);
			}
			else
			{
				logger.Error("Failed to create program handle, parameters: width:{0},height:{1},color:2");
			}

			return hProgram;
		}

		private bool UpdateLed()
		{
			var hProgram = CreateProgram();
			try
			{
				#region Add program
				var result = LedDll.LV_AddProgram(hProgram, 1, 0, 1);
				if (result != 0)
				{
					var errMsg = LedDll.LS_GetError(result);
					logger.Error("Failed to add a program. {0}", errMsg);
					return false;
				}
				logger.Debug("Added first program.");
				#endregion
				
				#region		1. Show Title
				var areaRect1 = new LedDll.AREARECT();   //区域坐标属性结构体变量
				areaRect1.left = 0;
				areaRect1.top = 0;
				areaRect1.width = LedWidth;
				areaRect1.height = TitleHeight;

				LedDll.LV_AddImageTextArea(hProgram, 1, 1, ref areaRect1, 0);
				logger.Debug("ImageTextArea #1 was added to program 1. L/T/W/H:{0}/{1}/{2}/{3}.",
								areaRect1.left, areaRect1.top, areaRect1.width, areaRect1.height);

				var fontProp1 = new LedDll.FONTPROP();//文字属性
				fontProp1.FontName = "黑体";       // 黑体
				fontProp1.FontSize = 14;
				fontProp1.FontColor = LedDll.COLOR_RED;
				fontProp1.FontBold = 0;

				var playProp1 = new LedDll.PLAYPROP();
				playProp1.InStyle = 0;
				playProp1.DelayTime = 15;
				playProp1.Speed = 4;

				//LedDll.LV_QuickAddSingleLineTextArea(hProgram, 1, 1, ref areaRect1, LedDll.ADDTYPE_STRING, factoryName, ref fontProp1, 8);
				LedDll.LV_AddMultiLineTextToImageTextArea(hProgram, 1, 1, LedDll.ADDTYPE_STRING, factoryName, ref fontProp1, ref playProp1, 2, 1);
				//LedDll.LV_AddStaticTextToImageTextArea(hProgram, 1, 1, LedDll.ADDTYPE_STRING, factoryName, ref fontProp1, 2, 2, 1);
				#endregion       //1. Show Title

				#region 2. Show stat data
				var areaRect2 = new LedDll.AREARECT();
				areaRect2.left = 0;
				areaRect2.top = TitleHeight;
				areaRect2.width = LedWidth;
				areaRect2.height = LedHeight - TitleHeight;

				LedDll.LV_AddImageTextArea(hProgram, 1, 2, ref areaRect2, 0);
				logger.Debug("ImageTextArea #2 was added to program 1. L/T/W/H:{0}/{1}/{2}/{3}.",
								areaRect2.left, areaRect2.top, areaRect2.width, areaRect2.height);

				var fontProp2 = new LedDll.FONTPROP();//文字属性
				fontProp2.FontName = "宋体";       // 宋体
				fontProp2.FontSize = 10;
				fontProp2.FontColor = LedDll.COLOR_GREEN;
				fontProp2.FontBold = 0;

				var playProp2 = new LedDll.PLAYPROP();
				playProp2.InStyle = 0;
				playProp2.DelayTime = 3;
				playProp2.Speed = 4;

				// Get real data from database
				var dal = new Dal(connection);
				var statData = dal.GetStatData();
				if (statData == null)
				{
					logger.Error("Cannot get stat data.");
					return false;
				}

				var info = string.Format(infoTemplate
										, (int)statData.TotalEnergyGenerated
										, (int)statData.TotalBiogasUsed
										, (int)statData.CurrentEnergyGenerated
										, (int)statData.CurrentBiogasUsed);

				logger.Debug("Will add the message [{0}] to Area #2.", info);

				LedDll.LV_AddMultiLineTextToImageTextArea(hProgram, 1, 2, LedDll.ADDTYPE_STRING, info, ref fontProp2, ref playProp2, 0, 0);//通过字符串添加一个多行文本到图文区
				#endregion      // 2. Show stat data

				result = LedDll.LV_Send(ref communicationInfo, hProgram);
				if (result != 0)
				{
					var errMsg = LedDll.LS_GetError(result);
					logger.Error("Failed to add mutliple line text to ImageTextArea 1. {0}", errMsg);
					return false;
				}
				
				return true;
			}
			finally
			{
				if (hProgram != 0)
					LedDll.LV_DeleteProgram(hProgram);
			}
		}
	}
}
