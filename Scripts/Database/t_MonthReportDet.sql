USE [OPC]
GO

ALTER TABLE [dbo].[MonthReportDet] DROP CONSTRAINT [FK_MonthReportDet_MonthReportMstr]
GO

ALTER TABLE [dbo].[MonthReportDet] DROP CONSTRAINT [DF_MonthReportDet_Status]
GO

ALTER TABLE [dbo].[MonthReportDet] DROP CONSTRAINT [DF_MonthReportDet_Subtotal]
GO

/****** Object:  Table [dbo].[MonthReportDet]    Script Date: 2016/12/24 23:14:21 ******/
DROP TABLE [dbo].[MonthReportDet]
GO

/****** Object:  Table [dbo].[MonthReportDet]    Script Date: 2016/12/24 23:14:21 ******/
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
	[Subtotal] [int] NOT NULL,
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

ALTER TABLE [dbo].[MonthReportDet] ADD  CONSTRAINT [DF_MonthReportDet_Subtotal]  DEFAULT ((0)) FOR [Subtotal]
GO

ALTER TABLE [dbo].[MonthReportDet] ADD  CONSTRAINT [DF_MonthReportDet_Status]  DEFAULT ('A') FOR [Status]
GO

ALTER TABLE [dbo].[MonthReportDet]  WITH CHECK ADD  CONSTRAINT [FK_MonthReportDet_MonthReportMstr] FOREIGN KEY([ReportId])
REFERENCES [dbo].[MonthReportMstr] ([ReportId])
GO

ALTER TABLE [dbo].[MonthReportDet] CHECK CONSTRAINT [FK_MonthReportDet_MonthReportMstr]
GO


