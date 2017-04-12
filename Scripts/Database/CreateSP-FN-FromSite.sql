/****** Object:  UserDefinedFunction [dbo].[GetWorkerNameByShift]    Script Date: 4/12/2017 7:10:46 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[GetWorkerNameByShift]') AND type in (N'FN', N'IF', N'TF', N'FS', N'FT'))
DROP FUNCTION [dbo].[GetWorkerNameByShift]
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentShiftId]    Script Date: 4/12/2017 7:10:46 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetCurrentShiftId]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_GetCurrentShiftId]
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 4/12/2017 7:10:46 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetCurrentMonthDataByDay]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_GetCurrentMonthDataByDay]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyWorkerReport]    Script Date: 4/12/2017 7:10:46 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyWorkerReport]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_CreateMonthlyWorkerReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyShiftReport]    Script Date: 4/12/2017 7:10:46 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyShiftReport]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_CreateMonthlyShiftReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyReport]    Script Date: 4/12/2017 7:10:46 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_CreateMonthlyReport]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_CreateMonthlyReport]
GO
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyReport]    Script Date: 4/12/2017 7:10:46 PM ******/
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
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyShiftReport]    Script Date: 4/12/2017 7:10:46 PM ******/
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
/****** Object:  StoredProcedure [dbo].[sp_CreateMonthlyWorkerReport]    Script Date: 4/12/2017 7:10:46 PM ******/
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
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 4/12/2017 7:10:46 PM ******/
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
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentShiftId]    Script Date: 4/12/2017 7:10:46 PM ******/
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
/****** Object:  UserDefinedFunction [dbo].[GetWorkerNameByShift]    Script Date: 4/12/2017 7:10:46 PM ******/
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
