
/****** Object:  StoredProcedure [dbo].[sp_GetCurrentShiftId]    Script Date: 4/12/2017 9:37:07 AM ******/
DROP PROCEDURE [dbo].[sp_GetCurrentShiftId]
GO

/****** Object:  StoredProcedure [dbo].[sp_GetCurrentShiftId]    Script Date: 4/12/2017 9:37:07 AM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO



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
		Select Top 1 @shiftId = ShiftId From ShiftStatMstr With(HoldLock, RowLock) Where Status='A' And @currTime >= BeginTime And @currTime < EndTime
		If (@@ROWCOUNT > 0)
		Begin
			Rollback Tran shiftMstr
			return 0
		End

		-- Need to create new shift
		Select @shiftId = NEWID()
		Select @shift1StartTime = RTrim(LTrim(Value)) From GeneralParams Where Category = 'System' And Name = 'ShiftStartTime1'
		Select @shift2StartTime = RTrim(LTrim(Value)) From GeneralParams Where Category = 'System' And Name = 'ShiftStartTime2'
		If ((@shift1StartTime Is Null) Or (@shift1StartTime = ''))
		Begin
			Select @shift1StartTime = '8:00:00'		-- Default time
		End

		If ((@shift2StartTime Is Null) Or (@shift2StartTime = ''))
		Begin
			Select @shift2StartTime = '20:00:00'		-- Default time
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

		Insert Into ShiftStatMstr (ShiftId, BeginTime,ActualBeginTime, EndTime,Status) Values (@shiftId, @beginTime, GETDATE(), @endTime, 'A')
	Commit Tran shiftMstr

	Return 0
END



GO


