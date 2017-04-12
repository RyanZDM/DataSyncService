/****** Object:  Trigger [tr_UpdateItemSubtotal]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.triggers WHERE object_id = OBJECT_ID(N'[dbo].[tr_UpdateItemSubtotal]'))
DROP TRIGGER [dbo].[tr_UpdateItemSubtotal]
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_UseInRole_User]') AND parent_object_id = OBJECT_ID(N'[dbo].[UserInRole]'))
ALTER TABLE [dbo].[UserInRole] DROP CONSTRAINT [FK_UseInRole_User]
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_UseInRole_Role]') AND parent_object_id = OBJECT_ID(N'[dbo].[UserInRole]'))
ALTER TABLE [dbo].[UserInRole] DROP CONSTRAINT [FK_UseInRole_Role]
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_ShiftStatDet_ShiftStatMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[ShiftStatDet]'))
ALTER TABLE [dbo].[ShiftStatDet] DROP CONSTRAINT [FK_ShiftStatDet_ShiftStatMstr]
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthWorkerReportDet_MonthReportMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthWorkerReportDet]'))
ALTER TABLE [dbo].[MonthWorkerReportDet] DROP CONSTRAINT [FK_MonthWorkerReportDet_MonthReportMstr]
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthReportShiftDet_MonthReportShiftDet]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthReportShiftDet]'))
ALTER TABLE [dbo].[MonthReportShiftDet] DROP CONSTRAINT [FK_MonthReportShiftDet_MonthReportShiftDet]
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthReportDet_MonthReportMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthReportDet]'))
ALTER TABLE [dbo].[MonthReportDet] DROP CONSTRAINT [FK_MonthReportDet_MonthReportMstr]
GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_WorkersInShift_LoginTime]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[WorkersInShift] DROP CONSTRAINT [DF_WorkersInShift_LoginTime]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_User_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[User] DROP CONSTRAINT [DF_User_Status]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_User_IsProtected]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[User] DROP CONSTRAINT [DF_User_IsProtected]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_User_UserId]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[User] DROP CONSTRAINT [DF_User_UserId]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_ShiftStatMstr_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[ShiftStatMstr] DROP CONSTRAINT [DF_ShiftStatMstr_Status]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_Role_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[Role] DROP CONSTRAINT [DF_Role_Status]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_Role_RoleId]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[Role] DROP CONSTRAINT [DF_Role_RoleId]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthWorkerReportDet_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthWorkerReportDet] DROP CONSTRAINT [DF_MonthWorkerReportDet_Status]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthWorkerReportDet_Subtotal]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthWorkerReportDet] DROP CONSTRAINT [DF_MonthWorkerReportDet_Subtotal]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthReportMstr_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthReportMstr] DROP CONSTRAINT [DF_MonthReportMstr_Status]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthReportMstr_IsFileCreated]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthReportMstr] DROP CONSTRAINT [DF_MonthReportMstr_IsFileCreated]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthReportMstr_CreateTime]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthReportMstr] DROP CONSTRAINT [DF_MonthReportMstr_CreateTime]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthReportDet_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthReportDet] DROP CONSTRAINT [DF_MonthReportDet_Status]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthReportDet_Subtotal]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthReportDet] DROP CONSTRAINT [DF_MonthReportDet_Subtotal]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonitorItem_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonitorItem] DROP CONSTRAINT [DF_MonitorItem_Status]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonitorItem_UpdateHistory]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonitorItem] DROP CONSTRAINT [DF_MonitorItem_UpdateHistory]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonitorItem_NeedAccumulate]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonitorItem] DROP CONSTRAINT [DF_MonitorItem_NeedAccumulate]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonitorItem_LastUpdate]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[ItemLatestStatus] DROP CONSTRAINT [DF_MonitorItem_LastUpdate]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_ItemLatestStatus_Val]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[ItemLatestStatus] DROP CONSTRAINT [DF_ItemLatestStatus_Val]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_ItemHistoryData_Id]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[ItemHistoryData] DROP CONSTRAINT [DF_ItemHistoryData_Id]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF__GeneralPa__IsPri__3A379A64]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[GeneralParams] DROP CONSTRAINT [DF__GeneralPa__IsPri__3A379A64]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF__GeneralPa__IsEnc__3943762B]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[GeneralParams] DROP CONSTRAINT [DF__GeneralPa__IsEnc__3943762B]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF__GeneralPar__Hide__384F51F2]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[GeneralParams] DROP CONSTRAINT [DF__GeneralPar__Hide__384F51F2]
END

GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF__GeneralPa__DispO__375B2DB9]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[GeneralParams] DROP CONSTRAINT [DF__GeneralPa__DispO__375B2DB9]
END

GO
/****** Object:  Index [IX_ItemHistoryData]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N'[dbo].[ItemHistoryData]') AND name = N'IX_ItemHistoryData')
DROP INDEX [IX_ItemHistoryData] ON [dbo].[ItemHistoryData] WITH ( ONLINE = OFF )
GO
/****** Object:  Table [dbo].[WorkersInShift]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[WorkersInShift]') AND type in (N'U'))
DROP TABLE [dbo].[WorkersInShift]
GO
/****** Object:  Table [dbo].[UserInRole]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[UserInRole]') AND type in (N'U'))
DROP TABLE [dbo].[UserInRole]
GO
/****** Object:  Table [dbo].[User]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[User]') AND type in (N'U'))
DROP TABLE [dbo].[User]
GO
/****** Object:  Table [dbo].[ShiftStatMstr]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ShiftStatMstr]') AND type in (N'U'))
DROP TABLE [dbo].[ShiftStatMstr]
GO
/****** Object:  Table [dbo].[ShiftStatDet]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ShiftStatDet]') AND type in (N'U'))
DROP TABLE [dbo].[ShiftStatDet]
GO
/****** Object:  Table [dbo].[Role]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[Role]') AND type in (N'U'))
DROP TABLE [dbo].[Role]
GO
/****** Object:  Table [dbo].[MonthWorkerReportDet]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonthWorkerReportDet]') AND type in (N'U'))
DROP TABLE [dbo].[MonthWorkerReportDet]
GO
/****** Object:  Table [dbo].[MonthReportShiftDet]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonthReportShiftDet]') AND type in (N'U'))
DROP TABLE [dbo].[MonthReportShiftDet]
GO
/****** Object:  Table [dbo].[MonthReportMstr]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonthReportMstr]') AND type in (N'U'))
DROP TABLE [dbo].[MonthReportMstr]
GO
/****** Object:  Table [dbo].[MonthReportDet]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonthReportDet]') AND type in (N'U'))
DROP TABLE [dbo].[MonthReportDet]
GO
/****** Object:  Table [dbo].[MonitorItem]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonitorItem]') AND type in (N'U'))
DROP TABLE [dbo].[MonitorItem]
GO
/****** Object:  Table [dbo].[ItemLatestStatus]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ItemLatestStatus]') AND type in (N'U'))
DROP TABLE [dbo].[ItemLatestStatus]
GO
/****** Object:  Table [dbo].[ItemHistoryData]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ItemHistoryData]') AND type in (N'U'))
DROP TABLE [dbo].[ItemHistoryData]
GO
/****** Object:  Table [dbo].[GeneralParams]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[GeneralParams]') AND type in (N'U'))
DROP TABLE [dbo].[GeneralParams]
GO
/****** Object:  Table [dbo].[AbnormalChange]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[AbnormalChange]') AND type in (N'U'))
DROP TABLE [dbo].[AbnormalChange]
GO
/****** Object:  UserDefinedFunction [dbo].[GetWorkerNameByShift]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[GetWorkerNameByShift]') AND type in (N'FN', N'IF', N'TF', N'FS', N'FT'))
DROP FUNCTION [dbo].[GetWorkerNameByShift]
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentShiftId]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetCurrentShiftId]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_GetCurrentShiftId]
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetCurrentMonthDataByDay]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_GetCurrentMonthDataByDay]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyWorkerReport]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyWorkerReport]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_CreateMonthlyWorkerReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyShiftReport]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyShiftReport]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_CreateMonthlyShiftReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyReport]    Script Date: 4/12/2017 7:05:48 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyReport]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_CreateMonthlyReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyReport]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyReport]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'

-- =======================================================
-- Author:		ZDM
-- Create date: 2016-09-26
-- Description:
-- Create the monthly report by Shift and Worker both,
-- in the tables MonthReportMstr, MonthReportDet
-- and WorkerReportDet
-- =======================================================
CREATE PROCEDURE [dbo].[sp_CreateMonthlyReport]
	@YearMonth char(6)		-- The format should be looks like ''201602'' for report of Feb. 2016
	,@ReportId uniqueidentifier OUTPUT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;	
		
	Declare @year char(4)
	Declare @month nvarchar(2)
	Declare @firstDay char(2)
	Declare @shift1StartTime nvarchar(10)
	Declare @beginTime datetime
	Declare @endTime datetime
	Declare @dayOffset int
	Declare @ret int
	
	-- TODO: need to check if already has report created, delete it?
	Select @ReportId = ReportId From MonthReportMstr Where YearMonth=@YearMonth
	If @@ROWCOUNT = 1	Return 0

	Select @ReportId = NEWID()

	-- May defined day offset for the begin/end day of monthly report in database
	Select @year = LEFT(@YearMonth, 4)
	Select @month = RIGHT(@YearMonth, 2)
	Select @firstDay = IsNull(Value, ''1'') From GeneralParams Where Category = ''System'' And Name=''FirstDayForMonthReportTime''
	Select @shift1StartTime =IsNull(Value, ''8:00:00'') From GeneralParams Where Category = ''System'' And Name=''ShiftStartTime1''
	Select @beginTime = CAST(@year + ''-'' + @month + ''-'' + @firstDay + '' '' + @shift1StartTime AS datetime)
	Select @endTime = DATEADD(m,1,@beginTime)
	
	Begin Tran
		Insert Into MonthReportMstr (ReportId, YearMonth, BeginTime, EndTime, CreateTime, IsFileCreated, Status)
				Values(@ReportId, @yearMonth, @beginTime, @endTime, GETDATE(), 0, ''A'')
		If @@ROWCOUNT <> 1
		Begin
			Rollback Tran
			Return -1
		End

		Exec @ret = sp_CreateMonthlyShiftReport @ReportId, @beginTime, @endTime
		If @ret < 0
		Begin
			Rollback Tran
			Return -2
		End

		Exec @ret = sp_CreateMonthlyWorkerReport @ReportId, @beginTime, @endTime
		If @ret < 0
		Begin
			Rollback Tran
			Return -3
		End		

	Commit Tran

	-- Remove the corresponding records from tables ProductionStatMstr, ProductionStatDet and WorkersInProduction
	-- This will improve the performance on table ProductionStatMstr, ProductionStatDet and WorkersInProduction
	-- This will not impact the result of the monthly report creation

	-- TODO: Need a flag indicate if to delete the data in tables ShiftStatMstr/ShiftStatDet
	--Delete ShiftStatDet
	--		From ShiftStatMstr mstr, ShiftStatDet det
	--		Where mstr.ShiftId = det.ShiftId And mstr.BeginTime >= @beginTime And mstr.EndTime < @endTime

	--Delete ShiftStatMstr Where BeginTime >= @beginTime And EndTime < @endTime
	
	Return 1
END


' 
END
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyShiftReport]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyShiftReport]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'

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
		
	Insert Into MonthReportDet (ReportId, ShiftId, Item, Subtotal, Status)
		Select @ReportId, mstr.ShiftId, det.Item, (Sum(IsNull(det.SubTotalLast,0)) - Sum(IsNull(det.SubTotalBegin,0))) As Subtotal, ''A''
		From ShiftStatMstr mstr, ShiftStatDet det
		Where mstr.ShiftId = det.ShiftId And mstr.Status = ''A''
				And mstr.BeginTime >= @BeginTime And mstr.EndTime < @EndTime
		Group By mstr.ShiftId, det.Item

	Return @@ROWCOUNT
END


' 
END
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyWorkerReport]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyWorkerReport]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'

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
		Select @ReportId, wrk.LoginId, wrk.LoginName, det.Item, (Sum(IsNull(det.SubTotalLast,0)) - Sum(IsNull(det.SubTotalBegin,0))) As Subtotal, ''A''
			From ShiftStatMstr mstr, ShiftStatDet det, WorkersInShift wrk
			Where mstr.ShiftId = det.ShiftId And mstr.ShiftId = wrk.ShiftId And  mstr.Status = ''A''
					And mstr.BeginTime >= @BeginTime And mstr.EndTime < @EndTime
			Group By wrk.LoginId, wrk.LoginName, det.Item

	Return @@ROWCOUNT
END


' 
END
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetCurrentMonthDataByDay]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'



-- =============================================
-- Author:		ZDM
-- Create date: 2016-10-12
-- Description:	Get the subtotal of each monitor
-- items by day and shift
-- =============================================
CREATE PROCEDURE [dbo].[sp_GetCurrentMonthDataByDay]
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	DECLARE @shiftBegin datetime
		,@todayStr varchar(10)
		,@todayInt int
		,@dateStr char(8)
		,@noon datetime
		,@begin datetime		-- First day of current month
		,@end datetime			-- By the end of today
		,@lastDay int			-- Today
		,@day int
		,@dayWorkers nvarchar(100)
		,@currShift uniqueidentifier

	DECLARE @shiftId uniqueidentifier
	
	EXEC sp_GetCurrentShiftId @ShiftId = @currShift OUTPUT
	SELECT @shiftBegin = BeginTime From ShiftStatMstr Where ShiftId = @currShift
	SELECT @todayInt = DATEPART(day, @shiftBegin)
	SELECT @dateStr = LEFT(CONVERT(char(10), @shiftBegin, 111), 8)	
	SELECT @begin = CONVERT(datetime, LEFT(CONVERT(char(10), @shiftBegin, 111), 8) + ''01'', 111)	
	SELECT @end = DATEADD(m, 1, @begin)	
	SELECT @lastDay = DAY(DATEADD(d, -1, @end))
		
	DECLARE @CurrentMonthRpt TABLE
	(
		[Day] int,
		DayWorkers nvarchar(100),
		DayBiogas int,
		DayEngeryProduction int,
		NightWorkers nvarchar(100),
		NightBiogas int,
		NightEngeryProduction int
	);

	SELECT @day = 1
	WHILE @day <= @todayInt		-- No need to get the data after today since no data
	BEGIN
		SELECT @todayStr = @dateStr + CONVERT(varchar(2), @day)
		SELECT @begin = CONVERT(datetime, @todayStr, 111)
		SELECT @end = DATEADD(d, 1, @begin)
		SELECT @noon = CAST(@todayStr as datetime) + CAST(''12:00:00'' As datetime)	-- Uses 12:00:00 as a flag to check the two shift

		-- Day shift
		SELECT @dayWorkers=dbo.GetWorkerNameByShift(ShiftId) FROM ShiftStatMstr WHERE BeginTime>=@begin AND BeginTime<@noon

		INSERT INTO @CurrentMonthRpt([Day], DayWorkers)
			VALUES(@day, ISNULL(@dayWorkers,'''') )

		UPDATE @CurrentMonthRpt SET DayBiogas=
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0)) - SUM(ISNULL(det.SubTotalBegin,0))), 0) FROM ShiftStatDet det, ShiftStatMstr mstr
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status=''A''
						AND (det.Item=''Biogas2GenSubtotal'' OR det.Item=''Biogas2TorchSubtotal'')
						AND mstr.BeginTime>=@begin AND mstr.BeginTime<@noon)			
			WHERE  [Day]=@day
		
		UPDATE @CurrentMonthRpt SET DayEngeryProduction=
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0)) - SUM(ISNULL(det.SubTotalBegin,0))), 0) FROM ShiftStatDet det, ShiftStatMstr mstr
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status=''A''
						AND (det.Item=''EnergyProduction1'' OR det.Item=''EnergyProduction2'')
						AND mstr.BeginTime>=@begin AND mstr.BeginTime<@noon)
			WHERE  [Day]=@day


		-- Night shift
		SELECT @shiftId = (SELECT TOP 1 ShiftId FROM ShiftStatMstr WHERE BeginTime>=@noon AND BeginTime<@end)

		UPDATE @CurrentMonthRpt SET NightWorkers=dbo.GetWorkerNameByShift(@shiftId), NightBiogas=
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0)) - SUM(ISNULL(det.SubTotalBegin,0))), 0) FROM ShiftStatDet det, ShiftStatMstr mstr
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status=''A''
						AND (det.Item=''Biogas2GenSubtotal'' OR det.Item=''Biogas2TorchSubtotal'')
						AND mstr.BeginTime>=@noon AND mstr.BeginTime<@end)
			WHERE  [Day]=@day

		UPDATE @CurrentMonthRpt SET NightEngeryProduction=
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0)) - SUM(ISNULL(det.SubTotalBegin,0))), 0) FROM ShiftStatDet det, ShiftStatMstr mstr
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status=''A''
						AND (det.Item=''EnergyProduction1'' OR det.Item=''EnergyProduction2'')
						AND mstr.BeginTime>=@noon AND mstr.BeginTime<@end)
			WHERE  [Day]=@day

		SELECT @day = @day + 1
	END

	WHILE @day <= @lastDay
	BEGIN
		INSERT INTO @CurrentMonthRpt([Day],DayWorkers,DayBiogas,DayEngeryProduction,NightWorkers,NightBiogas,NightEngeryProduction)
			VALUES(@day,'''',0,0,'''',0,0)

		SELECT @day = @day + 1
	END
	
	SELECT [Day], DayWorkers, DayBiogas, DayEngeryProduction, NightWorkers, NightBiogas, NightEngeryProduction FROM @CurrentMonthRpt

	SELECT (ISNULL(SUM(DayBiogas),0) + ISNULL(SUM(NightBiogas),0)) AS Biogas, (ISNULL(SUM(DayEngeryProduction),0) + ISNULL(SUM(NightEngeryProduction),0)) AS EngeryProduction FROM @CurrentMonthRpt
END




' 
END
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentShiftId]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetCurrentShiftId]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'


-- ================================================================
-- Author:		ZDM
-- Create date: 2016-11-20
-- Description:	Get the ID of current shift.
--              If the current time is out bound of shift time range
--              Create new shift and return new ID
-- ================================================================
CREATE PROCEDURE [dbo].[sp_GetCurrentShiftId] 
	@ShiftId uniqueidentifier OUTPUT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    Declare @currTime datetime
	Declare @shift1StartTime nvarchar(10)
	Declare @shift2StartTime nvarchar(10)
	Declare @today nvarchar(10)
	Declare @shift1TimeToday datetime
	Declare @shift2TimeToday datetime	
	Declare @beginTime datetime
	Declare @endTime datetime

	Select @today = CONVERT(nvarchar(10), GETDATE(), 23)
	Select @currTime = GETDATE()

	Begin Tran shiftMstr
		Select Top 1 @shiftId = ShiftId From ShiftStatMstr With(HoldLock, RowLock) Where Status=''A'' And @currTime >= BeginTime And @currTime < EndTime
		If (@@ROWCOUNT > 0)
		Begin
			Commit Tran shiftMstr
			return 0
		End

		-- Need to create new shift
		Select @shiftId = NEWID()
		Select @shift1StartTime = RTrim(LTrim(Value)) From GeneralParams Where Category = ''System'' And Name = ''ShiftStartTime1''
		Select @shift2StartTime = RTrim(LTrim(Value)) From GeneralParams Where Category = ''System'' And Name = ''ShiftStartTime2''
		If ((@shift1StartTime Is Null) Or (@shift1StartTime = ''''))
		Begin
			Select @shift1StartTime = ''8:00:00''		-- Default time
		End

		If ((@shift2StartTime Is Null) Or (@shift2StartTime = ''''))
		Begin
			Select @shift2StartTime = ''20:00:00''		-- Default time
		End

		Select @shift1TimeToday = CAST(@today as datetime) + CAST(@shift1StartTime As datetime)
		Select @shift2TimeToday = CAST(@today as datetime) + CAST(@shift2StartTime As datetime)

		If ((@currTime >= @shift1TimeToday) And (@currTime < @shift2TimeToday))
		Begin
			Select @beginTime = @shift1TimeToday					-- 8:00:00 today
			Select @endTime = @shift2TimeToday						-- 20:00:00 today
		End
		Else If (@currTime >=@shift2TimeToday)
		Begin
			Select @beginTime = @shift2TimeToday					-- 20:00:00 today
			Select @endTime = DATEADD(d, 1, @shift1TimeToday)		-- 8:00:00 tomorrow
		End
		Else	-- @currTime < @shift1TimeToday
		Begin
			Select @beginTime = DATEADD(d, -1, @shift2TimeToday)	-- 20:00:00 yestoday
			Select @endTime = @shift1TimeToday						-- 8:00:00 today
		End

		Insert Into ShiftStatMstr (ShiftId, BeginTime,ActualBeginTime, EndTime,Status) Values (@shiftId, @beginTime, GETDATE(), @endTime, ''A'')
	Commit Tran shiftMstr

	Return 0
END



' 
END
GO
/****** Object:  UserDefinedFunction [dbo].[GetWorkerNameByShift]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[GetWorkerNameByShift]') AND type in (N'FN', N'IF', N'TF', N'FS', N'FT'))
BEGIN
execute dbo.sp_executesql @statement = N'
-- ===============================================================
-- Author:		ZDM
-- Create date: 2016-12-7
-- Description:	Combin name of all workers into one single string
-- ===============================================================
CREATE FUNCTION [dbo].[GetWorkerNameByShift]
(
	@shiftId uniqueidentifier
)
RETURNS nvarchar(200)
AS
BEGIN
	DECLARE @worker nvarchar(50)
	DECLARE @workerList nvarchar(200)
	SELECT @workerList = ''''

	IF @shiftId IS NULL
		RETURN ''''

	DECLARE cursorWorker CURSOR FOR
		SELECT LoginName FROM WorkersInShift WHERE ShiftId=@shiftId 

	OPEN cursorWorker
	FETCH NEXT FROM cursorWorker INTO @worker
	WHILE @@FETCH_STATUS = 0
	BEGIN
		SELECT @workerList = @workerList + @worker + '',''
		FETCH NEXT FROM cursorWorker INTO @worker
	END

	CLOSE cursorWorker;  
	DEALLOCATE cursorWorker;  

	IF LEN(@workerList) > 0
		SELECT @workerList = LEFT(@workerList, LEN(@workerList) - 1)

	RETURN @workerList

END

' 
END

GO
/****** Object:  Table [dbo].[AbnormalChange]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[AbnormalChange]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[AbnormalChange](
	[ShiftId] [uniqueidentifier] NULL,
	[ItemName] [nvarchar](100) NULL,
	[PreValue] [int] NULL,
	[NewValue] [int] NULL,
	[UpdateTime] [datetime] NULL,
	[CreateTime] [datetime] NULL
) ON [PRIMARY]
END
GO
/****** Object:  Table [dbo].[GeneralParams]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[GeneralParams]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[GeneralParams](
	[Category] [varchar](50) NOT NULL,
	[Name] [varchar](50) NOT NULL,
	[Value] [nvarchar](200) NOT NULL,
	[DispOrder] [tinyint] NOT NULL,
	[DispName] [nvarchar](100) NULL,
	[Memo] [nvarchar](100) NULL,
	[Hide] [bit] NOT NULL,
	[IsEncrypted] [bit] NOT NULL,
	[IsProtected] [bit] NOT NULL,
 CONSTRAINT [PK_GENERALPARAMS] PRIMARY KEY CLUSTERED 
(
	[Category] ASC,
	[Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ItemHistoryData]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ItemHistoryData]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[ItemHistoryData](
	[ShiftId] [uniqueidentifier] NOT NULL,
	[ItemId] [varchar](100) NOT NULL,
	[Val] [varchar](1000) NULL,
	[UpdateTime] [datetime] NOT NULL,
 CONSTRAINT [PK_ItemHistoryData] PRIMARY KEY NONCLUSTERED 
(
	[ShiftId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ItemLatestStatus]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ItemLatestStatus]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[ItemLatestStatus](
	[ItemId] [varchar](100) NOT NULL,
	[Val] [float] NOT NULL,
	[LastUpdate] [datetime] NOT NULL,
	[Quality] [int] NULL,
 CONSTRAINT [PK_MonitorItem] PRIMARY KEY CLUSTERED 
(
	[ItemId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MonitorItem]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonitorItem]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[MonitorItem](
	[ItemId] [varchar](100) NOT NULL,
	[Address] [nvarchar](100) NOT NULL,
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
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MonthReportDet]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonthReportDet]') AND type in (N'U'))
BEGIN
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
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MonthReportMstr]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonthReportMstr]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[MonthReportMstr](
	[ReportId] [uniqueidentifier] NOT NULL,
	[YearMonth] [char](6) NOT NULL,
	[BeginTime] [datetime] NOT NULL,
	[EndTime] [datetime] NOT NULL,
	[CreateTime] [datetime] NOT NULL,
	[IsFileCreated] [bit] NOT NULL,
	[FilePath] [nvarchar](256) NULL,
	[FileCreateTime] [datetime] NULL,
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_MonthReportMstr] PRIMARY KEY CLUSTERED 
(
	[ReportId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MonthReportShiftDet]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonthReportShiftDet]') AND type in (N'U'))
BEGIN
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
END
GO
/****** Object:  Table [dbo].[MonthWorkerReportDet]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonthWorkerReportDet]') AND type in (N'U'))
BEGIN
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
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Role]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[Role]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[Role](
	[RoleId] [varchar](50) NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_Role] PRIMARY KEY CLUSTERED 
(
	[RoleId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ShiftStatDet]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ShiftStatDet]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[ShiftStatDet](
	[ShiftId] [uniqueidentifier] NOT NULL,
	[Item] [varchar](100) NOT NULL,
	[SubTotalBegin] [int] NOT NULL,
	[SubTotalLast] [int] NOT NULL,
 CONSTRAINT [PK_ShiftStatDet] PRIMARY KEY CLUSTERED 
(
	[ShiftId] ASC,
	[Item] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ShiftStatMstr]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ShiftStatMstr]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[ShiftStatMstr](
	[ShiftId] [uniqueidentifier] NOT NULL,
	[BeginTime] [datetime] NOT NULL,
	[ActualBeginTime] [datetime] NULL,
	[LastUpdateTime] [datetime] NULL,
	[EndTime] [datetime] NULL,
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_ShiftStatMstr] PRIMARY KEY CLUSTERED 
(
	[ShiftId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[User]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[User]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[User](
	[UserId] [uniqueidentifier] NOT NULL,
	[LoginId] [nvarchar](50) NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
	[Password] [nvarchar](50) NULL,
	[IDCard] [varchar](100) NULL,
	[IsProtected] [bit] NOT NULL,
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_User] PRIMARY KEY CLUSTERED 
(
	[UserId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY],
 CONSTRAINT [IX_UserLoginId] UNIQUE NONCLUSTERED 
(
	[LoginId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[UserInRole]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[UserInRole]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[UserInRole](
	[UserId] [uniqueidentifier] NOT NULL,
	[RoleId] [varchar](50) NOT NULL,
 CONSTRAINT [PK_UseInRole] PRIMARY KEY CLUSTERED 
(
	[UserId] ASC,
	[RoleId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[WorkersInShift]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[WorkersInShift]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[WorkersInShift](
	[ShiftId] [uniqueidentifier] NOT NULL,
	[LoginId] [varchar](50) NOT NULL,
	[LoginName] [nvarchar](50) NOT NULL,
	[LoginTime] [datetime] NOT NULL,
 CONSTRAINT [PK_WorkersInShift] PRIMARY KEY CLUSTERED 
(
	[ShiftId] ASC,
	[LoginId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_ItemHistoryData]    Script Date: 4/12/2017 7:05:48 PM ******/
