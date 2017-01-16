
ALTER TABLE [dbo].[MonthReportShiftDet] DROP CONSTRAINT [FK_MonthReportShiftDet_MonthReportShiftDet]
GO

/****** Object:  Table [dbo].[MonthReportShiftDet]    Script Date: 1/16/2017 5:02:28 PM ******/
DROP TABLE [dbo].[MonthReportShiftDet]
GO

/****** Object:  Table [dbo].[MonthReportShiftDet]    Script Date: 1/16/2017 5:02:28 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[MonthReportShiftDet](
	[ReportId] [uniqueidentifier] NOT NULL,
	[ShiftId] [uniqueidentifier] NOT NULL,
	[BeginTime] [datetime] NOT NULL,
	[ActualBeginTime] [datetime] NOT NULL,
	[LastUpdateTime] [datetime] NOT NULL,
	[EndTime] [datetime] NULL,
 CONSTRAINT [PK_MonthReportShiftDet] PRIMARY KEY CLUSTERED 
(
	[ReportId] ASC,
	[ShiftId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO

ALTER TABLE [dbo].[MonthReportShiftDet]  WITH CHECK ADD  CONSTRAINT [FK_MonthReportShiftDet_MonthReportShiftDet] FOREIGN KEY([ReportId])
REFERENCES [dbo].[MonthReportMstr] ([ReportId])
GO

ALTER TABLE [dbo].[MonthReportShiftDet] CHECK CONSTRAINT [FK_MonthReportShiftDet_MonthReportShiftDet]
GO


