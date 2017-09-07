/****** Object:  Trigger [tr_UpdateItemSubtotal]    Script Date: 9/6/2017 4:59:04 PM ******/
DROP TRIGGER [dbo].[tr_UpdateItemSubtotal]
GO

/****** Object:  Trigger [dbo].[tr_UpdateItemSubtotal]    Script Date: 9/6/2017 4:59:04 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO


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
	Declare @dateType int
    	
	Select @item=it.ItemID, @suffix=Upper(Right(it.ItemID,2)), @val=Cast(Val As int), @newTime=LastUpdate, @dateType=DataType From inserted it, MonitorItem mi
	If @item Is NULL Return

	-- Ignore the .H value, will count it in while the .L value changed
	If (@suffix = '.H') Return

	-- If the data type is not boolean, should not have the value less than 0
	If (@dateType <> 11) And @val <= 0 Return

	-- Check flag to see if need to accumulate the number in table ShitStatDet
	Select @needAccumulate = IsNull(NeedAccumulate,0) From MonitorItem Where ItemID = @item
	If @needAccumulate = 0 Return
		
	If ((@suffix = '.H') Or (@suffix = '.L'))
		-- Need to combine the value of .H and .L to one single value. Assume the value of .H is a integer
		-- For example: xxx.H = 12, xxx.L = 345, then the real value is 12345
		
		-- TODO Need to check if always calculate the CORRECT value!
		Begin
			Select @realItem = Left(@item, Len(@item) - 2)
			If (@suffix = '.H')
				Begin
					Select @anotherSuffix = '.L'
					Select @anotherItem = @realItem + @anotherSuffix		-- xxx.L
					Select @anotherValue = Cast(Val As int) From ItemLatestStatus Where ItemId = @anotherItem
					If (@@ROWCOUNT = 1)
						Select @realValue = CAST((CAST(@val As varchar(20)) + CAST(@anotherValue as varchar(20))) As int)
					Else	-- Not found the value of xxx.L
						Select @realValue = 0
				End
			Else		-- Suffix = '.L'
				Begin
					Select @anotherSuffix = '.H'
					Select @anotherItem = @realItem + @anotherSuffix		-- xxx.H
					Select @anotherValue = Cast(Val As int) From ItemLatestStatus Where ItemId = @anotherItem
					If (@@ROWCOUNT = 1)
						Select @realValue = CAST((CAST(@anotherValue As varchar(20)) + CAST(@val as varchar(20))) As int)	
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

	-- Log if the @realValue is negative
	If @realValue < 0
		Begin
			-- 5001 means that the value is negative, skip it
			Insert Into AbnormalChange (ShiftId, ItemName, PreValue, NewValue, UpdateTime, CreateTime) Values (@shiftId, @realItem, -5001, @realValue, @newTime, GetDate())
			Return
		End

	Update ShiftStatMstr Set LastUpdateTime = @newTime Where ShiftId = @shiftId
	
	Declare @subtotalBegin int
	Declare @newSubtotalBegin int
	Declare @subtotalLast int
	
	Select @subtotalLast = SubTotalLast, @subtotalBegin = SubTotalBegin From ShiftStatDet Where ShiftId = @shiftId and Item = @realItem
	If @@ROWCOUNT = 0	-- Record not exist, just insert
		Begin
			Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shiftId, @realItem, @realValue, @realValue)

			-- Record the abnormal change
			Insert Into AbnormalChange (ShiftId, ItemName, PreValue, NewValue, UpdateTime, CreateTime) Values (@shiftId, @realItem, -1, @realValue, @newTime, GetDate())

			Return
		End

	If (@realValue > @subtotalLast)	-- The value change is reasonable
		Begin
			Update ShiftStatDet Set SubTotalLast = @realValue Where ShiftId = @shiftId And Item = @realItem
			Return
		End

	If (@realValue < @subtotalLast)	-- The subtotal value must be reset, need to re-calculate the SubtotalBegin
		-- TDOO Seems sometimes there are one or two invalide value which should be ignored, the question is that how to recognize it
		Begin
			Select @newSubtotalBegin = @subtotalBegin - @subtotalLast	-- It should be a negative value
			Update ShiftStatDet Set SubTotalBegin = @newSubtotalBegin, SubTotalLast = @realValue Where ShiftId = @shiftId And Item = @realItem

			-- Record the abnormal change
			Insert Into AbnormalChange (ShiftId, ItemName, PreValue, NewValue, UpdateTime, CreateTime) Values (@shiftId, @realItem, @subtotalBegin, @newSubtotalBegin, @newTime, GetDate())
			Insert Into AbnormalChange (ShiftId, ItemName, PreValue, NewValue, UpdateTime, CreateTime) Values (@shiftId, @realItem, @subtotalLast, @realValue, @newTime, GetDate())
		End	
END

GO