IF NOT EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N'[dbo].[ItemHistoryData]') AND name = N'IX_ItemHistoryData')
CREATE CLUSTERED INDEX [IX_ItemHistoryData] ON [dbo].[ItemHistoryData]
(
	[ItemId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF__GeneralPa__DispO__375B2DB9]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[GeneralParams] ADD  CONSTRAINT [DF__GeneralPa__DispO__375B2DB9]  DEFAULT ((0)) FOR [DispOrder]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF__GeneralPar__Hide__384F51F2]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[GeneralParams] ADD  CONSTRAINT [DF__GeneralPar__Hide__384F51F2]  DEFAULT ((0)) FOR [Hide]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF__GeneralPa__IsEnc__3943762B]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[GeneralParams] ADD  CONSTRAINT [DF__GeneralPa__IsEnc__3943762B]  DEFAULT ((0)) FOR [IsEncrypted]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF__GeneralPa__IsPri__3A379A64]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[GeneralParams] ADD  CONSTRAINT [DF__GeneralPa__IsPri__3A379A64]  DEFAULT ((0)) FOR [IsProtected]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_ItemHistoryData_Id]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[ItemHistoryData] ADD  CONSTRAINT [DF_ItemHistoryData_Id]  DEFAULT (newid()) FOR [ShiftId]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_ItemLatestStatus_Val]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[ItemLatestStatus] ADD  CONSTRAINT [DF_ItemLatestStatus_Val]  DEFAULT ((0)) FOR [Val]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonitorItem_LastUpdate]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[ItemLatestStatus] ADD  CONSTRAINT [DF_MonitorItem_LastUpdate]  DEFAULT (getdate()) FOR [LastUpdate]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonitorItem_NeedAccumulate]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonitorItem] ADD  CONSTRAINT [DF_MonitorItem_NeedAccumulate]  DEFAULT ((0)) FOR [NeedAccumulate]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonitorItem_UpdateHistory]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonitorItem] ADD  CONSTRAINT [DF_MonitorItem_UpdateHistory]  DEFAULT ((0)) FOR [UpdateHistory]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonitorItem_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonitorItem] ADD  CONSTRAINT [DF_MonitorItem_Status]  DEFAULT ('A') FOR [Status]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthReportDet_Subtotal]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthReportDet] ADD  CONSTRAINT [DF_MonthReportDet_Subtotal]  DEFAULT ((0)) FOR [Subtotal]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthReportDet_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthReportDet] ADD  CONSTRAINT [DF_MonthReportDet_Status]  DEFAULT ('A') FOR [Status]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthReportMstr_CreateTime]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthReportMstr] ADD  CONSTRAINT [DF_MonthReportMstr_CreateTime]  DEFAULT (getdate()) FOR [CreateTime]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthReportMstr_IsFileCreated]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthReportMstr] ADD  CONSTRAINT [DF_MonthReportMstr_IsFileCreated]  DEFAULT ((0)) FOR [IsFileCreated]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthReportMstr_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthReportMstr] ADD  CONSTRAINT [DF_MonthReportMstr_Status]  DEFAULT ('A') FOR [Status]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthWorkerReportDet_Subtotal]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthWorkerReportDet] ADD  CONSTRAINT [DF_MonthWorkerReportDet_Subtotal]  DEFAULT ((0)) FOR [Subtotal]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_MonthWorkerReportDet_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[MonthWorkerReportDet] ADD  CONSTRAINT [DF_MonthWorkerReportDet_Status]  DEFAULT ('A') FOR [Status]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_Role_RoleId]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[Role] ADD  CONSTRAINT [DF_Role_RoleId]  DEFAULT (newid()) FOR [RoleId]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_Role_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[Role] ADD  CONSTRAINT [DF_Role_Status]  DEFAULT ('A') FOR [Status]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_ShiftStatMstr_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[ShiftStatMstr] ADD  CONSTRAINT [DF_ShiftStatMstr_Status]  DEFAULT ('A') FOR [Status]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_User_UserId]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[User] ADD  CONSTRAINT [DF_User_UserId]  DEFAULT (newid()) FOR [UserId]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_User_IsProtected]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[User] ADD  CONSTRAINT [DF_User_IsProtected]  DEFAULT ((0)) FOR [IsProtected]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_User_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[User] ADD  CONSTRAINT [DF_User_Status]  DEFAULT ('A') FOR [Status]
