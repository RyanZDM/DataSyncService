/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 4/13/2017 4:32:28 PM ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[sp_GetCurrentMonthDataByDay]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[sp_GetCurrentMonthDataByDay]
GO
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 4/13/2017 4:32:28 PM ******/
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
