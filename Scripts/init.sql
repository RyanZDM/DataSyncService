USE [OPC]
GO
/****** Object:  Trigger [tr_UpdateItemSubtotal]    Script Date: 12/1/2016 3:34:35 PM ******/
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
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthReportDet_MonthReportMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthReportDet]'))
ALTER TABLE [dbo].[MonthReportDet] DROP CONSTRAINT [FK_MonthReportDet_MonthReportMstr]
GO
IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_User_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[User] DROP CONSTRAINT [DF_User_Status]
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
/****** Object:  Index [PK_ItemHistoryData]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N'[dbo].[ItemHistoryData]') AND name = N'PK_ItemHistoryData')
ALTER TABLE [dbo].[ItemHistoryData] DROP CONSTRAINT [PK_ItemHistoryData]
GO
/****** Object:  Index [IX_ItemHistoryData]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N'[dbo].[ItemHistoryData]') AND name = N'IX_ItemHistoryData')
DROP INDEX [IX_ItemHistoryData] ON [dbo].[ItemHistoryData] WITH ( ONLINE = OFF )
GO
/****** Object:  Table [dbo].[UserInRole]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[UserInRole]') AND type in (N'U'))
DROP TABLE [dbo].[UserInRole]
GO
/****** Object:  Table [dbo].[User]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[User]') AND type in (N'U'))
DROP TABLE [dbo].[User]
GO
/****** Object:  Table [dbo].[ShiftStatMstr]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ShiftStatMstr]') AND type in (N'U'))
DROP TABLE [dbo].[ShiftStatMstr]
GO
/****** Object:  Table [dbo].[ShiftStatDet]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ShiftStatDet]') AND type in (N'U'))
DROP TABLE [dbo].[ShiftStatDet]
GO
/****** Object:  Table [dbo].[Role]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[Role]') AND type in (N'U'))
DROP TABLE [dbo].[Role]
GO
/****** Object:  Table [dbo].[MonthWorkerReportDet]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonthWorkerReportDet]') AND type in (N'U'))
DROP TABLE [dbo].[MonthWorkerReportDet]
GO
/****** Object:  Table [dbo].[MonthReportMstr]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonthReportMstr]') AND type in (N'U'))
DROP TABLE [dbo].[MonthReportMstr]
GO
/****** Object:  Table [dbo].[MonthReportDet]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonthReportDet]') AND type in (N'U'))
DROP TABLE [dbo].[MonthReportDet]
GO
/****** Object:  Table [dbo].[MonitorItem]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonitorItem]') AND type in (N'U'))
DROP TABLE [dbo].[MonitorItem]
GO
/****** Object:  Table [dbo].[ItemLatestStatus]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ItemLatestStatus]') AND type in (N'U'))
DROP TABLE [dbo].[ItemLatestStatus]
GO
/****** Object:  Table [dbo].[ItemHistoryData]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ItemHistoryData]') AND type in (N'U'))
DROP TABLE [dbo].[ItemHistoryData]
GO
/****** Object:  Table [dbo].[GeneralParams]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[GeneralParams]') AND type in (N'U'))
DROP TABLE [dbo].[GeneralParams]
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentShiftId]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetCurrentShiftId]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_GetCurrentShiftId]
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetCurrentMonthDataByDay]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_GetCurrentMonthDataByDay]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyWorkerReport]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyWorkerReport]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_CreateMonthlyWorkerReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyShiftReport]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyShiftReport]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_CreateMonthlyShiftReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyReport]    Script Date: 12/1/2016 3:34:35 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyReport]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_CreateMonthlyReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyReport]    Script Date: 12/1/2016 3:34:35 PM ******/
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
	
	Select @ReportId = NEWID()

	-- TODO: need to check if already has report created, delete it?

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
	
	Return 0
END

' 
END
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyShiftReport]    Script Date: 12/1/2016 3:34:35 PM ******/
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
		Select @ReportId, mstr.ShiftId, det.Item, (Sum(IsNull(det.SubTotalLast,0.0)) - Sum(IsNull(det.SubTotalBegin,0.0))) As Subtotal, ''A''
		From ShiftStatMstr mstr, ShiftStatDet det
		Where mstr.ShiftId = det.ShiftId And mstr.Status = ''A''
				And mstr.BeginTime >= @BeginTime And mstr.EndTime < @EndTime
		Group By mstr.ShiftId, det.Item

	Return @@ROWCOUNT
END

