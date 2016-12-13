USE [OPC]
GO

/****** Object:  Trigger [tr_UpdateItemSubtotal]    Script Date: 12/13/2016 9:26:58 PM ******/
DROP TRIGGER [dbo].[tr_UpdateItemSubtotal]
GO

/****** Object:  Trigger [dbo].[tr_UpdateItemSubtotal]    Script Date: 12/13/2016 9:26:58 PM ******/
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
	Declare @val float			
	Declare @valHight float, @valLow float
	Declare @realValue float	-- combination of xxx.H and xxx.L
	Declare @realItem varchar(100)
	Declare @needAccumulate bit
	Declare @newTime datetime
	Declare @suffix char(2), @anotherSuffix char(2)
	Declare @anotherItem varchar(100)
	Declare @anotherValue float
    	
	Select @item = ItemID, @suffix = Upper(Right(ItemID,2)), @val = Val, @newTime = LastUpdate From inserted
	If @item Is NULL Return
	
	Select @needAccumulate = IsNull(NeedAccumulate,0) From MonitorItem Where ItemID = @item
	If @needAccumulate = 0 Return

	If ((@suffix = '.H') Or (@suffix = '.L'))
		-- Need to combine the value of .H and .L to one single value. Assume the value of .H is a integer
		-- For example: xxx.H = 12, xxx.L = 345.67, then the real value is 12345.67
		Begin
			Select @realItem = Left(@item, Len(@item) - 2)
			If (@suffix = '.H')
				Begin
					Select @anotherSuffix = '.L'
					Select @anotherItem = @realItem + @anotherSuffix		-- xxx.L
					Select @anotherValue = Val From ItemLatestStatus Where ItemId = @anotherItem
					If (@@ROWCOUNT = 1)
						Select @realValue = CAST((CAST(Floor(@val) As varchar(100)) + CAST(@anotherValue as varchar(100))) As float)
					Else	-- Not found the value of xxx.L
						Select @realValue = @val
				End
			Else
				Begin
					Select @anotherSuffix = '.H'
					Select @anotherItem = @realItem + @anotherSuffix		-- xxx.H
					Select @anotherValue = Floor(Val) From ItemLatestStatus Where ItemId = @anotherItem
					If (@@ROWCOUNT = 1)
						Select @realValue = CAST((CAST(@anotherValue As varchar(100)) + CAST(@val as varchar(100))) As float)	
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
	
	Update ShiftStatDet Set SubTotalLast = @realValue Where ShiftId = @shiftId And Item = @realItem
	If @@ROWCOUNT = 0	-- Record not exist
	Begin
		Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shiftId, @realItem, @realValue, @realValue)
	End
END


GO

