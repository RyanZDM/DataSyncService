/****** Object:  StoredProcedure [dbo].[sp_GetMonthReportData]    Script Date: 4/13/2017 4:32:28 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetMonthReportData]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_GetMonthReportData]
GO
/****** Object:  StoredProcedure [dbo].[sp_GetMonthReportData]    Script Date: 4/13/2017 4:32:28 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetMonthReportData]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'
-- =============================================
-- Author:		ZDM
-- Create date: 20170206
-- Description:	Get monthly data by the shift/item
-- =============================================
CREATE PROCEDURE [dbo].[sp_GetMonthReportData]
	@ReportId uniqueidentifier
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

 	DECLARE @MonthReport TABLE
	(
		ShiftId uniqueidentifier,
		[Day] nvarchar(10),
		[Shift] nvarchar(10),
		Worker1 nvarchar(100),
		Worker2 nvarchar(100),
		EnergyProduction1 int,
		EnergyProduction2 int,
		Biogas2GenSubtotal int,
		Biogas2TorchSubtotal int,
		SubtotalRuntime1 int,
		SubtotalRuntime2 int
	);
	
	Insert Into @MonthReport (ShiftId,[Day],[Shift])
		Select ShiftId,Convert(varchar(10), BeginTime, 111), Case When Cast(Convert(Varchar(2), BeginTime, 114) As Int) < 12 Then ''Day'' Else ''Night'' End
			From MonthReportShiftDet Where ReportId=@ReportId Order By BeginTime

	Update @MonthReport Set EnergyProduction1=IsNull(Subtotal,0) From @MonthReport mr,MonthReportDet mrd
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item=''EnergyProduction1''

	Update @MonthReport Set EnergyProduction2=IsNull(Subtotal,0) From @MonthReport mr,MonthReportDet mrd
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item=''EnergyProduction2''

	Update @MonthReport Set Biogas2GenSubtotal=IsNull(Subtotal,0) From @MonthReport mr,MonthReportDet mrd
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item=''Biogas2GenSubtotal''

	Update @MonthReport Set Biogas2TorchSubtotal=IsNull(Subtotal,0) From @MonthReport mr,MonthReportDet mrd
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item=''Biogas2TorchSubtotal''

	Update @MonthReport Set SubtotalRuntime1=IsNull(Subtotal,0) From @MonthReport mr, MonthReportDet mrd
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item=''SubtotalRuntime1''

	Update @MonthReport Set SubtotalRuntime2=IsNull(Subtotal,0) From @MonthReport mr, MonthReportDet mrd
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item=''SubtotalRuntime2''

	Update @MonthReport Set Worker1=wk.LoginName
		From @MonthReport mr, (Select ShiftId,LoginName,(ROW_NUMBER() OVER(PARTITION by ShiftId ORDER BY ShiftId)) As [Row] From WorkersInShift) wk
			Where mr.ShiftId=wk.ShiftId And wk.Row=1

	Update @MonthReport Set Worker2=wk.LoginName
		From @MonthReport mr, (Select ShiftId,LoginName,(ROW_NUMBER() OVER(PARTITION by ShiftId ORDER BY ShiftId)) As [Row] From WorkersInShift) wk
			Where mr.ShiftId=wk.ShiftId And wk.Row=2

	Select ROW_NUMBER() OVER(Order By [Day]) As [Row],[Day],[Shift],Worker1,Worker2,EnergyProduction1,EnergyProduction2,Biogas2GenSubtotal,Biogas2TorchSubtotal,SubtotalRuntime1,SubtotalRuntime2 from @MonthReport
END

' 
END
GO
