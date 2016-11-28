USE [OPC]
GO
INSERT [MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas1', N'Channel_4.Device_6.Short_1', N'Biogas1', 3, 1, 1, 1, NULL, NULL, N'A')
INSERT [MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'biogas2', N'Channel_4.Device_6.Word_1', N'Biogas2', 3, 1, 0, 1, NULL, NULL, N'A')
INSERT [MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'EnergyProduction1', N'Channel_4.Device_5.Short_1', N'EnergyProduction1', 3, 1, 1, 1, NULL, NULL, N'A')
INSERT [MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'EnergyProduction2', N'Channel_4.Device_5.Word_1', N'EnergyProduction2', 3, 1, 1, 1, N'', NULL, N'A')
INSERT [MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Runtime1', N'Channel_3.Device_4.Word_1', N'Runtime1', 3, 1, 1, 1, NULL, NULL, N'A')
INSERT [MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Runtime2', N'Channel_3.Device_4.Word_2', N'Runtime2', 3, 1, 1, 1, NULL, NULL, N'A')
INSERT [MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'TotalRuntime1', N'Channel_4.Device_6.Word_2', N'TotalRuntime1', 3, 1, 0, 0, NULL, NULL, N'A')
INSERT [MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'TotalRuntime2', N'Channel_4.Device_6.Word_3', N'TotalRuntime2', 3, 1, 0, 0, NULL, NULL, N'A')
