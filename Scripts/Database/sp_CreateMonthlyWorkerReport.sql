USE [OPC]
GO

/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyWorkerReport]    Script Date: 2016/12/24 23:18:13 ******/
DROP PROCEDURE [dbo].[sp_CreateMonthlyWorkerReport]
GO

/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyWorkerReport]    Script Date: 2016/12/24 23:18:13 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO


-- =============================================
-- Author:		ZDM
-- Create date: 2016-09-26
-- Description:	Create the monthly report for individual worker
-- =============================================
CREATE PROCEDURE [dbo].[sp_CreateMonthlyWorkerReport] 
	@ReportId uniqueidentifier,
	@BeginTime datetime,
	@EndTime datetime
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	
	-- TODO: need to consider the case that login id is null or empty

	Insert Into MonthWorkerReportDet (ReportId, WorkerId, WorkerName, Item, Subtotal, Status)
		Select @ReportId, wrk.LoginId, wrk.LoginName, det.Item, (Sum(IsNull(det.SubTotalLast,0)) - Sum(IsNull(det.SubTotalBegin,0))) As Subtotal, 'A'
			From ShiftStatMstr mstr, ShiftStatDet det, WorkersInShift wrk
			Where mstr.ShiftId = det.ShiftId And mstr.ShiftId = wrk.ShiftId And  mstr.Status = 'A'
					And mstr.BeginTime >= @BeginTime And mstr.EndTime < @EndTime
			Group By wrk.LoginId, wrk.LoginName, det.Item

	Return @@ROWCOUNT
END


GO