' 
END
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyWorkerReport]    Script Date: 12/1/2016 3:34:35 PM ******/
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
		Select @ReportId, IsNull(mstr.LastLoginId, ''<No Worker>''), IsNull(mstr.LastLoginName, ''<No Worker>''), det.Item, (Sum(IsNull(det.SubTotalLast,0.0)) - Sum(IsNull(det.SubTotalBegin,0.0))) As Subtotal, ''A''
			From ShiftStatMstr mstr, ShiftStatDet det
			Where mstr.ShiftId = det.ShiftId And mstr.Status = ''A''
					And mstr.BeginTime >= @BeginTime And mstr.EndTime < @EndTime
			Group By mstr.LastLoginId, mstr.LastLoginName, det.Item

	Return @@ROWCOUNT
END

' 
END
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 12/1/2016 3:34:35 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetCurrentMonthDataByDay]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'-- =============================================
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

	DECLARE @Now datetime
	DECLARE @DateStr char(8)
	DECLARE @Begin datetime
	DECLARE @End datetime
	DECLARE @LastDay int
	DECLARE @Day int

	DECLARE @shiftId uniqueidentifier

	EXEC sp_GetCurrentShiftId @ShiftId = @shiftId OUTPUT

	SELECT @Now = GETDATE()
	SELECT @DateStr = LEFT(CONVERT(char(10), @Now, 111), 8)
	SELECT @Begin = CONVERT(datetime, LEFT(CONVERT(char(10), @Now, 111), 8) + ''01'', 111)
	SELECT @End = DATEADD(m, 1, @Begin)
	SELECT @LastDay = DAY(DATEADD(d, -1, @End))
	
	DECLARE @CurrentMonthRpt TABLE
	(
		Day int,
		Biogas float,
		EngeryProduction float
	);

	SELECT @Day = 1
	WHILE @Day <= @LastDay
	BEGIN
		SELECT @Begin = CONVERT(datetime, @DateStr + CONVERT(varchar(2), @Day), 111)
		SELECT @End = DATEADD(d, 1, @Begin)

		INSERT INTO @CurrentMonthRpt(Day, Biogas)
			SELECT @Day, ISNULL((SUM(ISNULL(det.SubTotalLast,0.0)) - SUM(ISNULL(det.SubTotalBegin,0.0))), 0.0) FROM ShiftStatDet det, ShiftStatMstr mstr
				WHERE mstr.ShiftId=det.ShiftId AND mstr.ShiftId=@shiftId AND mstr.Status=''A''
						AND (det.Item=''Biogas1'' OR det.Item=''Biogas2'')
						AND mstr.BeginTime>=@Begin AND mstr.BeginTime<@End

		UPDATE @CurrentMonthRpt SET EngeryProduction=
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0.0)) - SUM(ISNULL(det.SubTotalBegin,0.0))), 0.0) FROM ShiftStatDet det, ShiftStatMstr mstr
				WHERE mstr.ShiftId=det.ShiftId AND mstr.ShiftId=@shiftId AND mstr.Status=''A''
						AND (det.Item=''EnergyProduction1'' OR det.Item=''EnergyProduction2'')
						AND mstr.BeginTime>=@Begin AND mstr.BeginTime<@End)
			WHERE  Day=@Day

		SELECT @Day = @Day + 1
	END
	
	SELECT * FROM @CurrentMonthRpt

	SELECT SUM(Biogas) AS Biogas, SUM(EngeryProduction) AS EngeryProduction FROM @CurrentMonthRpt
END
' 
END
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentShiftId]    Script Date: 12/1/2016 3:34:35 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetCurrentShiftId]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'-- ================================================================
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

	Select Top 1 @shiftId = ShiftId From ShiftStatMstr Where @currTime >= BeginTime And @currTime < EndTime
	If (@@ROWCOUNT > 0)
	Begin
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

	Return 0
