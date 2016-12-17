using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace LedSyncService
{
	public class LedSyncSvc
	{
		private bool ledInitialized;

		private bool keepWorking;

		private bool programsCreated;

		private string ledIP = "";

		private int hProgram = 0;

		private LedDll.COMMUNICATIONINFO communicationInfo;

		private int intervalForUpdatingLed = 2000;		// 2 seconds

		public void Shutdown()
		{
			Stop();
		}

		public bool Stop()
		{
			keepWorking = false;
			Cleanup();

			return true;
		}

		public bool Start()
		{
			Task.Run(() =>
				{
					DoWork();
				});

			return true;
		}

		private void DoWork()
		{
			keepWorking = true;
			while (keepWorking)		// It will be set to false outside
			{
				try
				{
					if (!InitLed())
						continue;

					if (!CreatePrograms())
						continue;

					UpdateLedInfo();
				}
				catch (Exception ex)
				{
					// log error
				}
				finally
				{
					Thread.Sleep(intervalForUpdatingLed);
				}
			}
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
			}
			catch (Exception ex)
			{
				// log error
			}
		}

		private bool InitLed()
		{
			if (ledInitialized)
				return ledInitialized;

			var connection = DbFactory.GetConnection();
			var dal = new Dal(connection);
			var parameters = dal.GetGeneralParameters();
			var intervalParam = parameters.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase)
															&& string.Equals(p.Name, "IntervalForUpdatingLed", StringComparison.OrdinalIgnoreCase));
			// Update the retry interval
			if (intervalParam != null)
			{
				intervalForUpdatingLed = int.Parse(intervalParam.Value);
			}

			var param = parameters.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase)
													&& string.Equals(p.Name, "LedIP", StringComparison.OrdinalIgnoreCase));
			if (parameters == null)
			{
				//log error
				return false;
			}

			IPAddress ip;
			if (IPAddress.TryParse(param.Value, out ip))
			{
				// log error
				return false;
			}

			ledIP = param.Value;
			// log to connect to led with IP

			communicationInfo = new LedDll.COMMUNICATIONINFO();//定义一通讯参数结构体变量用于对设定的LED通讯，具体对此结构体元素赋值说明见COMMUNICATIONINFO结构体定义部份注示
															   //ZeroMemory(&CommunicationInfo,sizeof(COMMUNICATIONINFO));
			//TCP通讯********************************************************************************
			communicationInfo.SendType = 0;                                             //设为固定IP通讯模式，即TCP通讯
			communicationInfo.IpStr = ledIP;                                            //给IpStr赋值LED控制卡的IP
			communicationInfo.LedNumber = 1;											//LED屏号为1，注意socket通讯和232通讯不识别屏号，默认赋1就行了，485必需根据屏的实际屏号进行赋值

			var result = LedDll.LV_SetBasicInfo(ref communicationInfo, 2, 64, 32);		//设置屏参，屏的颜色为2即为双基色，64为屏宽点数，32为屏高点数，具体函数参数说明见函数声明注示
			if (result != 0)															//如果失败则可以调用LV_GetError获取中文错误信息
			{
				var errMsg = LedDll.LS_GetError(result);
				// log error
				return false;
			}
						
			ledInitialized = true;

			// log init ok
			
			return ledInitialized;
		}

		private bool CreatePrograms()
		{
			if (programsCreated)
				return true;

			if (hProgram == 0)
			{
				hProgram = LedDll.LV_CreateProgram(64, 32, 2);//根据传的参数创建节目句柄，64是屏宽点数，32是屏高点数，2是屏的颜色，注意此处屏宽高及颜色参数必需与设置屏参的屏宽高及颜色一致，否则发送时会提示错误
															  //此处可自行判断有未创建成功，hProgram返回NULL失败，非NULL成功,一般不会失败
				// todo
				if (hProgram == 0)
				{
					// log error
					return false;
				}
			}

			var result = LedDll.LV_AddProgram(hProgram, 1, 0, 1);	//添加一个节目，显示厂名
			if (result != 0)
			{
				var errStr = LedDll.LS_GetError(result);
				// todo log error
				return false;
			}
			LedDll.AREARECT AreaRect = new LedDll.AREARECT();	//区域坐标属性结构体变量
			AreaRect.left = 0;
			AreaRect.top = 0;
			AreaRect.width = 64;	// todo, real width/height?
			AreaRect.height = 32;

			LedDll.LV_AddImageTextArea(hProgram, 1, 1, ref AreaRect, 0);

			var fontProp = new LedDll.FONTPROP();//文字属性
			fontProp.FontName = "黑体";		// 宋体
			fontProp.FontSize = 16;
			fontProp.FontColor = LedDll.COLOR_RED;
			fontProp.FontBold = 0;

			var playProp = new LedDll.PLAYPROP();
			playProp.InStyle = 0;
			playProp.DelayTime = 3;
			playProp.Speed = 4;

			result = LedDll.LV_AddMultiLineTextToImageTextArea(hProgram, 1, 1, LedDll.ADDTYPE_STRING, "黎明沼气\r\n发电厂", ref fontProp, ref playProp, 0, 0);//通过字符串添加一个多行文本到图文区
			result = LedDll.LV_Send(ref communicationInfo, hProgram);//发送
			if (result != 0)//如果失败则可以调用LV_GetError获取中文错误信息
			{
				var errStr = LedDll.LS_GetError(result);
				// todo log erro
				return false;
			}

			// todo log success

			programsCreated = true;
			return programsCreated;
		}

		private void UpdateLedInfo()
		{
			//
		}
	}
}
