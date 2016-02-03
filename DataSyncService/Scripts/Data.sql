USE [OPC]
GO
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsPrimary]) VALUES (N'System', N'EnableLog', N'true', 0, NULL, NULL, 0, 0, 0)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsPrimary]) VALUES (N'System', N'OPCServerProgID', N'KEPware.KEPServerEx.V4', 0, NULL, NULL, 0, 0, 0)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsPrimary]) VALUES (N'System', N'QueryInterval', N'1000', 0, NULL, NULL, 0, 0, 0)
INSERT [GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsPrimary]) VALUES (N'System', N'RemoteMachine', N'', 0, NULL, NULL, 0, 0, 0)


INSERT [MonitorItem] ([ItemID], [DataType], [Status]) VALUES (N'Channel_4.Device_5.Short_1', 3, N'A')
INSERT [MonitorItem] ([ItemID], [DataType], [Status]) VALUES (N'Channel_4.Device_5.Word_1', 3, N'A')