END
' 
END
GO
/****** Object:  Table [dbo].[GeneralParams]    Script Date: 12/1/2016 3:34:35 PM ******/
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
/****** Object:  Table [dbo].[ItemHistoryData]    Script Date: 12/1/2016 3:34:35 PM ******/
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
	[UpdateTime] [datetime] NOT NULL
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ItemLatestStatus]    Script Date: 12/1/2016 3:34:35 PM ******/
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
/****** Object:  Table [dbo].[MonitorItem]    Script Date: 12/1/2016 3:34:35 PM ******/
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
	[Address] [varchar](100) NOT NULL,
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
/****** Object:  Table [dbo].[MonthReportDet]    Script Date: 12/1/2016 3:34:35 PM ******/
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
	[Subtotal] [float] NOT NULL,
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
/****** Object:  Table [dbo].[MonthReportMstr]    Script Date: 12/1/2016 3:34:35 PM ******/
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
/****** Object:  Table [dbo].[MonthWorkerReportDet]    Script Date: 12/1/2016 3:34:35 PM ******/
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
	[Subtotal] [float] NOT NULL,
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
/****** Object:  Table [dbo].[Role]    Script Date: 12/1/2016 3:34:35 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[Role]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[Role](
	[RoleId] [uniqueidentifier] NOT NULL,
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
/****** Object:  Table [dbo].[ShiftStatDet]    Script Date: 12/1/2016 3:34:35 PM ******/
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
	[SubTotalBegin] [float] NOT NULL,
	[SubTotalLast] [float] NOT NULL,
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
/****** Object:  Table [dbo].[ShiftStatMstr]    Script Date: 12/1/2016 3:34:35 PM ******/
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
	[LastLoginId] [varchar](50) NULL,
	[LastLoginName] [varchar](50) NULL,
	[LastLoginTime] [datetime] NULL,
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
/****** Object:  Table [dbo].[User]    Script Date: 12/1/2016 3:34:35 PM ******/
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
	[Status] [char](1) NOT NULL,
 CONSTRAINT [PK_User] PRIMARY KEY CLUSTERED 
