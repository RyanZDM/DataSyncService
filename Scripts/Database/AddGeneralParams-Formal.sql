USE [OPC-Site]
GO
DELETE FROM [GeneralParams]
GO
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'CreateMonthReportTime', N'2', 9, N'�±�����ʱ��', N'ÿ���µļ��ţ���Чֵ1-31', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'EnableLog', N'true', 1, N'��¼��־', N'true �� false', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'FirstDayForMonthReportTime', N'1', 8, N'�±�ͳ�Ƴ�ʼʱ��', N'ÿ���µļ��ţ���Чֵ1-31', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'KeepDbConnection', N'true', 2, N'�������ݿ������', N'true �� false', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'LedIP', N'192.168.0.99', 14, N'LED IP ��ַ', N'xxx.xxx.xxx.xxx', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'OPCServerProgID', N'OPCServer.WinCC.1', 3, N'OPC Server ProgID', NULL, 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'PathToReportFile', N'D:\MonthlyReport', 11, N'�±��ļ����·��', NULL, 0, 0, 0)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'QueryInterval', N'1000', 5, N'OPC Client��ѯ���', N'��OPC Server��ѯ�������λms', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'RefreshDataInterval', N'2000', 10, N'����ϵͳ������ѯ���', N'��λms', 0, 0, 0)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'RemoteMachine', N'', 4, N'OPC Server���ڼ������', N'���OPC Server��DataSync������ͬһ�����������Ϊ��', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ReportFilenameFormat', N'MonthlyReport-{0}-{1:D2}.xlsm', 12, N'�±��ļ�����ʽ', NULL, 0, 0, 0)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ReportFileTemplate', N'D:\MonthlyReport\Template\MonthlyReportTemplate.xlsm', 13, N'�±�Excelģ���ȫ·��', N'1', 0, 0, 0)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ShiftStartTime1', N'8:00:00', 6, N'����1��ʼʱ��', N'����ʱ�䴮������ 8:12:23', 0, 0, 1)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ShiftStartTime2', N'20:00:00', 7, N'����2��ʼʱ��', N'����ʱ�䴮������ 8:12:23', 0, 0, 1)