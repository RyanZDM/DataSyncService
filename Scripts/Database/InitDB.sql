/****** Object:  Trigger [tr_UpdateItemSubtotal]    Script Date: 2016/12/13 23:41:07 ******/
DROP TRIGGER [dbo].[tr_UpdateItemSubtotal]
GO
ALTER TABLE [dbo].[UserInRole] DROP CONSTRAINT [FK_UseInRole_User]
GO
ALTER TABLE [dbo].[UserInRole] DROP CONSTRAINT [FK_UseInRole_Role]
GO
ALTER TABLE [dbo].[ShiftStatDet] DROP CONSTRAINT [FK_ShiftStatDet_ShiftStatMstr]
GO
ALTER TABLE [dbo].[MonthWorkerReportDet] DROP CONSTRAINT [FK_MonthWorkerReportDet_MonthReportMstr]
GO
ALTER TABLE [dbo].[MonthReportDet] DROP CONSTRAINT [FK_MonthReportDet_MonthReportMstr]
GO
ALTER TABLE [dbo].[WorkersInShift] DROP CONSTRAINT [DF_WorkersInShift_LoginTime]
GO
ALTER TABLE [dbo].[User] DROP CONSTRAINT [DF_User_Status]
GO
ALTER TABLE [dbo].[User] DROP CONSTRAINT [DF_User_IsProtected]
GO
ALTER TABLE [dbo].[User] DROP CONSTRAINT [DF_User_UserId]
GO
ALTER TABLE [dbo].[ShiftStatMstr] DROP CONSTRAINT [DF_ShiftStatMstr_Status]
GO
ALTER TABLE [dbo].[Role] DROP CONSTRAINT [DF_Role_Status]
GO
ALTER TABLE [dbo].[Role] DROP CONSTRAINT [DF_Role_RoleId]
GO
ALTER TABLE [dbo].[MonthWorkerReportDet] DROP CONSTRAINT [DF_MonthWorkerReportDet_Status]
GO
ALTER TABLE [dbo].[MonthWorkerReportDet] DROP CONSTRAINT [DF_MonthWorkerReportDet_Subtotal]
GO
ALTER TABLE [dbo].[MonthReportMstr] DROP CONSTRAINT [DF_MonthReportMstr_Status]
GO
ALTER TABLE [dbo].[MonthReportMstr] DROP CONSTRAINT [DF_MonthReportMstr_IsFileCreated]
GO
ALTER TABLE [dbo].[MonthReportMstr] DROP CONSTRAINT [DF_MonthReportMstr_CreateTime]
GO
ALTER TABLE [dbo].[MonthReportDet] DROP CONSTRAINT [DF_MonthReportDet_Status]
GO
ALTER TABLE [dbo].[MonthReportDet] DROP CONSTRAINT [DF_MonthReportDet_Subtotal]
GO
ALTER TABLE [dbo].[MonitorItem] DROP CONSTRAINT [DF_MonitorItem_Status]
GO
ALTER TABLE [dbo].[MonitorItem] DROP CONSTRAINT [DF_MonitorItem_UpdateHistory]
GO
ALTER TABLE [dbo].[MonitorItem] DROP CONSTRAINT [DF_MonitorItem_NeedAccumulate]
GO
ALTER TABLE [dbo].[ItemLatestStatus] DROP CONSTRAINT [DF_MonitorItem_LastUpdate]
GO
ALTER TABLE [dbo].[ItemLatestStatus] DROP CONSTRAINT [DF_ItemLatestStatus_Val]
GO
ALTER TABLE [dbo].[ItemHistoryData] DROP CONSTRAINT [DF_ItemHistoryData_Id]
GO
ALTER TABLE [dbo].[GeneralParams] DROP CONSTRAINT [DF__GeneralPa__IsPri__3A379A64]
GO
ALTER TABLE [dbo].[GeneralParams] DROP CONSTRAINT [DF__GeneralPa__IsEnc__3943762B]
GO
ALTER TABLE [dbo].[GeneralParams] DROP CONSTRAINT [DF__GeneralPar__Hide__384F51F2]
GO
ALTER TABLE [dbo].[GeneralParams] DROP CONSTRAINT [DF__GeneralPa__DispO__375B2DB9]
GO
/****** Object:  Index [IX_UserLoginId]    Script Date: 2016/12/13 23:41:07 ******/
ALTER TABLE [dbo].[User] DROP CONSTRAINT [IX_UserLoginId]
GO
/****** Object:  Index [PK_ItemHistoryData]    Script Date: 2016/12/13 23:41:07 ******/
ALTER TABLE [dbo].[ItemHistoryData] DROP CONSTRAINT [PK_ItemHistoryData]
GO
/****** Object:  Index [IX_ItemHistoryData]    Script Date: 2016/12/13 23:41:07 ******/
DROP INDEX [IX_ItemHistoryData] ON [dbo].[ItemHistoryData] WITH ( ONLINE = OFF )
GO
/****** Object:  Table [dbo].[WorkersInShift]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[WorkersInShift]
GO
/****** Object:  Table [dbo].[UserInRole]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[UserInRole]
GO
/****** Object:  Table [dbo].[User]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[User]
GO
/****** Object:  Table [dbo].[ShiftStatMstr]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[ShiftStatMstr]
GO
/****** Object:  Table [dbo].[ShiftStatDet]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[ShiftStatDet]
GO
/****** Object:  Table [dbo].[Role]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[Role]
GO
/****** Object:  Table [dbo].[MonthWorkerReportDet]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[MonthWorkerReportDet]
GO
/****** Object:  Table [dbo].[MonthReportMstr]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[MonthReportMstr]
GO
/****** Object:  Table [dbo].[MonthReportDet]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[MonthReportDet]
GO
/****** Object:  Table [dbo].[MonitorItem]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[MonitorItem]
GO
/****** Object:  Table [dbo].[ItemLatestStatus]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[ItemLatestStatus]
GO
/****** Object:  Table [dbo].[ItemHistoryData]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[ItemHistoryData]
GO
/****** Object:  Table [dbo].[GeneralParams]    Script Date: 2016/12/13 23:41:07 ******/
DROP TABLE [dbo].[GeneralParams]
GO

/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 2016/12/13 23:42:22 ******/
DROP PROCEDURE [dbo].[sp_GetCurrentMonthDataByDay]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyWorkerReport]    Script Date: 2016/12/13 23:42:22 ******/
DROP PROCEDURE [dbo].[sp_CreateMonthlyWorkerReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyShiftReport]    Script Date: 2016/12/13 23:42:22 ******/
DROP PROCEDURE [dbo].[sp_CreateMonthlyShiftReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyReport]    Script Date: 2016/12/13 23:42:22 ******/
DROP PROCEDURE [dbo].[sp_CreateMonthlyReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentShiftId]    Script Date: 2016/12/13 23:42:22 ******/
DROP PROCEDURE [dbo].[sp_GetCurrentShiftId]
GO
/****** Object:  UserDefinedFunction [dbo].[GetWorkerNameByShift]    Script Date: 2016/12/13 23:42:22 ******/
DROP FUNCTION [dbo].[GetWorkerNameByShift]
GO