(
	[UserId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[UserInRole]    Script Date: 12/1/2016 3:34:35 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[UserInRole]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[UserInRole](
	[UserId] [uniqueidentifier] NOT NULL,
	[RoleId] [uniqueidentifier] NOT NULL,
 CONSTRAINT [PK_UseInRole] PRIMARY KEY CLUSTERED 
(
	[UserId] ASC,
	[RoleId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_ItemHistoryData]    Script Date: 12/1/2016 3:34:35 PM ******/
IF NOT EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N'[dbo].[ItemHistoryData]') AND name = N'IX_ItemHistoryData')
CREATE CLUSTERED INDEX [IX_ItemHistoryData] ON [dbo].[ItemHistoryData]
(
	[ItemId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'CreateMonthReportTime', N'30', 9, N'月报创建时间', N'TODO', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'EnableLog', N'true', 1, N'记录日志', N'true 或 false', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'FirstDayForMonthReportTime', N'1', 8, N'月报统计初始时间', N'每个月的几号，有效值1-31', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'KeepDbConnection', N'true', 2, N'保持数据库的连接', N'true 或 false', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'OPCServerProgID', N'KEPware.KEPServerEx.V4', 3, N'OPC Server ProgID', NULL, 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'PathToReportFile', N'D:\MonthlyReport', 0, N'月报文件存放路径', NULL, 0, 0, 0)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'QueryInterval', N'1000', 5, N'轮询间隔', N'向OPC Server轮询间隔，单位ms', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'RemoteMachine', N'', 4, N'OPC Server所在计算机名', N'如果OPC Server与DataSync服务在同一计算机，则设为空', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ReportFilenameFormat', N'MonthlyReport-{0}-{1:D2}.xlsx', 0, N'月报文件名格式', NULL, 0, 0, 0)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ReportFileTemplate', N'D:\MonthlyReport\Template\MonthlyReportTemplate.xlsx', 0, N'月报Excel模板的全路径', NULL, 0, 0, 0)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ShiftStartTime1', N'8:00:00', 6, N'工班1开始时间', N'输入时间串，例如 8:12:23', 0, 0, 1)
INSERT [dbo].[GeneralParams] ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) VALUES (N'System', N'ShiftStartTime2', N'20:00:00', 7, N'工班2开始时间', N'输入时间串，例如 8:12:23', 0, 0, 1)
INSERT [dbo].[ItemHistoryData] ([ShiftId], [ItemId], [Val], [UpdateTime]) VALUES (N'952f3f84-f9f1-4a04-aa91-df1cf221a8dc', N'Biogas1', N'20', CAST(0x0000A6C100FDA0E8 AS DateTime))
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'Biogas1', 459, CAST(0x0000A6CD0013E3CC AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'biogas2', 459, CAST(0x0000A6CD0013E3CC AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'BiogasSubtotal1', 23869, CAST(0x0000A6CF00C4002C AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'BiogasSubtotal2', 11935, CAST(0x0000A6CF00C4002C AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'EnergyProduction1', 11935, CAST(0x0000A6CF00C4002C AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'EnergyProduction2', 11935, CAST(0x0000A6CF00C4002C AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'GeneratorPower1', 11935, CAST(0x0000A6CF00C4002C AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'GeneratorPower2', 11935, CAST(0x0000A6CF00C4002C AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'Runtime1', 881, CAST(0x0000A6C5016B2AB4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'Runtime2', 875, CAST(0x0000A6C5016B2AB4 AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'TotalRuntime1', 23870, CAST(0x0000A6CF00C4002C AS DateTime), 192)
INSERT [dbo].[ItemLatestStatus] ([ItemId], [Val], [LastUpdate], [Quality]) VALUES (N'TotalRuntime2', 11935, CAST(0x0000A6CF00C4002C AS DateTime), 192)
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'Biogas1', N'Channel_4.Device_6.Short_1-', N'沼气1瞬时流量', 3, 0, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'biogas2', N'Channel_4.Device_6.Word_1-', N'沼气2瞬时流量', 3, 0, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'BiogasSubtotal1', N'Channel_4.Device_6.Short_1', N'沼气1累计流量', 3, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'BiogasSubtotal2', N'Channel_4.Device_6.Word_1', N'沼气2累计流量', 3, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'EnergyProduction1', N'Channel_4.Device_5.Short_1', N'1#发电机累计发电量', 3, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'EnergyProduction2', N'Channel_4.Device_5.Word_1', N'2#发电机累计发电量', 3, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'GeneratorPower1', N'Channel_3.Device_4.Word_1', N'1#发电机功率', 3, 0, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'GeneratorPower2', N'Channel_3.Device_4.Word_2', N'2#发电机功率', 3, 0, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'TotalRuntime1', N'Channel_4.Device_6.Word_2', N'1#发电机累计运行时间', 3, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[MonitorItem] ([ItemId], [Address], [DisplayName], [DataType], [NeedAccumulate], [UpdateHistory], [InConverter], [OutConverter], [Status]) VALUES (N'TotalRuntime2', N'Channel_4.Device_6.Word_3', N'2#发电机累计运行时间', 3, 1, 0, NULL, NULL, N'A')
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'b134b847-c72a-419a-b474-4b78e6eec043', N'BiogasSubtotal1', 0, 23869)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'b134b847-c72a-419a-b474-4b78e6eec043', N'BiogasSubtotal2', 1, 11935)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'b134b847-c72a-419a-b474-4b78e6eec043', N'EnergyProduction1', 1, 11935)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'b134b847-c72a-419a-b474-4b78e6eec043', N'EnergyProduction2', 1, 11935)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'b134b847-c72a-419a-b474-4b78e6eec043', N'TotalRuntime1', 2, 23870)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'b134b847-c72a-419a-b474-4b78e6eec043', N'TotalRuntime2', 1, 11935)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'BiogasSubtotal1', 1996, 2172)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'BiogasSubtotal2', 1996, 2172)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'EnergyProduction1', 1996, 2172)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'EnergyProduction2', 1996, 2172)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'TotalRuntime1', 1996, 2172)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', N'TotalRuntime2', 1996, 2172)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'dbc85c90-c478-40d8-82db-a868158c446b', N'BiogasSubtotal1', 0, 683)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'dbc85c90-c478-40d8-82db-a868158c446b', N'BiogasSubtotal2', 497, 683)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'dbc85c90-c478-40d8-82db-a868158c446b', N'EnergyProduction1', 2, 683)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'dbc85c90-c478-40d8-82db-a868158c446b', N'EnergyProduction2', 2, 683)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'dbc85c90-c478-40d8-82db-a868158c446b', N'TotalRuntime1', 2, 683)
INSERT [dbo].[ShiftStatDet] ([ShiftId], [Item], [SubTotalBegin], [SubTotalLast]) VALUES (N'dbc85c90-c478-40d8-82db-a868158c446b', N'TotalRuntime2', 2, 683)
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [LastLoginId], [LastLoginName], [LastLoginTime], [Status]) VALUES (N'b134b847-c72a-419a-b474-4b78e6eec043', CAST(0x0000A6CF0083D600 AS DateTime), CAST(0x0000A6CF00A84A48 AS DateTime), CAST(0x0000A6CF00C4002C AS DateTime), CAST(0x0000A6CF01499700 AS DateTime), N'id1', N'name1', CAST(0x0000A6CF00A898AB AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [LastLoginId], [LastLoginName], [LastLoginTime], [Status]) VALUES (N'd2e0507f-5cc9-48d3-b2a5-93b940ae33e5', CAST(0x0000A6CD01499700 AS DateTime), CAST(0x0000A6CD0188B80C AS DateTime), CAST(0x0000A6CD018A6410 AS DateTime), CAST(0x0000A6CE0083D600 AS DateTime), N'id1', N'name1', CAST(0x0000A6CD0188BA01 AS DateTime), N'A')
INSERT [dbo].[ShiftStatMstr] ([ShiftId], [BeginTime], [ActualBeginTime], [LastUpdateTime], [EndTime], [LastLoginId], [LastLoginName], [LastLoginTime], [Status]) VALUES (N'dbc85c90-c478-40d8-82db-a868158c446b', CAST(0x0000A6CC01499700 AS DateTime), CAST(0x0000A6CC01824B99 AS DateTime), CAST(0x0000A6CD0014670C AS DateTime), CAST(0x0000A6CD0083D600 AS DateTime), N'id1', N'name1', CAST(0x0000A6CC01824FA4 AS DateTime), N'A')
INSERT [dbo].[User] ([UserId], [LoginId], [Name], [Password], [IDCard], [Status]) VALUES (N'ae0e7169-40b3-4ee8-b8a8-709cc715f575', N'id1', N'name1', N'id1', N'', N'A')
INSERT [dbo].[User] ([UserId], [LoginId], [Name], [Password], [IDCard], [Status]) VALUES (N'b9744510-4b7b-4c2b-9fb3-aaaf1391f073', N'id2', N'name2', N'id2', N'', N'A')
INSERT [dbo].[User] ([UserId], [LoginId], [Name], [Password], [IDCard], [Status]) VALUES (N'e4e57bed-f3d9-4b11-bf43-da02863a3f1e', N'id3', N'name3', N'id3', N'', N'A')
INSERT [dbo].[User] ([UserId], [LoginId], [Name], [Password], [IDCard], [Status]) VALUES (N'61c04148-4008-42ae-9f1e-e781e620ebb1', N'id4', N'name4', N'id4', N'0027552770', N'A')
INSERT [dbo].[User] ([UserId], [LoginId], [Name], [Password], [IDCard], [Status]) VALUES (N'62c04148-4008-42ae-9f1e-e781e620ebb1', N'id5', N'name5', N'id5', N'', N'A')
/****** Object:  Index [PK_ItemHistoryData]    Script Date: 12/1/2016 3:34:35 PM ******/
IF NOT EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N'[dbo].[ItemHistoryData]') AND name = N'PK_ItemHistoryData')
ALTER TABLE [dbo].[ItemHistoryData] ADD  CONSTRAINT [PK_ItemHistoryData] PRIMARY KEY NONCLUSTERED 
(
	[ShiftId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
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
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[DF_User_Status]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[User] ADD  CONSTRAINT [DF_User_Status]  DEFAULT ('A') FOR [Status]
END

GO
IF NOT EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthReportDet_MonthReportMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthReportDet]'))
ALTER TABLE [dbo].[MonthReportDet]  WITH CHECK ADD  CONSTRAINT [FK_MonthReportDet_MonthReportMstr] FOREIGN KEY([ReportId])
REFERENCES [dbo].[MonthReportMstr] ([ReportId])
GO
IF  EXISTS (SELECT * FROM sys.foreign_keys WHERE object_id = OBJECT_ID(N'[dbo].[FK_MonthReportDet_MonthReportMstr]') AND parent_object_id = OBJECT_ID(N'[dbo].[MonthReportDet]'))
ALTER TABLE [dbo].[MonthReportDet] CHECK CONSTRAINT [FK_MonthReportDet_MonthReportMstr]
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
/****** Object:  Trigger [dbo].[tr_UpdateItemSubtotal]    Script Date: 12/1/2016 3:34:35 PM ******/
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
	Declare @val float
	Declare @needAccumulate bit
	Declare @newTime datetime
    	
	Select @item = ItemID, @val = Val, @newTime = LastUpdate From inserted
	If @@ROWCOUNT <> 1 Return
	
	Select @needAccumulate = IsNull(NeedAccumulate,0) From MonitorItem Where ItemID = @item
	If @needAccumulate = 0 Return

	EXEC sp_GetCurrentShiftId @ShiftId = @shiftId OUTPUT

	Update ShiftStatMstr Set LastUpdateTime = @newTime Where ShiftId = @shiftId

	Update ShiftStatDet Set SubTotalLast = @val Where ShiftId = @shiftId And Item = @item
	If @@ROWCOUNT = 0	-- Record not exist
	Begin
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shiftId, @item, @val, @val)
	End
END

' 
GO
