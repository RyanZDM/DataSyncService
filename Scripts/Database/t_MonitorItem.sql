ALTER TABLE [dbo].[MonitorItem] DROP CONSTRAINT [DF_MonitorItem_Status]
GO

ALTER TABLE [dbo].[MonitorItem] DROP CONSTRAINT [DF_MonitorItem_UpdateHistory]
GO

ALTER TABLE [dbo].[MonitorItem] DROP CONSTRAINT [DF_MonitorItem_NeedAccumulate]
GO

/****** Object:  Table [dbo].[MonitorItem]    Script Date: 2/9/2017 1:51:41 PM ******/
DROP TABLE [dbo].[MonitorItem]
GO

/****** Object:  Table [dbo].[MonitorItem]    Script Date: 2/9/2017 1:51:41 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [dbo].[MonitorItem](
	[ItemId] [varchar](100) NOT NULL,
	[Address] [nvarchar](100) NOT NULL,
	[Alias] [varchar](100) NULL,
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

ALTER TABLE [dbo].[MonitorItem] ADD  CONSTRAINT [DF_MonitorItem_NeedAccumulate]  DEFAULT ((0)) FOR [NeedAccumulate]
GO

ALTER TABLE [dbo].[MonitorItem] ADD  CONSTRAINT [DF_MonitorItem_UpdateHistory]  DEFAULT ((0)) FOR [UpdateHistory]
GO

ALTER TABLE [dbo].[MonitorItem] ADD  CONSTRAINT [DF_MonitorItem_Status]  DEFAULT ('A') FOR [Status]
GO


