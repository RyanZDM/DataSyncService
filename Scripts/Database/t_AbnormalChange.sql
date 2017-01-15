
/****** Object:  Table [dbo].[AbnormalChange]    Script Date: 2017/1/14 21:54:23 ******/
DROP TABLE [dbo].[AbnormalChange]
GO

/****** Object:  Table [dbo].[AbnormalChange]    Script Date: 2017/1/14 21:54:23 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[AbnormalChange](
	[ShiftId] [uniqueidentifier] NULL,
	[ItemName] [nvarchar](100) NULL,
	[PreValue] [int] NULL,
	[NewValue] [int] NULL,
	[UpdateTime] [datetime] NULL,
	[CreateTime] [datetime] NULL
) ON [PRIMARY]

GO


