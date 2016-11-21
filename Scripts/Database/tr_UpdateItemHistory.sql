USE [OPC]
GO

/****** Object:  Trigger [tr_UpdateItemHistory]    Script Date: 11/21/2016 3:20:11 PM ******/
DROP TRIGGER [dbo].[tr_UpdateItemHistory]
GO

/****** Object:  Trigger [dbo].[tr_UpdateItemHistory]    Script Date: 11/21/2016 3:20:11 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO


-- =============================================
-- Author:		ZDM
-- Create date: 2016-10-04
-- Description:	Add item value into history table
-- =============================================
CREATE TRIGGER [dbo].[tr_UpdateItemHistory]
   ON  [dbo].[ItemLatestStatus]
   AFTER INSERT,UPDATE
AS 
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	Declare @shiftId uniqueidentifier
	Declare @reportId uniqueidentifier
	Declare @item varchar(100)
	Declare @val float
	Declare @updateHistory bit
	Declare @updateTime datetime
	Declare @lastUpdate datetime
    
	Select @item = ItemID, @val = Val, @updateTime  = LastUpdate From inserted
	If @@ROWCOUNT <> 1 Return
	
	Select @updateHistory = IsNull(UpdateHistory,0) From MonitorItem Where ItemID = @item
	If @UpdateHistory = 0 Return
	
	Select @lastUpdate = LastUpdate From deleted Where ItemId = @item
	If (DATEDIFF(ss, @lastUpdate, @updateTime) <= 0) Return

	EXEC sp_GetCurrentShiftId @ShiftId = @shiftId OUTPUT

	Insert Into ItemHistoryData(ShiftId, ItemId, Val, UpdateTime) Values (@shiftId, @item, @val, @updateTime)
END


GO


