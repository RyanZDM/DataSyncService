using System;
using System.Windows.Forms;

namespace Demo
{
	public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int nResult;
            LedDll.COMMUNICATIONINFO CommunicationInfo = new LedDll.COMMUNICATIONINFO();//定义一通讯参数结构体变量用于对设定的LED通讯，具体对此结构体元素赋值说明见COMMUNICATIONINFO结构体定义部份注示
            //ZeroMemory(&CommunicationInfo,sizeof(COMMUNICATIONINFO));
            //TCP通讯********************************************************************************
            CommunicationInfo.SendType = 0;//设为固定IP通讯模式，即TCP通讯
            CommunicationInfo.IpStr = "192.168.1.245";//给IpStr赋值LED控制卡的IP
            CommunicationInfo.LedNumber = 1;//LED屏号为1，注意socket通讯和232通讯不识别屏号，默认赋1就行了，485必需根据屏的实际屏号进行赋值
            //广播通讯********************************************************************************
            //CommunicationInfo.SendType=1;//设为单机直连，即广播通讯无需设LED控制器的IP地址
            //串口通讯********************************************************************************
            //CommunicationInfo.SendType=2;//串口通讯
            //CommunicationInfo.Commport=1;//串口的编号，如设备管理器里显示为 COM3 则此处赋值 3
            //CommunicationInfo.Baud=9600;//波特率
            //CommunicationInfo.LedNumber=1;


            nResult = LedDll.LV_SetBasicInfo(ref CommunicationInfo, 2, 64, 32);//设置屏参，屏的颜色为2即为双基色，64为屏宽点数，32为屏高点数，具体函数参数说明见函数声明注示
            if (nResult!=0)//如果失败则可以调用LV_GetError获取中文错误信息
            {
                string ErrStr;
                ErrStr=LedDll.LS_GetError(nResult);
                MessageBox.Show(ErrStr);
            }
            else
            {
                MessageBox.Show("设置成功");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            int nResult;
            LedDll.COMMUNICATIONINFO CommunicationInfo = new LedDll.COMMUNICATIONINFO();//定义一通讯参数结构体变量用于对设定的LED通讯，具体对此结构体元素赋值说明见COMMUNICATIONINFO结构体定义部份注示
            //ZeroMemory(&CommunicationInfo,sizeof(COMMUNICATIONINFO));
            //TCP通讯********************************************************************************
            CommunicationInfo.SendType = 0;//设为固定IP通讯模式，即TCP通讯
            CommunicationInfo.IpStr = "192.168.1.245";//给IpStr赋值LED控制卡的IP
            CommunicationInfo.LedNumber = 1;//LED屏号为1，注意socket通讯和232通讯不识别屏号，默认赋1就行了，485必需根据屏的实际屏号进行赋值
            //广播通讯********************************************************************************
            //CommunicationInfo.SendType=1;//设为单机直连，即广播通讯无需设LED控制器的IP地址
            //串口通讯********************************************************************************
            //CommunicationInfo.SendType=2;//串口通讯
            //CommunicationInfo.Commport=1;//串口的编号，如设备管理器里显示为 COM3 则此处赋值 3
            //CommunicationInfo.Baud=9600;//波特率
            //CommunicationInfo.LedNumber=1;

            int hProgram;//节目句柄
            hProgram = LedDll.LV_CreateProgram(64, 32, 2);//根据传的参数创建节目句柄，64是屏宽点数，32是屏高点数，2是屏的颜色，注意此处屏宽高及颜色参数必需与设置屏参的屏宽高及颜色一致，否则发送时会提示错误
            //此处可自行判断有未创建成功，hProgram返回NULL失败，非NULL成功,一般不会失败

            nResult = LedDll.LV_AddProgram(hProgram, 1, 0, 1);//添加一个节目，参数说明见函数声明注示
            if (nResult!=0)
            {
                string ErrStr;
                ErrStr = LedDll.LS_GetError(nResult);
                MessageBox.Show(ErrStr);
                return;
            }
            LedDll.AREARECT AreaRect=new LedDll.AREARECT();//区域坐标属性结构体变量
            AreaRect.left = 0;
            AreaRect.top = 0;
            AreaRect.width = 64;
            AreaRect.height = 32;

            LedDll.FONTPROP FontProp=new LedDll.FONTPROP();//文字属性
            FontProp.FontName="宋体";
            FontProp.FontSize = 12;
            FontProp.FontColor = LedDll.COLOR_GREEN;
            FontProp.FontBold = 0;
            //int nsize = System.Runtime.InteropServices.Marshal.SizeOf(typeof(LedDll.FONTPROP));

            nResult = LedDll.LV_QuickAddSingleLineTextArea(hProgram, 1, 1, ref AreaRect, LedDll.ADDTYPE_STRING, "上海灵信视觉技术股份有限公司", ref FontProp, 4);//快速通过字符添加一个单行文本区域，函数见函数声明注示
           // nResult = LedDll.LV_QuickAddSingleLineTextArea(hProgram, 1, 1, ref AreaRect, LedDll.ADDTYPE_FILE, "test.rtf", ref FontProp, 4);//快速通过rtf文件添加一个单行文本区域，函数见函数声明注示
            //nResult = LedDll.LV_QuickAddSingleLineTextArea(hProgram, 1, 1, ref AreaRect, LedDll.ADDTYPE_FILE, "test.txt", ref FontProp, 4);//快速通过txt文件添加一个单行文本区域，函数见函数声明注示


            nResult = LedDll.LV_Send(ref CommunicationInfo, hProgram);//发送，见函数声明注示
            LedDll.LV_DeleteProgram(hProgram);//删除节目内存对象，详见函数声明注示
            if (nResult != 0)//如果失败则可以调用LV_GetError获取中文错误信息
            {
                string ErrStr;
                ErrStr = LedDll.LS_GetError(nResult);
                MessageBox.Show(ErrStr);
            }
            else
            {
                MessageBox.Show("发送成功");
            }

        }