/****** Object:  Table [dbo].[GeneralParams]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[GeneralParams](
	[Category] [varchar](50) NOT NULL,
	[Name] [varchar](50) NOT NULL,
	[Value] [nvarchar](200) NOT NULL,
	[DispOrder] [tinyint] NOT NULL,
	[DispName] [nvarchar](100) NULL,
	[Memo] [nvarchar](100) NULL,
	[Hide] [bit] NOT NULL,
	[IsEncrypted] [bit] NOT NULL,
	[IsProtected] [bit] NOT NULL,
 CONSTRAINT [PK_GENERALPARAMS] PRIMARY KEY CLUSTERED 
(
	[Category] ASC,
	[Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ItemHistoryData]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ItemHistoryData](
	[ShiftId] [uniqueidentifier] NOT NULL,
	[ItemId] [varchar](100) NOT NULL,
	[Val] [varchar](1000) NULL,
	[UpdateTime] [datetime] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ItemLatestStatus]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ItemLatestStatus](
	[ItemId] [varchar](100) NOT NULL,
	[Val] [float] NOT NULL,
	[LastUpdate] [datetime] NOT NULL,
	[Quality] [int] NULL,
 CONSTRAINT [PK_MonitorItem] PRIMARY KEY CLUSTERED 
(
	[ItemId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MonitorItem]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MonitorItem](
	[ItemId] [varchar](100) NOT NULL,
	[Address] [nvarchar](100) NOT NULL,
	[DisplayName] [nvarchar](100) NULL,
	[DataType] [int] NOT NULL,
	[NeedAccumulate] [bit] NOT NULL,
	[UpdateHistory] [bit] NOT NULL,
	[InConverter] [nvarchar](100) NULL,
	[OutConverter] [nvarchar](100) NULL,
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_MonitoredItem] PRIMARY KEY CLUSTERED 
(
	[ItemId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MonthReportDet]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MonthReportDet](
	[ReportId] [uniqueidentifier] NOT NULL,
	[ShiftId] [uniqueidentifier] NOT NULL,
	[Item] [varchar](100) NOT NULL,
	[Subtotal] [float] NOT NULL,
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_MonthReportDet] PRIMARY KEY CLUSTERED 
(
	[ReportId] ASC,
	[ShiftId] ASC,
	[Item] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MonthReportMstr]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MonthReportMstr](
	[ReportId] [uniqueidentifier] NOT NULL,
	[YearMonth] [char](6) NOT NULL,
	[BeginTime] [datetime] NOT NULL,
	[EndTime] [datetime] NOT NULL,
	[CreateTime] [datetime] NOT NULL,
	[IsFileCreated] [bit] NOT NULL,
	[FilePath] [nvarchar](256) NULL,
	[FileCreateTime] [datetime] NULL,
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_MonthReportMstr] PRIMARY KEY CLUSTERED 
(
	[ReportId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MonthWorkerReportDet]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MonthWorkerReportDet](
	[ReportId] [uniqueidentifier] NOT NULL,
	[WorkerId] [varchar](50) NOT NULL,
	[WorkerName] [nvarchar](50) NOT NULL,
	[Item] [varchar](100) NOT NULL,
	[Subtotal] [float] NOT NULL,
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_MonthWorkerReportDet] PRIMARY KEY CLUSTERED 
(
	[ReportId] ASC,
	[WorkerId] ASC,
	[Item] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Role]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Role](
	[RoleId] [varchar](50) NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_Role] PRIMARY KEY CLUSTERED 
(
	[RoleId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ShiftStatDet]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ShiftStatDet](
	[ShiftId] [uniqueidentifier] NOT NULL,
	[Item] [varchar](100) NOT NULL,
	[SubTotalBegin] [float] NOT NULL,
	[SubTotalLast] [float] NOT NULL,
 CONSTRAINT [PK_ShiftStatDet] PRIMARY KEY CLUSTERED 
(
	[ShiftId] ASC,
	[Item] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ShiftStatMstr]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ShiftStatMstr](
	[ShiftId] [uniqueidentifier] NOT NULL,
	[BeginTime] [datetime] NOT NULL,
	[ActualBeginTime] [datetime] NULL,
	[LastUpdateTime] [datetime] NULL,
	[EndTime] [datetime] NULL,
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_ShiftStatMstr] PRIMARY KEY CLUSTERED 
(
	[ShiftId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[User]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[User](
	[UserId] [uniqueidentifier] NOT NULL,
	[LoginId] [nvarchar](50) NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
	[Password] [nvarchar](50) NULL,
	[IDCard] [varchar](100) NULL,
	[IsProtected] [bit] NOT NULL,
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_User] PRIMARY KEY CLUSTERED 
(
	[UserId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[UserInRole]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[UserInRole](
	[UserId] [uniqueidentifier] NOT NULL,
	[RoleId] [varchar](50) NOT NULL,
 CONSTRAINT [PK_UseInRole] PRIMARY KEY CLUSTERED 
(
	[UserId] ASC,
	[RoleId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[WorkersInShift]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[WorkersInShift](
	[ShiftId] [uniqueidentifier] NOT NULL,
	[LoginId] [varchar](50) NOT NULL,
	[LoginName] [nvarchar](50) NOT NULL,
	[LoginTime] [datetime] NOT NULL,
 CONSTRAINT [PK_WorkersInShift] PRIMARY KEY CLUSTERED 
(
	[ShiftId] ASC,
	[LoginId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_ItemHistoryData]    Script Date: 2016/12/13 23:41:07 ******/
CREATE CLUSTERED INDEX [IX_ItemHistoryData] ON [dbo].[ItemHistoryData]
(
	[ItemId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'CreateMonthReportTime', N'30', 9, N'月报创建时间', N'每个月的几号，有效值1-31', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'EnableLog', N'true', 1, N'记录日志', N'true 或 false', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'FirstDayForMonthReportTime', N'1', 8, N'月报统计初始时间', N'每个月的几号，有效值1-31', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'KeepDbConnection', N'true', 2, N'保持数据库的连接', N'true 或 false', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'OPCServerProgID', N'KEPware.KEPServerEx.V4', 3, N'OPC Server ProgID', NULL, 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'PathToReportFile', N'D:\MonthlyReport', 11, N'月报文件存放路径', NULL, 0, 0, 0)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'QueryInterval', N'1000', 5, N'OPC Client轮询间隔', N'向OPC Server轮询间隔，单位ms', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'RefreshDataInterval', N'2000', 10, N'看板系统数据轮询间隔', N'单位ms', 0, 0, 0)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'RemoteMachine', N'', 4, N'OPC Server所在计算机名', N'如果OPC Server与DataSync服务在同一计算机，则设为空', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ReportFilenameFormat', N'MonthlyReport-{0}-{1:D2}.xlsx', 12, N'月报文件名格式', NULL, 0, 0, 0)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ReportFileTemplate', N'D:\MonthlyReport\Template\MonthlyReportTemplate.xlsx', 13, N'月报Excel模板的全路径', N'1', 0, 0, 0)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ShiftStartTime1', N'8:00:00', 6, N'工班1开始时间', N'输入时间串，例如 8:12:23', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ShiftStartTime2', N'20:00:00', 7, N'工班2开始时间', N'输入时间串，例如 8:12:23', 0, 0, 1)
