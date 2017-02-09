/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyShiftReport]    Script Date: 2/9/2017 2:04:16 PM ******/
DROP PROCEDURE [dbo].[sp_CreateMonthlyShiftReport]
GO

/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyShiftReport]    Script Date: 2/9/2017 2:04:16 PM ******/
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

	Insert Into MonthReportShiftDet (ReportId, ShiftId, BeginTime, ActualBeginTime, LastUpdateTime, EndTime)
		Select @ReportId, ShiftId, BeginTime, ActualBeginTime, LastUpdateTime, EndTime From ShiftStatMstr
			Where Status = 'A' And BeginTime >= @BeginTime And EndTime < @EndTime
		
	Insert Into MonthReportDet (ReportId, ShiftId, Item, Subtotal, Status)
		Select @ReportId, mstr.ShiftId, det.Item, (Sum(IsNull(det.SubTotalLast,0)) - Sum(IsNull(det.SubTotalBegin,0))) As Subtotal, 'A'
		From ShiftStatMstr mstr, ShiftStatDet det
		Where mstr.ShiftId = det.ShiftId And mstr.Status = 'A'
				And mstr.BeginTime >= @BeginTime And mstr.EndTime < @EndTime
		Group By mstr.ShiftId, det.Item

	Return @@ROWCOUNT
END



GO