END

GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_WorkersInShift_LoginTime]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[WorkersInShift] ADD  CONSTRAINT [DF_WorkersInShift_LoginTime]  DEFAULT (getdate()) FOR [LoginTime]
END

GO
IF NOT EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthReportDet_MonthReportMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthReportDet]'))
ALTER TABLE [dbo].[MonthReportDet]  WITH CHECK ADD  CONSTRAINT [FK_MonthReportDet_MonthReportMstr] FOREIGN KEY([ReportId])
REFERENCES [dbo].[MonthReportMstr] ([ReportId])
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthReportDet_MonthReportMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthReportDet]'))
ALTER TABLE [dbo].[MonthReportDet] CHECK CONSTRAINT [FK_MonthReportDet_MonthReportMstr]
GO
IF NOT EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthReportShiftDet_MonthReportShiftDet]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthReportShiftDet]'))
ALTER TABLE [dbo].[MonthReportShiftDet]  WITH CHECK ADD  CONSTRAINT [FK_MonthReportShiftDet_MonthReportShiftDet] FOREIGN KEY([ReportId])
REFERENCES [dbo].[MonthReportMstr] ([ReportId])
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthReportShiftDet_MonthReportShiftDet]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthReportShiftDet]'))
ALTER TABLE [dbo].[MonthReportShiftDet] CHECK CONSTRAINT [FK_MonthReportShiftDet_MonthReportShiftDet]
GO
IF NOT EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthWorkerReportDet_MonthReportMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthWorkerReportDet]'))
ALTER TABLE [dbo].[MonthWorkerReportDet]  WITH CHECK ADD  CONSTRAINT [FK_MonthWorkerReportDet_MonthReportMstr] FOREIGN KEY([ReportId])
REFERENCES [dbo].[MonthReportMstr] ([ReportId])
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthWorkerReportDet_MonthReportMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthWorkerReportDet]'))
ALTER TABLE [dbo].[MonthWorkerReportDet] CHECK CONSTRAINT [FK_MonthWorkerReportDet_MonthReportMstr]
GO
IF NOT EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_ShiftStatDet_ShiftStatMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[ShiftStatDet]'))
ALTER TABLE [dbo].[ShiftStatDet]  WITH CHECK ADD  CONSTRAINT [FK_ShiftStatDet_ShiftStatMstr] FOREIGN KEY([ShiftId])
REFERENCES [dbo].[ShiftStatMstr] ([ShiftId])
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_ShiftStatDet_ShiftStatMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[ShiftStatDet]'))
ALTER TABLE [dbo].[ShiftStatDet] CHECK CONSTRAINT [FK_ShiftStatDet_ShiftStatMstr]
GO
IF NOT EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_UseInRole_Role]') AND parent_object_id = OBJECT_ID(N'[dbo].[UserInRole]'))
ALTER TABLE [dbo].[UserInRole]  WITH CHECK ADD  CONSTRAINT [FK_UseInRole_Role] FOREIGN KEY([RoleId])
REFERENCES [dbo].[Role] ([RoleId])
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_UseInRole_Role]') AND parent_object_id = OBJECT_ID(N'[dbo].[UserInRole]'))
ALTER TABLE [dbo].[UserInRole] CHECK CONSTRAINT [FK_UseInRole_Role]
GO
IF NOT EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_UseInRole_User]') AND parent_object_id = OBJECT_ID(N'[dbo].[UserInRole]'))
ALTER TABLE [dbo].[UserInRole]  WITH CHECK ADD  CONSTRAINT [FK_UseInRole_User] FOREIGN KEY([UserId])
REFERENCES [dbo].[User] ([UserId])
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_UseInRole_User]') AND parent_object_id = OBJECT_ID(N'[dbo].[UserInRole]'))
ALTER TABLE [dbo].[UserInRole] CHECK CONSTRAINT [FK_UseInRole_User]
GO
/****** Object:  Trigger [dbo].[tr_UpdateItemSubtotal]    Script Date: 4/12/2017 7:05:48 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.triggers WHERE object_id = OBJECT_ID(N'[dbo].[tr_UpdateItemSubtotal]'))
EXEC dbo.sp_executesql @statement = N'


-- ================================================================
-- Author:		ZDM
-- Create date: 2016-09-26
-- Description:
-- Accumulate the subtotal value of monitored items into the tables
-- ShiftStatMstr/ShiftStatDet
-- ================================================================
CREATE TRIGGER [dbo].[tr_UpdateItemSubtotal] 
   ON [dbo].[ItemLatestStatus] 
   AFTER INSERT,UPDATE
AS 
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	Declare @shiftId uniqueidentifier
	Declare @item varchar(100)
	Declare @val int			
	Declare @valHight int, @valLow int
	Declare @realValue int	-- combination of xxx.H and xxx.L
	Declare @realItem varchar(100)
	Declare @needAccumulate bit
	Declare @newTime datetime
	Declare @suffix char(2), @anotherSuffix char(2)
	Declare @anotherItem varchar(100)
	Declare @anotherValue int
    	
	Select @item = ItemID, @suffix = Upper(Right(ItemID,2)), @val = Cast(Val As int), @newTime = LastUpdate From inserted
	If @item Is NULL Return
--insert into test1(num1, num2,total, item) values(@val,-1,-1, ''-'' + @item)	
	If @val = 0 Return	

	Select @needAccumulate = IsNull(NeedAccumulate,0) From MonitorItem Where ItemID = @item
	If @needAccumulate = 0 Return
	
	If ((@suffix = ''.H'') Or (@suffix = ''.L''))
		-- Need to combine the value of .H and .L to one single value. Assume the value of .H is a integer
		-- For example: xxx.H = 12, xxx.L = 345.67, then the real value is 12345.67
		Begin
			Select @realItem = Left(@item, Len(@item) - 2)
			If (@suffix = ''.H'')
				Begin
					Select @anotherSuffix = ''.L''
					Select @anotherItem = @realItem + @anotherSuffix		-- xxx.L
					Select @anotherValue = Cast(Val As int) From ItemLatestStatus Where ItemId = @anotherItem
					If (@@ROWCOUNT = 1)
						Select @realValue = CAST((CAST(@val As varchar(100)) + CAST(@anotherValue as varchar(100))) As int)
					Else	-- Not found the value of xxx.L
						Select @realValue = @val
				End
			Else
				Begin
					Select @anotherSuffix = ''.H''
					Select @anotherItem = @realItem + @anotherSuffix		-- xxx.H
					Select @anotherValue = Cast(Val As int) From ItemLatestStatus Where ItemId = @anotherItem
					If (@@ROWCOUNT = 1)
						Select @realValue = CAST((CAST(@anotherValue As varchar(100)) + CAST(@val as varchar(100))) As int)	
					Else	-- Not found the value of xxx.H
						Select @realValue = @val
				End			
		End
	Else	-- No need to combine
		Begin
			Select @realItem = @item
			Select @realValue = @val
		End
	
	EXEC sp_GetCurrentShiftId @ShiftId = @shiftId OUTPUT

	Update ShiftStatMstr Set LastUpdateTime = @newTime Where ShiftId = @shiftId

	Declare @subtotalBegin int
	Declare @newSubtotalBegin int
	Declare @subtotalLast int
	
	Select @subtotalLast = SubTotalLast, @subtotalBegin = SubTotalBegin From ShiftStatDet Where ShiftId = @shiftId and Item = @realItem
	If @@ROWCOUNT = 0	-- Record not exist
		Begin
			Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shiftId, @realItem, @realValue, @realValue)

			-- Record the abnormal change
			Insert Into AbnormalChange (ShiftId, ItemName, PreValue, NewValue, UpdateTime, CreateTime) Values (@shiftId, @realItem, -1, @realValue, @newTime, GetDate())
			Return
		End

	If (@realValue >= @subtotalLast)	-- The value change is reasonable
		Begin
			Update ShiftStatDet Set SubTotalLast = @realValue Where ShiftId = @shiftId And Item = @realItem
		End
	Else							-- The subtotal value must be reset, need to r-calculte the SubtotalBegin
		Begin
--insert into test1(num1, num2,total, item) values(@subtotalLast,@realValue,@val, @realItem + ''-'' + @item)

			Select @newSubtotalBegin = @subtotalBegin - @subtotalLast	-- It is a neg value
			Update ShiftStatDet Set SubTotalBegin = @newSubtotalBegin, SubTotalLast = @realValue Where ShiftId = @shiftId And Item = @realItem

			-- Record the abnormal change
			Insert Into AbnormalChange (ShiftId, ItemName, PreValue, NewValue, UpdateTime, CreateTime) Values (@shiftId, @realItem, @subtotalBegin, @newSubtotalBegin, @newTime, GetDate())
			Insert Into AbnormalChange (ShiftId, ItemName, PreValue, NewValue, UpdateTime, CreateTime) Values (@shiftId, @realItem, @subtotalLast, @realValue, @newTime, GetDate())
		End	
END



' 
GO