INSERT [dbo].[ItemHistoryData] ([ShiftId], [ItemId], [Val], [UpdateTime]) VALUES (N'952f3f84-f9f1-4a04-aa91-df1cf221a8dc', N'Biogas1', N'20', CAST(0x0000A6C100FDA0E8 AS DateTime))
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'Biogas2Gen', 213.996094, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'Biogas2GenSubtotal.H', 1.25, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'Biogas2GenSubtotal.L', 0, CAST(0x0000A6DA0177DEA8 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'Biogas2Torch', 359.566864, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'Biogas2TorchSubtotal.H', 1.25, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'Biogas2TorchSubtotal.L', 213.75, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'EnergyProduction1', 171, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'EnergyProduction2', 171, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'Generator1Running', 0, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'Generator2Running', 0, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'GeneratorPower1', 171, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'GeneratorPower2', -14192, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'SubtotalRuntime1.H', 17332, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'SubtotalRuntime1.L', 171, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'SubtotalRuntime2.H', 171, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'SubtotalRuntime2.L', 171, CAST(0x0000A6DA0177DFD4 AS DateTime), 192)
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2Gen', N'Test.Device1.预处理沼气标况流量瞬时值', N'发电机沼气消耗量瞬时值', 4, 0, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2Gen-', N'预处理沼气标况流量瞬时值', N'发电机沼气消耗量瞬时值', 4, 0, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2GenSubtotal.H', N'Test.Device1.预处理沼气标况流量累计值H', N'发电机沼气消耗量累计值H', 4, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2GenSubtotal.H-', N'预处理沼气标况流量累计值H', N'发电机沼气消耗量累计值H', 4, 1, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2GenSubtotal.L', N'Test.Device1.预处理沼气标况流量累计值L', N'发电机沼气消耗量累计值L', 4, 1, 0, N'f', NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2GenSubtotal.L-', N'预处理沼气标况流量累计值L', N'发电机沼气消耗量累计值L', 4, 1, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2Torch', N'Test.Device1.火炬沼气标况流量瞬时值', N'火炬沼气消耗量瞬时值', 4, 0, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2Torch-', N'火炬沼气标况流量瞬时值', N'火炬沼气消耗量瞬时值', 4, 0, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2TorchSubtotal.H', N'Test.Device1.火炬沼气标况流量累计值H', N'火炬沼气消耗量累计值H', 4, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2TorchSubtotal.H-', N'火炬沼气标况流量累计值H', N'火炬沼气消耗量累计值H', 4, 1, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2TorchSubtotal.L', N'Test.Device1.火炬沼气标况流量累计值L', N'火炬沼气消耗量累计值L', 4, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas2TorchSubtotal.L-', N'火炬沼气标况流量累计值L', N'火炬沼气消耗量累计值L', 4, 1, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'EnergyProduction1', N'Test.Device1.机组1发电机kWh', N'机组1.发电机kWh', 3, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'EnergyProduction1-', N'机组1.发电机kWh', N'机组1.发电机kWh', 3, 1, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'EnergyProduction2', N'Test.Device1.机组2发电机kWh', N'机组2.发电机kWh', 3, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'EnergyProduction2-', N'机组2.发电机kWh', N'机组2.发电机kWh', 3, 1, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Generator1Running', N'Test.Device1.机组1发动机运行', N'机组1.发动机运行', 11, 0, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Generator1Running-', N'机组1.发动机运行', N'机组1.发动机运行', 11, 0, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Generator2Running', N'Test.Device1.机组2发动机运行', N'机组2.发动机运行', 11, 0, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Generator2Running-', N'机组2.发动机运行', N'机组2.发动机运行', 11, 0, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'GeneratorPower1', N'Test.Device1.机组1发电机P', N'机组1.发电机P', 2, 0, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'GeneratorPower1-', N'机组1.发电机P', N'机组1.发电机P', 2, 0, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'GeneratorPower2', N'Test.Device1.机组2发电机P', N'机组2.发电机P', 2, 0, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'GeneratorPower2-', N'机组2.发电机P', N'机组2.发电机P', 2, 0, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'SubtotalRuntime1.H', N'Test.Device1.机组1运行时间H', N'机组1.运行时间H', 2, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'SubtotalRuntime1.H-', N'机组1.运行时间H', N'机组1.运行时间H', 2, 1, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'SubtotalRuntime1.L', N'Test.Device1.机组1运行时间L', N'机组1.运行时间L', 2, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'SubtotalRuntime1.L-', N'机组1.运行时间L', N'机组1.运行时间L', 2, 1, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'SubtotalRuntime2.H', N'Test.Device1.机组2运行时间H', N'机组2.运行时间H', 2, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'SubtotalRuntime2.H-', N'机组2.运行时间H', N'机组2.运行时间H', 2, 1, 0, NULL, NULL, N'X')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'SubtotalRuntime2.L', N'Test.Device1.机组2运行时间L', N'机组2.运行时间L', 2, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'SubtotalRuntime2.L-', N'机组2.运行时间L', N'机组2.运行时间L', 2, 1, 0, NULL, NULL, N'X')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'BiogasSubtotal1', 176, N'A')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'BiogasSubtotal2', 176, N'A')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'EnergyProduction1', 176, N'A')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'EnergyProduction2', 176, N'A')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'TotalRuntime1', 176, N'A')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'TotalRuntime2', 176, N'A')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'dbc85c90-c478-40d8-82db-a868158c446b', N'BiogasSubtotal1', 683, N'A')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'dbc85c90-c478-40d8-82db-a868158c446b', N'BiogasSubtotal2', 186, N'A')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'dbc85c90-c478-40d8-82db-a868158c446b', N'EnergyProduction1', 681, N'A')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'dbc85c90-c478-40d8-82db-a868158c446b', N'EnergyProduction2', 681, N'A')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'dbc85c90-c478-40d8-82db-a868158c446b', N'TotalRuntime1', 681, N'A')
INSERT [dbo].[MonthReportDet] ([ReportId], [ShiftId], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'dbc85c90-c478-40d8-82db-a868158c446b', N'TotalRuntime2', 681, N'A')
INSERT [dbo].[MonthReportMstr] ([ReportId], [YearMonth], [BeginTime], [EndTime], [CreateTime], [IsFileCreated], [FilePath], [FileCreateTime], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'201611', CAST(0x0000A6B10083D600 AS DateTime), CAST(0x0000A6CF0083D600 AS DateTime), CAST(0x0000A6CE016B2121 AS DateTime), 0, NULL, NULL, N'A')
INSERT [dbo].[MonthWorkerReportDet] ([ReportId], [WorkerId], [WorkerName], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'id1', N'name1', N'BiogasSubtotal1', 859, N'A')
INSERT [dbo].[MonthWorkerReportDet] ([ReportId], [WorkerId], [WorkerName], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'id1', N'name1', N'BiogasSubtotal2', 362, N'A')
INSERT [dbo].[MonthWorkerReportDet] ([ReportId], [WorkerId], [WorkerName], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'id1', N'name1', N'EnergyProduction1', 857, N'A')
INSERT [dbo].[MonthWorkerReportDet] ([ReportId], [WorkerId], [WorkerName], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'id1', N'name1', N'EnergyProduction2', 857, N'A')
INSERT [dbo].[MonthWorkerReportDet] ([ReportId], [WorkerId], [WorkerName], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'id1', N'name1', N'TotalRuntime1', 857, N'A')
INSERT [dbo].[MonthWorkerReportDet] ([ReportId], [WorkerId], [WorkerName], [Item], [Subtotal], [Status]) VALUES (N'ce369e2c-0bf1-4f0d-8d27-c67aad7aa914', N'id1', N'name1', N'TotalRuntime2', 857, N'A')
INSERT [dbo].[Role] ([RoleId], [Name], [Status]) VALUES (N'Administrators', N'管理员', N'A')
INSERT [dbo].[Role] ([RoleId], [Name], [Status]) VALUES (N'DataMaintain', N'基础数据维护', N'A')
INSERT [dbo].[Role] ([RoleId], [Name], [Status]) VALUES (N'ReportManage', N'报表管理', N'A')
INSERT [dbo].[Role] ([RoleId], [Name], [Status]) VALUES (N'Users', N'普通用户', N'A')
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'b784abcb-7ff8-4c48-9cf9-0d4e78393861', N'Biogas2GenSubtotal', 11.25, 10)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'b784abcb-7ff8-4c48-9cf9-0d4e78393861', N'Biogas2TorchSubtotal', 18547.5, 1213.75)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'b784abcb-7ff8-4c48-9cf9-0d4e78393861', N'SubtotalRuntime1', 164986838, 17332171)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'b784abcb-7ff8-4c48-9cf9-0d4e78393861', N'SubtotalRuntime2', 36838, 171171)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'f364e780-197f-4d87-8f0b-3844f120e6bc', N'Biogas2GenSubtotal', 0, 10)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'f364e780-197f-4d87-8f0b-3844f120e6bc', N'Biogas2TorchSubtotal', 1, 23)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'f364e780-197f-4d87-8f0b-3844f120e6bc', N'EnergyProduction1', 2, 2345)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'f364e780-197f-4d87-8f0b-3844f120e6bc', N'EnergyProduction2', 1, 111)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'f364e780-197f-4d87-8f0b-3844f120e6bc', N'SubtotalRuntime1', 1, 23)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'f364e780-197f-4d87-8f0b-3844f120e6bc', N'SubtotalRuntime2', 1, 23)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'33c9749e-8b8b-474b-8354-77d5d56e6668', N'Biogas2GenSubtotal', 10, 11.25)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'33c9749e-8b8b-474b-8354-77d5d56e6668', N'Biogas2TorchSubtotal', 1, 233)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'33c9749e-8b8b-474b-8354-77d5d56e6668', N'EnergyProduction1', 1, 1212)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'33c9749e-8b8b-474b-8354-77d5d56e6668', N'EnergyProduction2', 2, 234)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'33c9749e-8b8b-474b-8354-77d5d56e6668', N'SubtotalRuntime1', 1, 23)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'33c9749e-8b8b-474b-8354-77d5d56e6668', N'SubtotalRuntime2', 1, 232)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'55032450-38d6-4406-b2df-8f0c1d21715e', N'Biogas2TorchSubtotal', 1, 55.55)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'4d4e6982-6768-4a2b-bcc6-b4e46e197a70', N'Biogas2GenSubtotal', 1, 10)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'4d4e6982-6768-4a2b-bcc6-b4e46e197a70', N'Biogas2TorchSubtotal', 1, 23)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'4d4e6982-6768-4a2b-bcc6-b4e46e197a70', N'SubtotalRuntime1', 1, 23)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'4d4e6982-6768-4a2b-bcc6-b4e46e197a70', N'SubtotalRuntime2', 1, 232)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'ed321b76-534c-4088-9ea2-b591838cf648', N'Biogas2GenSubtotal', 11.25, 11.25)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'ed321b76-534c-4088-9ea2-b591838cf648', N'Biogas2TorchSubtotal', 32, 18547.5)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'ed321b76-534c-4088-9ea2-b591838cf648', N'SubtotalRuntime1', 23, 176246838)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'ed321b76-534c-4088-9ea2-b591838cf648', N'SubtotalRuntime2', 232, 68386838)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'60f57150-ea79-493d-a38d-e9f209ba7037', N'Biogas2GenSubtotal', 1, 23)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'60f57150-ea79-493d-a38d-e9f209ba7037', N'Biogas2TorchSubtotal', 1, 23)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'60f57150-ea79-493d-a38d-e9f209ba7037', N'SubtotalRuntime1', 1, 3223)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'60f57150-ea79-493d-a38d-e9f209ba7037', N'SubtotalRuntime2', 1, 232)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'60f57150-ea79-493d-a38d-e9f209ba7037', N'TotalRuntime1', 3, 323)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'60f57150-ea79-493d-a38d-e9f209ba7037', N'TotalRuntime2', 3, 22)
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'b784abcb-7ff8-4c48-9cf9-0d4e78393861', CAST(0x0000A6DA01499700 AS DateTime), CAST(0x0000A6DA01732C93 AS DateTime), CAST(0x0000A6DA0177DFD4 AS DateTime), CAST(0x0000A6DB0083D600 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'8ebbc641-1923-4617-a6c6-2644a60e5d23', CAST(0x0000A6DA01499700 AS DateTime), CAST(0x0000A6DA01732C93 AS DateTime), NULL, CAST(0x0000A6DB0083D600 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'f364e780-197f-4d87-8f0b-3844f120e6bc', CAST(0x0000A6D60083D600 AS DateTime), CAST(0x0000A6D50083D69F AS DateTime), CAST(0x0000A6D5016253D0 AS DateTime), CAST(0x0000A6D601499700 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'9a8e7430-073c-4ffe-9025-77472c9fdc34', CAST(0x0000A6CE0083D600 AS DateTime), CAST(0x0000A6CE014685E4 AS DateTime), NULL, CAST(0x0000A6CE01499700 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'33c9749e-8b8b-474b-8354-77d5d56e6668', CAST(0x0000A6D601499700 AS DateTime), CAST(0x0000A6D5015E7471 AS DateTime), CAST(0x0000A6D700024EA0 AS DateTime), CAST(0x0000A6D70083D600 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'55032450-38d6-4406-b2df-8f0c1d21715e', CAST(0x0000A6D101499700 AS DateTime), CAST(0x0000A6D1015AC33F AS DateTime), CAST(0x0000A6D001780A04 AS DateTime), CAST(0x0000A6D20083D600 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', CAST(0x0000A6CD01499700 AS DateTime), CAST(0x0000A6CD0188B80C AS DateTime), CAST(0x0000A6CD018A6410 AS DateTime), CAST(0x0000A6CE0083D600 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'dbc85c90-c478-40d8-82db-a868158c446b', CAST(0x0000A6CC01499700 AS DateTime), CAST(0x0000A6CC01824B99 AS DateTime), CAST(0x0000A6CD0014670C AS DateTime), CAST(0x0000A6CD0083D600 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'289cc7e2-ca46-4dc6-b220-b209a0db2b27', CAST(0x0000A6CE01499700 AS DateTime), CAST(0x0000A6CE0165E404 AS DateTime), CAST(0x0000A6CE016B3B1C AS DateTime), CAST(0x0000A6CF0083D600 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'4d4e6982-6768-4a2b-bcc6-b4e46e197a70', CAST(0x0000A6D401499700 AS DateTime), CAST(0x0000A6D401771B32 AS DateTime), CAST(0x0000A6D500156300 AS DateTime), CAST(0x0000A6D50083D600 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'ed321b76-534c-4088-9ea2-b591838cf648', CAST(0x0000A6D70083D600 AS DateTime), CAST(0x0000A6D700AB1545 AS DateTime), CAST(0x0000A6D700CF9054 AS DateTime), CAST(0x0000A6D701499700 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'cb33d4bd-2604-408c-afcc-cb7be4ec9096', CAST(0x0000A6CF01499700 AS DateTime), CAST(0x0000A6CF01779807 AS DateTime), CAST(0x0000A6D000057030 AS DateTime), CAST(0x0000A6D00083D600 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'410daf72-58cb-4a05-b36a-e3fe6ef6270f', CAST(0x0000A6D401499700 AS DateTime), CAST(0x0000A6D401771B32 AS DateTime), NULL, CAST(0x0000A6D50083D600 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'60f57150-ea79-493d-a38d-e9f209ba7037', CAST(0x0000A6D20083D600 AS DateTime), CAST(0x0000A6D201151703 AS DateTime), CAST(0x0000A6D2014995D4 AS DateTime), CAST(0x0000A6D201499700 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [Status]) VALUES (N'7e319cfc-a119-462b-8273-fc716932eb4a', CAST(0x0000A6D001499700 AS DateTime), CAST(0x0000A6D0016C01DB AS DateTime), CAST(0x0000A6D00177CF6C AS DateTime), CAST(0x0000A6D10083D600 AS DateTime), N'A')
INSERT [dbo].[User] ([UserId], [LoginId], [Name], [Password], [IDCard], [IsProtected], [Status]) VALUES (N'11111111-1111-1111-1111-111111111111', N'Admin', N'管理员', N'admin', NULL, 1, N'A')
INSERT [dbo].[User] ([UserId], [LoginId], [Name], [Password], [IDCard], [IsProtected], [Status]) VALUES (N'ae0e7169-40b3-4ee8-b8a8-709cc715f575', N'yjf', N'俞锦峰', N'123', NULL, 0, N'A')
INSERT [dbo].[UserInRole] ([UserId], [RoleId]) VALUES (N'11111111-1111-1111-1111-111111111111', N'Administrators')
INSERT [dbo].[UserInRole] ([UserId], [RoleId]) VALUES (N'ae0e7169-40b3-4ee8-b8a8-709cc715f575', N'Administrators')
INSERT [dbo].[WorkersInShift] ([ShiftId], [LoginId], [LoginName], [LoginTime]) VALUES (N'33c9749e-8b8b-474b-8354-77d5d56e6668', N'id2', N'name2', CAST(0x0000A6D6000A6640 AS DateTime))
INSERT [dbo].[WorkersInShift] ([ShiftId], [LoginId], [LoginName], [LoginTime]) VALUES (N'33c9749e-8b8b-474b-8354-77d5d56e6668', N'id3', N'name3', CAST(0x0000A6D6001674A9 AS DateTime))
INSERT [dbo].[WorkersInShift] ([ShiftId], [LoginId], [LoginName], [LoginTime]) VALUES (N'60f57150-ea79-493d-a38d-e9f209ba7037', N'id1', N'name1', CAST(0x0000A6D201257BD1 AS DateTime))
INSERT [dbo].[WorkersInShift] ([ShiftId], [LoginId], [LoginName], [LoginTime]) VALUES (N'60f57150-ea79-493d-a38d-e9f209ba7037', N'id2', N'name2', CAST(0x0000A6D20126DD8D AS DateTime))
/****** Object:  Index [PK_ItemHistoryData]    Script Date: 2016/12/13 23:41:07 ******/
ALTER TABLE [dbo].[ItemHistoryData] ADD  CONSTRAINT [PK_ItemHistoryData] PRIMARY KEY NONCLUSTERED 
(
	[ShiftId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_UserLoginId]    Script Date: 2016/12/13 23:41:07 ******/
ALTER TABLE [dbo].[User] ADD  CONSTRAINT [IX_UserLoginId] UNIQUE NONCLUSTERED 
(
	[LoginId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
ALTER TABLE [dbo].[GeneralParams] ADD  CONSTRAINT [DF__GeneralPa__DispO__375B2DB9]  DEFAULT ((0)) FOR [DispOrder]
GO
ALTER TABLE [dbo].[GeneralParams] ADD  CONSTRAINT [DF__GeneralPar__Hide__384F51F2]  DEFAULT ((0)) FOR [Hide]
GO
ALTER TABLE [dbo].[GeneralParams] ADD  CONSTRAINT [DF__GeneralPa__IsEnc__3943762B]  DEFAULT ((0)) FOR [IsEncrypted]
GO
ALTER TABLE [dbo].[GeneralParams] ADD  CONSTRAINT [DF__GeneralPa__IsPri__3A379A64]  DEFAULT ((0)) FOR [IsProtected]
GO
ALTER TABLE [dbo].[ItemHistoryData] ADD  CONSTRAINT [DF_ItemHistoryData_Id]  DEFAULT (newid()) FOR [ShiftId]
GO
ALTER TABLE [dbo].[ItemLatestStatus] ADD  CONSTRAINT [DF_ItemLatestStatus_Val]  DEFAULT ((0)) FOR [Val]
GO
ALTER TABLE [dbo].[ItemLatestStatus] ADD  CONSTRAINT [DF_MonitorItem_LastUpdate]  DEFAULT (getdate()) FOR [LastUpdate]
GO
ALTER TABLE [dbo].[MonitorItem] ADD  CONSTRAINT [DF_MonitorItem_NeedAccumulate]  DEFAULT ((0)) FOR [NeedAccumulate]
GO
ALTER TABLE [dbo].[MonitorItem] ADD  CONSTRAINT [DF_MonitorItem_UpdateHistory]  DEFAULT ((0)) FOR [UpdateHistory]
GO
ALTER TABLE [dbo].[MonitorItem] ADD  CONSTRAINT [DF_MonitorItem_Status]  DEFAULT ('A') FOR [Status]
GO
ALTER TABLE [dbo].[MonthReportDet] ADD  CONSTRAINT [DF_MonthReportDet_Subtotal]  DEFAULT ((0)) FOR [Subtotal]
GO
ALTER TABLE [dbo].[MonthReportDet] ADD  CONSTRAINT [DF_MonthReportDet_Status]  DEFAULT ('A') FOR [Status]
GO
ALTER TABLE [dbo].[MonthReportMstr] ADD  CONSTRAINT [DF_MonthReportMstr_CreateTime]  DEFAULT (getdate()) FOR [CreateTime]
GO
ALTER TABLE [dbo].[MonthReportMstr] ADD  CONSTRAINT [DF_MonthReportMstr_IsFileCreated]  DEFAULT ((0)) FOR [IsFileCreated]
GO
ALTER TABLE [dbo].[MonthReportMstr] ADD  CONSTRAINT [DF_MonthReportMstr_Status]  DEFAULT ('A') FOR [Status]
GO
ALTER TABLE [dbo].[MonthWorkerReportDet] ADD  CONSTRAINT [DF_MonthWorkerReportDet_Subtotal]  DEFAULT ((0)) FOR [Subtotal]
GO
ALTER TABLE [dbo].[MonthWorkerReportDet] ADD  CONSTRAINT [DF_MonthWorkerReportDet_Status]  DEFAULT ('A') FOR [Status]
GO
ALTER TABLE [dbo].[Role] ADD  CONSTRAINT [DF_Role_RoleId]  DEFAULT (newid()) FOR [RoleId]
GO
ALTER TABLE [dbo].[Role] ADD  CONSTRAINT [DF_Role_Status]  DEFAULT ('A') FOR [Status]
GO
ALTER TABLE [dbo].[ShiftStatMstr] ADD  CONSTRAINT [DF_ShiftStatMstr_Status]  DEFAULT ('A') FOR [Status]
GO
ALTER TABLE [dbo].[User] ADD  CONSTRAINT [DF_User_UserId]  DEFAULT (newid()) FOR [UserId]
GO
ALTER TABLE [dbo].[User] ADD  CONSTRAINT [DF_User_IsProtected]  DEFAULT ((0)) FOR [IsProtected]
GO
ALTER TABLE [dbo].[User] ADD  CONSTRAINT [DF_User_Status]  DEFAULT ('A') FOR [Status]
GO
ALTER TABLE [dbo].[WorkersInShift] ADD  CONSTRAINT [DF_WorkersInShift_LoginTime]  DEFAULT (getdate()) FOR [LoginTime]
GO
ALTER TABLE [dbo].[MonthReportDet]  WITH CHECK ADD  CONSTRAINT [FK_MonthReportDet_MonthReportMstr] FOREIGN KEY([ReportId])
REFERENCES [dbo].[MonthReportMstr] ([ReportId])
GO
ALTER TABLE [dbo].[MonthReportDet] CHECK CONSTRAINT [FK_MonthReportDet_MonthReportMstr]
GO
ALTER TABLE [dbo].[MonthWorkerReportDet]  WITH CHECK ADD  CONSTRAINT [FK_MonthWorkerReportDet_MonthReportMstr] FOREIGN KEY([ReportId])
REFERENCES [dbo].[MonthReportMstr] ([ReportId])
GO
ALTER TABLE [dbo].[MonthWorkerReportDet] CHECK CONSTRAINT [FK_MonthWorkerReportDet_MonthReportMstr]
GO
ALTER TABLE [dbo].[ShiftStatDet]  WITH CHECK ADD  CONSTRAINT [FK_ShiftStatDet_ShiftStatMstr] FOREIGN KEY([ShiftId])
REFERENCES [dbo].[ShiftStatMstr] ([ShiftId])
GO
ALTER TABLE [dbo].[ShiftStatDet] CHECK CONSTRAINT [FK_ShiftStatDet_ShiftStatMstr]
GO
ALTER TABLE [dbo].[UserInRole]  WITH CHECK ADD  CONSTRAINT [FK_UseInRole_Role] FOREIGN KEY([RoleId])
REFERENCES [dbo].[Role] ([RoleId])
GO
ALTER TABLE [dbo].[UserInRole] CHECK CONSTRAINT [FK_UseInRole_Role]
GO
ALTER TABLE [dbo].[UserInRole]  WITH CHECK ADD  CONSTRAINT [FK_UseInRole_User] FOREIGN KEY([UserId])
REFERENCES [dbo].[User] ([UserId])
GO
ALTER TABLE [dbo].[UserInRole] CHECK CONSTRAINT [FK_UseInRole_User]
GO
/****** Object:  Trigger [dbo].[tr_UpdateItemSubtotal]    Script Date: 2016/12/13 23:41:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

/****** Object:  StoredProcedure [dbo].[sp_GetCurrentShiftId]    Script Date: 2016/12/13 23:42:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- ================================================================
-- Author:		ZDM
-- Create date: 2016-11-20
-- Description:	Get the ID of current shift.
--              If the current time is out bound of shift time range
--              Create new shift and return new ID
-- ================================================================
CREATE PROCEDURE [dbo].[sp_GetCurrentShiftId] 
	@ShiftId uniqueidentifier OUTPUT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    Declare @currTime datetime
	Declare @shift1StartTime nvarchar(10)
	Declare @shift2StartTime nvarchar(10)
	Declare @today nvarchar(10)
	Declare @shift1TimeToday datetime
	Declare @shift2TimeToday datetime	
	Declare @beginTime datetime
	Declare @endTime datetime

	Select @today = CONVERT(nvarchar(10), GETDATE(), 23)
	Select @currTime = GETDATE()

	Select Top 1 @shiftId = ShiftId From ShiftStatMstr Where @currTime >= BeginTime And @currTime < EndTime
	If (@@ROWCOUNT > 0)
	Begin
		return 0
	End

	-- Need to create new shift
	Select @shiftId = NEWID()
	Select @shift1StartTime = RTrim(LTrim(Value)) From GeneralParams Where Category = 'System' And Name = 'ShiftStartTime1'
	Select @shift2StartTime = RTrim(LTrim(Value)) From GeneralParams Where Category = 'System' And Name = 'ShiftStartTime2'
	If ((@shift1StartTime Is Null) Or (@shift1StartTime = ''))
	Begin
		Select @shift1StartTime = '8:00:00'		-- Default time
	End

	If ((@shift2StartTime Is Null) Or (@shift2StartTime = ''))
	Begin
		Select @shift2StartTime = '20:00:00'		-- Default time
	End

	Select @shift1TimeToday = CAST(@today as datetime) + CAST(@shift1StartTime As datetime)
	Select @shift2TimeToday = CAST(@today as datetime) + CAST(@shift2StartTime As datetime)

	If ((@currTime >= @shift1TimeToday) And (@currTime < @shift2TimeToday))
	Begin
		Select @beginTime = @shift1TimeToday					-- 8:00:00 today
		Select @endTime = @shift2TimeToday						-- 20:00:00 today
	End
	Else If (@currTime >=@shift2TimeToday)
	Begin
		Select @beginTime = @shift2TimeToday					-- 20:00:00 today
		Select @endTime = DATEADD(d, 1, @shift1TimeToday)		-- 8:00:00 tomorrow
	End
	Else	-- @currTime < @shift1TimeToday
	Begin
		Select @beginTime = DATEADD(d, -1, @shift2TimeToday)	-- 20:00:00 yestoday
		Select @endTime = @shift1TimeToday						-- 8:00:00 today
	End

	Insert Into ShiftStatMstr (ShiftId, BeginTime,ActualBeginTime, EndTime,Status) Values (@shiftId, @beginTime, GETDATE(), @endTime, 'A')

	Return 0
END

GO

-- ================================================================
-- Author:		ZDM
-- Create date: 2016-09-26
-- Description:
-- Accumulate the subtotal value of monitored items into the tables
-- ShiftStatMstr/ShiftStatDet
-- ================================================================
CREATE TRIGGER [dbo].[tr_UpdateItemSubtotal] 
   ON [dbo].[ItemLatestStatus] 
   AFTER INSERT,UPDATE
AS 
BEGIN
	If @@ROWCOUNT = 0 Return
	
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	Declare @shiftId uniqueidentifier
	Declare @item varchar(100)
	Declare @val float			
	Declare @valHight float, @valLow float
	Declare @realValue float	-- combination of xxx.H and xxx.L
	Declare @realItem varchar(100)
	Declare @needAccumulate bit
	Declare @newTime datetime
	Declare @suffix char(2), @anotherSuffix char(2)
	Declare @anotherItem varchar(100)
	Declare @anotherValue float
    	
	Select @item = ItemID, @suffix = Upper(Right(ItemID,2)), @val = Val, @newTime = LastUpdate From inserted
	
	Select @needAccumulate = IsNull(NeedAccumulate,0) From MonitorItem Where ItemID = @item
	If @needAccumulate = 0 Return

	If ((@suffix = '.H') Or (@suffix = '.L'))
		-- Need to combine the value of .H and .L to one single value. Assume the value of .H is a integer
		-- For example: xxx.H = 12, xxx.L = 345.67, then the real value is 12345.67
		Begin
			Select @realItem = Left(@item, Len(@item) - 2)
			If (@suffix = '.H')
				Begin
					Select @anotherSuffix = '.L'
					Select @anotherItem = @realItem + @anotherSuffix		-- xxx.L
					Select @anotherValue = Val From ItemLatestStatus Where ItemId = @anotherItem
					If (@@ROWCOUNT = 1)
						Select @realValue = CAST((CAST(Floor(@val) As varchar(100)) + CAST(@anotherValue as varchar(100))) As float)
					Else	-- Not found the value of xxx.L
						Select @realValue = @val
				End
			Else
				Begin
					Select @anotherSuffix = '.H'
					Select @anotherItem = @realItem + @anotherSuffix		-- xxx.H
					Select @anotherValue = Floor(Val) From ItemLatestStatus Where ItemId = @anotherItem
					If (@@ROWCOUNT = 1)
						Select @realValue = CAST((CAST(@anotherValue As varchar(100)) + CAST(@val as varchar(100))) As float)	
					Else	-- Not found the value of xxx.H
						Select @realValue = @val
				End			
		End
	Else	-- No need to combine
		Begin
			Select @realItem = @item
			Select @realValue = @val
		End
	
	EXEC sp_GetCurrentShiftId @ShiftId = @shiftId OUTPUT

	Update ShiftStatMstr Set LastUpdateTime = @newTime Where ShiftId = @shiftId
	
	Update ShiftStatDet Set SubTotalLast = @realValue Where ShiftId = @shiftId And Item = @realItem
	If @@ROWCOUNT = 0	-- Record not exist
	Begin
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shiftId, @realItem, @realValue, @realValue)
	End
END

GO

/****** Object:  UserDefinedFunction [dbo].[GetWorkerNameByShift]    Script Date: 2016/12/13 23:42:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- ===============================================================
-- Author:		ZDM
-- Create date: 2016-12-7
-- Description:	Combin name of all workers into one single string
-- ===============================================================
CREATE FUNCTION [dbo].[GetWorkerNameByShift]
(
	@shiftId uniqueidentifier
)
RETURNS nvarchar(200)
AS
BEGIN
	DECLARE @worker nvarchar(50)
	DECLARE @workerList nvarchar(200)
	SELECT @workerList = ''

	IF @shiftId IS NULL
		RETURN ''

	DECLARE cursorWorker CURSOR FOR
		SELECT LoginName FROM WorkersInShift WHERE ShiftId=@shiftId 

	OPEN cursorWorker
	FETCH NEXT FROM cursorWorker INTO @worker
	WHILE @@FETCH_STATUS = 0
	BEGIN
		SELECT @workerList = @workerList + @worker + ','
		FETCH NEXT FROM cursorWorker INTO @worker
	END

	IF LEN(@workerList) > 0
		SELECT @workerList = LEFT(@workerList, LEN(@workerList) - 1)

	RETURN @workerList

END

GO

/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyShiftReport]    Script Date: 2016/12/13 23:42:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		ZDM
-- Create date: 2016-01-26
-- Description:	Create the monthly report for individual shift
-- =============================================
CREATE PROCEDURE [dbo].[sp_CreateMonthlyShiftReport] 
	@ReportId uniqueidentifier,
	@BeginTime datetime,
	@EndTime datetime
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
		
	Insert Into MonthReportDet (ReportId, ShiftId, Item, Subtotal, Status)
		Select @ReportId, mstr.ShiftId, det.Item, (Sum(IsNull(det.SubTotalLast,0.0)) - Sum(IsNull(det.SubTotalBegin,0.0))) As Subtotal, 'A'
		From ShiftStatMstr mstr, ShiftStatDet det
		Where mstr.ShiftId = det.ShiftId And mstr.Status = 'A'
				And mstr.BeginTime >= @BeginTime And mstr.EndTime < @EndTime
		Group By mstr.ShiftId, det.Item

	Return @@ROWCOUNT
END

GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyWorkerReport]    Script Date: 2016/12/13 23:42:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		ZDM
-- Create date: 2016-09-26
-- Description:	Create the monthly report for individual worker
-- =============================================
CREATE PROCEDURE [dbo].[sp_CreateMonthlyWorkerReport] 
	@ReportId uniqueidentifier,
	@BeginTime datetime,
	@EndTime datetime
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	
	-- TODO: need to consider the case that login id is null or empty

	Insert Into MonthWorkerReportDet (ReportId, WorkerId, WorkerName, Item, Subtotal, Status)
		Select @ReportId, wrk.LoginId, wrk.LoginName, det.Item, (Sum(IsNull(det.SubTotalLast,0.0)) - Sum(IsNull(det.SubTotalBegin,0.0))) As Subtotal, 'A'
			From ShiftStatMstr mstr, ShiftStatDet det, WorkersInShift wrk
			Where mstr.ShiftId = det.ShiftId And mstr.ShiftId = wrk.ShiftId And  mstr.Status = 'A'
					And mstr.BeginTime >= @BeginTime And mstr.EndTime < @EndTime
			Group By wrk.LoginId, wrk.LoginName, det.Item

	Return @@ROWCOUNT
END

GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 2016/12/13 23:42:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		ZDM
-- Create date: 2016-10-12
-- Description:	Get the subtotal of each monitor
-- items by day and shift
-- =============================================
CREATE PROCEDURE [dbo].[sp_GetCurrentMonthDataByDay]
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	DECLARE @now datetime
		,@todayStr varchar(10)
		,@todayInt int
		,@dateStr char(8)
		,@noon datetime
		,@begin datetime		-- First day of current month
		,@end datetime			-- By the end of today
		,@lastDay int			-- Today
		,@day int
		,@dayWorkers nvarchar(100)

	DECLARE @shiftId uniqueidentifier
	
	SELECT @now = GETDATE()
	SELECT @todayInt = DATEPART(day, @now)
	SELECT @dateStr = LEFT(CONVERT(char(10), @now, 111), 8)	
	SELECT @begin = CONVERT(datetime, LEFT(CONVERT(char(10), @now, 111), 8) + '01', 111)	
	SELECT @end = DATEADD(m, 1, @begin)	
	SELECT @lastDay = DAY(DATEADD(d, -1, @end))
		
	DECLARE @CurrentMonthRpt TABLE
	(
		[Day] int,
		DayWorkers nvarchar(100),
		BiogasDay float,
		EngeryProductionDay float,
		NightWorkers nvarchar(100),
		BiogasNight float,
		EngeryProductionNight float
	);

	SELECT @day = 1
	WHILE @day <= @todayInt		-- No need to get the data after today since no data
	BEGIN
		SELECT @todayStr = @dateStr + CONVERT(varchar(2), @day)
		SELECT @begin = CONVERT(datetime, @todayStr, 111)
		SELECT @end = DATEADD(d, 1, @begin)
		SELECT @noon = CAST(@todayStr as datetime) + CAST('12:00:00' As datetime)	-- Uses 12:00:00 as a flag to check the two shift

		-- Day shift
		SELECT @dayWorkers=dbo.GetWorkerNameByShift(ShiftId) FROM ShiftStatMstr WHERE BeginTime>=@begin AND BeginTime<@noon

		INSERT INTO @CurrentMonthRpt([Day], DayWorkers)
			VALUES(@day, ISNULL(@dayWorkers,'') )

		UPDATE @CurrentMonthRpt SET BiogasDay=
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0.0)) - SUM(ISNULL(det.SubTotalBegin,0.0))), 0.0) FROM ShiftStatDet det, ShiftStatMstr mstr
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status='A'
						AND (det.Item='Biogas2GenSubtotal' OR det.Item='Biogas2TorchSubtotal')
						AND mstr.BeginTime>=@begin AND mstr.BeginTime<@noon)			
			WHERE  [Day]=@day
		
		UPDATE @CurrentMonthRpt SET EngeryProductionDay=
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0.0)) - SUM(ISNULL(det.SubTotalBegin,0.0))), 0.0) FROM ShiftStatDet det, ShiftStatMstr mstr
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status='A'
						AND (det.Item='EnergyProduction1' OR det.Item='EnergyProduction2')
						AND mstr.BeginTime>=@begin AND mstr.BeginTime<@noon)
			WHERE  [Day]=@day


		-- Night shift
		SELECT @shiftId = (SELECT TOP 1 ShiftId FROM ShiftStatMstr WHERE BeginTime>=@noon AND BeginTime<@end)

		UPDATE @CurrentMonthRpt SET NightWorkers=dbo.GetWorkerNameByShift(@shiftId), BiogasNight=
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0.0)) - SUM(ISNULL(det.SubTotalBegin,0.0))), 0.0) FROM ShiftStatDet det, ShiftStatMstr mstr
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status='A'
						AND (det.Item='Biogas2GenSubtotal' OR det.Item='Biogas2TorchSubtotal')
						AND mstr.BeginTime>=@noon AND mstr.BeginTime<@end)
			WHERE  [Day]=@day

		UPDATE @CurrentMonthRpt SET EngeryProductionNight=
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0.0)) - SUM(ISNULL(det.SubTotalBegin,0.0))), 0.0) FROM ShiftStatDet det, ShiftStatMstr mstr
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status='A'
						AND (det.Item='EnergyProduction1' OR det.Item='EnergyProduction2')
						AND mstr.BeginTime>=@noon AND mstr.BeginTime<@end)
			WHERE  [Day]=@day

		SELECT @day = @day + 1
	END

	WHILE @day <= @lastDay
	BEGIN
		INSERT INTO @CurrentMonthRpt([Day],DayWorkers,BiogasDay,EngeryProductionDay,NightWorkers,BiogasNight,EngeryProductionNight)
			VALUES(@day,'',0,0,'',0,0)

		SELECT @day = @day + 1
	END
	
	SELECT * FROM @CurrentMonthRpt

	SELECT (ISNULL(SUM(BiogasDay),0) + ISNULL(SUM(BiogasNight),0)) AS Biogas, (ISNULL(SUM(EngeryProductionDay),0) + ISNULL(SUM(EngeryProductionNight),0)) AS EngeryProduction FROM @CurrentMonthRpt
END

GO

/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyReport]    Script Date: 2016/12/13 23:42:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =======================================================
-- Author:		ZDM
-- Create date: 2016-09-26
-- Description:
-- Create the monthly report by Shift and Worker both,
-- in the tables MonthReportMstr, MonthReportDet
-- and WorkerReportDet
-- =======================================================
CREATE PROCEDURE [dbo].[sp_CreateMonthlyReport]
	@YearMonth char(6)		-- The format should be looks like '201602' for report of Feb. 2016
	,@ReportId uniqueidentifier OUTPUT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;	
		
	Declare @year char(4)
	Declare @month nvarchar(2)
	Declare @firstDay char(2)
	Declare @shift1StartTime nvarchar(10)
	Declare @beginTime datetime
	Declare @endTime datetime
	Declare @dayOffset int
	Declare @ret int
	
	Select @ReportId = NEWID()

	-- TODO: need to check if already has report created, delete it?

	-- May defined day offset for the begin/end day of monthly report in database
	Select @year = LEFT(@YearMonth, 4)
	Select @month = RIGHT(@YearMonth, 2)
	Select @firstDay = IsNull(Value, '1') From GeneralParams Where Category = 'System' And Name='FirstDayForMonthReportTime'
	Select @shift1StartTime =IsNull(Value, '8:00:00') From GeneralParams Where Category = 'System' And Name='ShiftStartTime1'
	Select @beginTime = CAST(@year + '-' + @month + '-' + @firstDay + ' ' + @shift1StartTime AS datetime)
	Select @endTime = DATEADD(m,1,@beginTime)
	
	Begin Tran
		Insert Into MonthReportMstr (ReportId, YearMonth, BeginTime, EndTime, CreateTime, IsFileCreated, Status)
				Values(@ReportId, @yearMonth, @beginTime, @endTime, GETDATE(), 0, 'A')
		If @@ROWCOUNT <> 1
		Begin
			Rollback Tran
			Return -1
		End

		Exec @ret = sp_CreateMonthlyShiftReport @ReportId, @beginTime, @endTime
		If @ret < 0
		Begin
			Rollback Tran
			Return -2
		End

		Exec @ret = sp_CreateMonthlyWorkerReport @ReportId, @beginTime, @endTime
		If @ret < 0
		Begin
			Rollback Tran
			Return -3
		End		

	Commit Tran

	-- Remove the corresponding records from tables ProductionStatMstr, ProductionStatDet and WorkersInProduction
	-- This will improve the performance on table ProductionStatMstr, ProductionStatDet and WorkersInProduction
	-- This will not impact the result of the monthly report creation

	-- TODO: Need a flag indicate if to delete the data in tables ShiftStatMstr/ShiftStatDet
	--Delete ShiftStatDet
	--		From ShiftStatMstr mstr, ShiftStatDet det
	--		Where mstr.ShiftId = det.ShiftId And mstr.BeginTime >= @beginTime And mstr.EndTime < @endTime

	--Delete ShiftStatMstr Where BeginTime >= @beginTime And EndTime < @endTime
	
	Return 0
END

GO