USE [OPC-Site]
GO
DELETE FROM [GeneralParams]
GO
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'CreateMonthReportTime', N'2', 9, N'月报创建时间', N'每个月的几号，有效值1-31', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'EnableLog', N'true', 1, N'记录日志', N'true 或 false', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'FirstDayForMonthReportTime', N'1', 8, N'月报统计初始时间', N'每个月的几号，有效值1-31', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'KeepDbConnection', N'true', 2, N'保持数据库的连接', N'true 或 false', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'LedIP', N'192.168.0.99', 14, N'LED IP 地址', N'xxx.xxx.xxx.xxx', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'OPCServerProgID', N'OPCServer.WinCC.1', 3, N'OPC Server ProgID', NULL, 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'PathToReportFile', N'D:\MonthlyReport', 11, N'月报文件存放路径', NULL, 0, 0, 0)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'QueryInterval', N'1000', 5, N'OPC Client轮询间隔', N'向OPC Server轮询间隔，单位ms', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'RefreshDataInterval', N'2000', 10, N'看板系统数据轮询间隔', N'单位ms', 0, 0, 0)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'RemoteMachine', N'', 4, N'OPC Server所在计算机名', N'如果OPC Server与DataSync服务在同一计算机，则设为空', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ReportFilenameFormat', N'MonthlyReport-{0}-{1:D2}.xlsm', 12, N'月报文件名格式', NULL, 0, 0, 0)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ReportFileTemplate', N'D:\MonthlyReport\Template\MonthlyReportTemplate.xlsm', 13, N'月报Excel模板的全路径', N'1', 0, 0, 0)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ShiftStartTime1', N'8:00:00', 6, N'工班1开始时间', N'输入时间串，例如 8:12:23', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ShiftStartTime2', N'20:00:00', 7, N'工班2开始时间', N'输入时间串，例如 8:12:23', 0, 0, 1)