        private void button3_Click(object sender, EventArgs e)
        {
            int nResult;
            LedDll.COMMUNICATIONINFO CommunicationInfo = new LedDll.COMMUNICATIONINFO();//定义一通讯参数结构体变量用于对设定的LED通讯，具体对此结构体元素赋值说明见COMMUNICATIONINFO结构体定义部份注示
            //ZeroMemory(&CommunicationInfo,sizeof(COMMUNICATIONINFO));
            //TCP通讯********************************************************************************
            CommunicationInfo.SendType = 0;//设为固定IP通讯模式，即TCP通讯
            CommunicationInfo.IpStr = "192.168.1.245";//给IpStr赋值LED控制卡的IP
            CommunicationInfo.LedNumber = 1;//LED屏号为1，注意socket通讯和232通讯不识别屏号，默认赋1就行了，485必需根据屏的实际屏号进行赋值
            //广播通讯********************************************************************************
            //CommunicationInfo.SendType=1;//设为单机直连，即广播通讯无需设LED控制器的IP地址
            //串口通讯********************************************************************************
            //CommunicationInfo.SendType=2;//串口通讯
            //CommunicationInfo.Commport=1;//串口的编号，如设备管理器里显示为 COM3 则此处赋值 3
            //CommunicationInfo.Baud=9600;//波特率
            //CommunicationInfo.LedNumber=1;

            int hProgram;//节目句柄
            hProgram = LedDll.LV_CreateProgram(64, 32, 2);//根据传的参数创建节目句柄，64是屏宽点数，32是屏高点数，2是屏的颜色，注意此处屏宽高及颜色参数必需与设置屏参的屏宽高及颜色一致，否则发送时会提示错误
            //此处可自行判断有未创建成功，hProgram返回NULL失败，非NULL成功,一般不会失败

            nResult = LedDll.LV_AddProgram(hProgram, 1, 0, 1);//添加一个节目，参数说明见函数声明注示
            if (nResult != 0)
            {
                string ErrStr;
                ErrStr = LedDll.LS_GetError(nResult);
                MessageBox.Show(ErrStr);
                return;
            }
            LedDll.AREARECT AreaRect = new LedDll.AREARECT();//区域坐标属性结构体变量
            AreaRect.left = 0;
            AreaRect.top = 0;
            AreaRect.width = 64;
            AreaRect.height = 32;

            LedDll.LV_AddImageTextArea(hProgram,1,1,ref AreaRect,0);

            LedDll.FONTPROP FontProp = new LedDll.FONTPROP();//文字属性
            FontProp.FontName = "宋体";
            FontProp.FontSize = 12;
            FontProp.FontColor = LedDll.COLOR_RED;
            FontProp.FontBold = 0;

            LedDll.PLAYPROP PlayProp = new LedDll.PLAYPROP();
            PlayProp.InStyle = 0;
            PlayProp.DelayTime = 3;
            PlayProp.Speed = 4;
            //可以添加多个子项到图文区，如下添加可以选一个或多个添加
            nResult = LedDll.LV_AddMultiLineTextToImageTextArea(hProgram, 1, 1, LedDll.ADDTYPE_STRING, "上海灵信~~~~~", ref FontProp, ref PlayProp, 0, 0);//通过字符串添加一个多行文本到图文区，参数说明见声明注示
            nResult = LedDll.LV_AddMultiLineTextToImageTextArea(hProgram, 1, 1, LedDll.ADDTYPE_FILE, "test.rtf", ref FontProp, ref PlayProp, 0, 0);//通过rtf文件添加一个多行文本到图文区，参数说明见声明注示
            nResult = LedDll.LV_AddMultiLineTextToImageTextArea(hProgram, 1, 1, LedDll.ADDTYPE_FILE, "test.txt", ref FontProp, ref PlayProp, 0, 0);//通过txt文件添加一个多行文本到图文区，参数说明见声明注示


            nResult = LedDll.LV_Send(ref CommunicationInfo, hProgram);//发送，见函数声明注示
            LedDll.LV_DeleteProgram(hProgram);//删除节目内存对象，详见函数声明注示
            if (nResult != 0)//如果失败则可以调用LV_GetError获取中文错误信息
            {
                string ErrStr;
                ErrStr = LedDll.LS_GetError(nResult);
                MessageBox.Show(ErrStr);
            }
            else
            {
                MessageBox.Show("发送成功");
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            int nResult;
            LedDll.COMMUNICATIONINFO CommunicationInfo = new LedDll.COMMUNICATIONINFO();//定义一通讯参数结构体变量用于对设定的LED通讯，具体对此结构体元素赋值说明见COMMUNICATIONINFO结构体定义部份注示
            //ZeroMemory(&CommunicationInfo,sizeof(COMMUNICATIONINFO));
            //TCP通讯********************************************************************************
            CommunicationInfo.SendType = 0;//设为固定IP通讯模式，即TCP通讯
            CommunicationInfo.IpStr = "192.168.1.245";//给IpStr赋值LED控制卡的IP
            CommunicationInfo.LedNumber = 1;//LED屏号为1，注意socket通讯和232通讯不识别屏号，默认赋1就行了，485必需根据屏的实际屏号进行赋值
            //广播通讯********************************************************************************
            //CommunicationInfo.SendType=1;//设为单机直连，即广播通讯无需设LED控制器的IP地址
            //串口通讯********************************************************************************
            //CommunicationInfo.SendType=2;//串口通讯
            //CommunicationInfo.Commport=1;//串口的编号，如设备管理器里显示为 COM3 则此处赋值 3
            //CommunicationInfo.Baud=9600;//波特率
            //CommunicationInfo.LedNumber=1;

            int hProgram;//节目句柄
            hProgram = LedDll.LV_CreateProgram(64, 32, 2);//根据传的参数创建节目句柄，64是屏宽点数，32是屏高点数，2是屏的颜色，注意此处屏宽高及颜色参数必需与设置屏参的屏宽高及颜色一致，否则发送时会提示错误
            //此处可自行判断有未创建成功，hProgram返回NULL失败，非NULL成功,一般不会失败

            nResult = LedDll.LV_AddProgram(hProgram, 1, 0, 1);//添加一个节目，参数说明见函数声明注示
            if (nResult != 0)
            {
                string ErrStr;
                ErrStr = LedDll.LS_GetError(nResult);
                MessageBox.Show(ErrStr);
                return;
            }
            LedDll.AREARECT AreaRect = new LedDll.AREARECT();//区域坐标属性结构体变量
            AreaRect.left = 0;
            AreaRect.top = 0;
            AreaRect.width = 64;
            AreaRect.height = 32;

            LedDll.LV_AddImageTextArea(hProgram, 1, 1, ref AreaRect, 0);


            LedDll.PLAYPROP PlayProp = new LedDll.PLAYPROP();
            PlayProp.InStyle = 0;
            PlayProp.DelayTime = 3;
            PlayProp.Speed = 4;
            //可以添加多个子项到图文区，如下添加可以选一个或多个添加
            //可以添加多个子项到图文区，如下添加可以选一个或多个添加
            nResult = LedDll.LV_AddFileToImageTextArea(hProgram, 1, 1, "test.bmp", ref PlayProp);
            nResult = LedDll.LV_AddFileToImageTextArea(hProgram, 1, 1, "test.jpg", ref PlayProp);
            nResult = LedDll.LV_AddFileToImageTextArea(hProgram, 1, 1, "test.png", ref PlayProp);
            PlayProp.Speed = 3;
            nResult = LedDll.LV_AddFileToImageTextArea(hProgram, 1, 1, "test.gif", ref PlayProp);

            nResult = LedDll.LV_Send(ref CommunicationInfo, hProgram);//发送，见函数声明注示
            LedDll.LV_DeleteProgram(hProgram);//删除节目内存对象，详见函数声明注示
            if (nResult != 0)//如果失败则可以调用LV_GetError获取中文错误信息
            {
                string ErrStr;
                ErrStr = LedDll.LS_GetError(nResult);
                MessageBox.Show(ErrStr);
            }
            else
            {
                MessageBox.Show("发送成功");
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            int nResult;
            LedDll.COMMUNICATIONINFO CommunicationInfo = new LedDll.COMMUNICATIONINFO();//定义一通讯参数结构体变量用于对设定的LED通讯，具体对此结构体元素赋值说明见COMMUNICATIONINFO结构体定义部份注示
            //ZeroMemory(&CommunicationInfo,sizeof(COMMUNICATIONINFO));
            //TCP通讯********************************************************************************
            CommunicationInfo.SendType = 0;//设为固定IP通讯模式，即TCP通讯
            CommunicationInfo.IpStr = "192.168.1.245";//给IpStr赋值LED控制卡的IP
            CommunicationInfo.LedNumber = 1;//LED屏号为1，注意socket通讯和232通讯不识别屏号，默认赋1就行了，485必需根据屏的实际屏号进行赋值
            //广播通讯********************************************************************************
            //CommunicationInfo.SendType=1;//设为单机直连，即广播通讯无需设LED控制器的IP地址
            //串口通讯********************************************************************************
            //CommunicationInfo.SendType=2;//串口通讯
            //CommunicationInfo.Commport=1;//串口的编号，如设备管理器里显示为 COM3 则此处赋值 3
            //CommunicationInfo.Baud=9600;//波特率
            //CommunicationInfo.LedNumber=1;

            int hProgram;//节目句柄
            hProgram = LedDll.LV_CreateProgram(64, 32, 2);//根据传的参数创建节目句柄，64是屏宽点数，32是屏高点数，2是屏的颜色，注意此处屏宽高及颜色参数必需与设置屏参的屏宽高及颜色一致，否则发送时会提示错误
            //此处可自行判断有未创建成功，hProgram返回NULL失败，非NULL成功,一般不会失败

            nResult = LedDll.LV_AddProgram(hProgram, 1, 0, 1);//添加一个节目，参数说明见函数声明注示
            if (nResult != 0)
            {
                string ErrStr;
                ErrStr = LedDll.LS_GetError(nResult);
                MessageBox.Show(ErrStr);
                return;
            }
            LedDll.AREARECT AreaRect = new LedDll.AREARECT();//区域坐标属性结构体变量
            AreaRect.left = 0;
            AreaRect.top = 0;
            AreaRect.width = 64;
            AreaRect.height = 16;

            LedDll.FONTPROP FontProp = new LedDll.FONTPROP();//文字属性
            FontProp.FontName = "宋体";
            FontProp.FontSize = 12;
            FontProp.FontColor = LedDll.COLOR_RED;
            FontProp.FontBold = 0;
            //int nsize = System.Runtime.InteropServices.Marshal.SizeOf(typeof(LedDll.FONTPROP));

            nResult = LedDll.LV_QuickAddSingleLineTextArea(hProgram, 1, 1, ref AreaRect, LedDll.ADDTYPE_STRING, "上海灵信视觉技术股份有限公司", ref FontProp, 4);//快速通过字符添加一个单行文本区域，函数见函数声明注示

            AreaRect.left = 0;
            AreaRect.top = 16;
            AreaRect.width = 64;
            AreaRect.height = 16;
            LedDll.DIGITALCLOCKAREAINFO DigitalClockAreaInfo = new LedDll.DIGITALCLOCKAREAINFO();
            DigitalClockAreaInfo.TimeColor = LedDll.COLOR_RED;
         
            DigitalClockAreaInfo.ShowStrFont.FontName = "宋体";
            DigitalClockAreaInfo.ShowStrFont.FontSize = 12;
            DigitalClockAreaInfo.IsShowHour = 1;
            DigitalClockAreaInfo.IsShowMinute = 1;


            nResult = LedDll.LV_AddDigitalClockArea(hProgram, 1, 2, ref AreaRect, ref DigitalClockAreaInfo);//注意区域号不能一样，详见函数声明注示

            nResult = LedDll.LV_Send(ref CommunicationInfo, hProgram);//发送，见函数声明注示
            LedDll.LV_DeleteProgram(hProgram);//删除节目内存对象，详见函数声明注示
            if (nResult != 0)//如果失败则可以调用LV_GetError获取中文错误信息
            {
                string ErrStr;
                ErrStr = LedDll.LS_GetError(nResult);
                MessageBox.Show(ErrStr);
            }
            else
            {
                MessageBox.Show("发送成功");
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            int nResult;
            LedDll.COMMUNICATIONINFO CommunicationInfo = new LedDll.COMMUNICATIONINFO();//定义一通讯参数结构体变量用于对设定的LED通讯，具体对此结构体元素赋值说明见COMMUNICATIONINFO结构体定义部份注示
            //ZeroMemory(&CommunicationInfo,sizeof(COMMUNICATIONINFO));
            //TCP通讯********************************************************************************
            CommunicationInfo.SendType = 0;//设为固定IP通讯模式，即TCP通讯
            CommunicationInfo.IpStr = "192.168.1.245";//给IpStr赋值LED控制卡的IP
            CommunicationInfo.LedNumber = 1;//LED屏号为1，注意socket通讯和232通讯不识别屏号，默认赋1就行了，485必需根据屏的实际屏号进行赋值
            //广播通讯********************************************************************************
            //CommunicationInfo.SendType=1;//设为单机直连，即广播通讯无需设LED控制器的IP地址
            //串口通讯********************************************************************************
            //CommunicationInfo.SendType=2;//串口通讯
            //CommunicationInfo.Commport=1;//串口的编号，如设备管理器里显示为 COM3 则此处赋值 3
            //CommunicationInfo.Baud=9600;//波特率
            //CommunicationInfo.LedNumber=1;

            int hProgram;//节目句柄
            hProgram = LedDll.LV_CreateProgram(64, 32, 2);//根据传的参数创建节目句柄，64是屏宽点数，32是屏高点数，2是屏的颜色，注意此处屏宽高及颜色参数必需与设置屏参的屏宽高及颜色一致，否则发送时会提示错误
            //此处可自行判断有未创建成功，hProgram返回NULL失败，非NULL成功,一般不会失败

            nResult = LedDll.LV_AddProgram(hProgram, 1, 0, 1);//添加一个节目，参数说明见函数声明注示
            if (nResult != 0)
            {
                string ErrStr;
                ErrStr = LedDll.LS_GetError(nResult);
                MessageBox.Show(ErrStr);
                return;
            }
            LedDll.AREARECT AreaRect = new LedDll.AREARECT();//区域坐标属性结构体变量
            AreaRect.left = 0;
            AreaRect.top = 0;
            AreaRect.width = 64;
            AreaRect.height = 16;

            LedDll.FONTPROP FontProp = new LedDll.FONTPROP();//文字属性
            FontProp.FontName = "宋体";
            FontProp.FontSize = 12;
            FontProp.FontColor = LedDll.COLOR_RED;
            FontProp.FontBold = 0;
            //int nsize = System.Runtime.InteropServices.Marshal.SizeOf(typeof(LedDll.FONTPROP));

            nResult = LedDll.LV_QuickAddSingleLineTextArea(hProgram, 1, 1, ref AreaRect, LedDll.ADDTYPE_STRING, "上海灵信视觉技术股份有限公司", ref FontProp, 4);//快速通过字符添加一个单行文本区域，函数见函数声明注示

            AreaRect.left = 0;
            AreaRect.top = 16;
            AreaRect.width = 64;
            AreaRect.height = 16;
            LedDll.DIGITALCLOCKAREAINFO DigitalClockAreaInfo = new LedDll.DIGITALCLOCKAREAINFO();
            DigitalClockAreaInfo.TimeColor = LedDll.COLOR_RED;

            DigitalClockAreaInfo.ShowStrFont.FontName = "宋体";
            DigitalClockAreaInfo.ShowStrFont.FontSize = 12;
            DigitalClockAreaInfo.IsShowHour = 1;
            DigitalClockAreaInfo.IsShowMinute = 1;

            nResult = LedDll.LV_AddDigitalClockArea(hProgram, 1, 2, ref AreaRect, ref DigitalClockAreaInfo);//注意区域号不能一样，详见函数声明注示
///////////////////////////////////////////////////
            nResult = LedDll.LV_AddProgram(hProgram, 2, 0, 1);

            AreaRect.left = 0;
            AreaRect.top = 0;
            AreaRect.width = 64;
            AreaRect.height = 16;
            FontProp.FontName = "黑体";
            FontProp.FontSize = 12;
            FontProp.FontColor = LedDll.COLOR_RED;
            FontProp.FontBold = 0;
            //int nsize = System.Runtime.InteropServices.Marshal.SizeOf(typeof(LedDll.FONTPROP));

            nResult = LedDll.LV_QuickAddSingleLineTextArea(hProgram, 2, 1, ref AreaRect, LedDll.ADDTYPE_STRING, "胡半仙到此一游", ref FontProp, 4);//快速通过字符添加一个单行文本区域，函数见函数声明注示

            AreaRect.left = 0;
            AreaRect.top = 16;
            AreaRect.width = 64;
            AreaRect.height = 16;

            DigitalClockAreaInfo.ShowStrFont.FontName = "黑体";
            DigitalClockAreaInfo.ShowStrFont.FontSize = 12;
            DigitalClockAreaInfo.IsShowHour = 1;
            DigitalClockAreaInfo.IsShowMinute = 1;
            DigitalClockAreaInfo.TimeFormat = 2;

            nResult = LedDll.LV_AddDigitalClockArea(hProgram, 2, 2, ref AreaRect, ref DigitalClockAreaInfo);//注意区域号不能一样，详见函数声明注示

            nResult = LedDll.LV_Send(ref CommunicationInfo, hProgram);//发送，见函数声明注示
            LedDll.LV_DeleteProgram(hProgram);//删除节目内存对象，详见函数声明注示
            if (nResult != 0)//如果失败则可以调用LV_GetError获取中文错误信息
            {
                string ErrStr;
                ErrStr = LedDll.LS_GetError(nResult);
                MessageBox.Show(ErrStr);
            }
            else
            {
                MessageBox.Show("发送成功");
            }
        }

        private void button7_Click(object sender, EventArgs e)
        {
            int nResult;
            LedDll.COMMUNICATIONINFO CommunicationInfo = new LedDll.COMMUNICATIONINFO();//定义一通讯参数结构体变量用于对设定的LED通讯，具体对此结构体元素赋值说明见COMMUNICATIONINFO结构体定义部份注示
            //ZeroMemory(&CommunicationInfo,sizeof(COMMUNICATIONINFO));
            //TCP通讯********************************************************************************
            CommunicationInfo.SendType = 0;//设为固定IP通讯模式，即TCP通讯
            CommunicationInfo.IpStr = "192.168.1.245";//给IpStr赋值LED控制卡的IP
            CommunicationInfo.LedNumber = 1;//LED屏号为1，注意socket通讯和232通讯不识别屏号，默认赋1就行了，485必需根据屏的实际屏号进行赋值

            LedDll.ONOFFTIMEINFO Info=new LedDll.ONOFFTIMEINFO();
            Info.TimeFlag = new int[3];
            Info.StartHour = new int[3];
            Info.StartMinute = new int[3];
            Info.EndHour = new int[3];
            Info.EndMinute = new int[3];

            Info.TimeFlag[0] = 1;
            Info.StartHour[0] = 16;
            Info.StartMinute[0] = 11;
            Info.EndHour[0] = 16;
            Info.EndMinute[0] = 12;

            nResult = LedDll.LV_TimePowerOnOff(ref CommunicationInfo, ref Info);
        }

 
    }
}
