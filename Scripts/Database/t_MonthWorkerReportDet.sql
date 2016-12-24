USE [OPC]
GO

ALTER TABLE [dbo].[MonthWorkerReportDet] DROP CONSTRAINT [FK_MonthWorkerReportDet_MonthReportMstr]
GO

ALTER TABLE [dbo].[MonthWorkerReportDet] DROP CONSTRAINT [DF_MonthWorkerReportDet_Status]
GO

ALTER TABLE [dbo].[MonthWorkerReportDet] DROP CONSTRAINT [DF_MonthWorkerReportDet_Subtotal]
GO

/****** Object:  Table [dbo].[MonthWorkerReportDet]    Script Date: 2016/12/24 23:15:46 ******/
DROP TABLE [dbo].[MonthWorkerReportDet]
GO

/****** Object:  Table [dbo].[MonthWorkerReportDet]    Script Date: 2016/12/24 23:15:46 ******/
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
	[Subtotal] [int] NOT NULL,
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

ALTER TABLE [dbo].[MonthWorkerReportDet] ADD  CONSTRAINT [DF_MonthWorkerReportDet_Subtotal]  DEFAULT ((0)) FOR [Subtotal]
GO

ALTER TABLE [dbo].[MonthWorkerReportDet] ADD  CONSTRAINT [DF_MonthWorkerReportDet_Status]  DEFAULT ('A') FOR [Status]
GO

ALTER TABLE [dbo].[MonthWorkerReportDet]  WITH CHECK ADD  CONSTRAINT [FK_MonthWorkerReportDet_MonthReportMstr] FOREIGN KEY([ReportId])
REFERENCES [dbo].[MonthReportMstr] ([ReportId])
GO

ALTER TABLE [dbo].[MonthWorkerReportDet] CHECK CONSTRAINT [FK_MonthWorkerReportDet_MonthReportMstr]
GO


