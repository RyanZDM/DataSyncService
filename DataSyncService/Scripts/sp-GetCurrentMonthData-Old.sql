USE [OPC]
GO

/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 10/18/2016 10:19:52 PM ******/
DROP PROCEDURE [dbo].[sp_GetCurrentMonthDataByDay]
GO

/****** Object:  StoredProcedure [dbo].[sp_GetCurrentMonthDataByDay]    Script Date: 10/18/2016 10:19:52 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

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

	DECLARE @Now datetime
	DECLARE @DateStr char(8)
	DECLARE @Begin datetime
	DECLARE @End datetime
	DECLARE @LastDay int
	DECLARE @Day int

	DECLARE @Id uniqueidentifier
	DECLARE @ShiftId nvarchar(20)

	SELECT @Id = Id, @ShiftId = ShiftId FROM ProductionStatMstr WHERE IsCurrent=1
	IF @@ROWCOUNT <> 1 RETURN

	SELECT @Now = GETDATE()
	SELECT @DateStr = LEFT(CONVERT(char(10), @Now, 111), 8)
	SELECT @Begin = CONVERT(datetime, LEFT(CONVERT(char(10), @Now, 111), 8) + '01', 111)
	SELECT @End = DATEADD(m, 1, @Begin)
	SELECT @LastDay = DAY(DATEADD(d, -1, @End))
	
	DECLARE @CurrentMonthRpt TABLE
	(
		Day int,
		Item varchar(100),
		Subtotal float
	);

	SELECT @Day = 1
	WHILE @Day <= @LastDay
	BEGIN
		SELECT @Begin = CONVERT(datetime, @DateStr + CONVERT(varchar(2), @Day), 111)
		SELECT @End = DATEADD(d, 1, @Begin)

		INSERT INTO @CurrentMonthRpt(Day, Item, Subtotal)
			SELECT @Day, ItemId, ISNULL(prod.SubTotal,0) FROM MonitorItem itm LEFT JOIN 
				(SELECT Item, SUM(SubTotal) AS SubTotal FROM ProductionStatDet, ProductionStatMstr 
					WHERE ProductionId=Id AND Status='A' AND ShiftId=@ShiftId
							AND BeginTime>=@Begin AND BeginTime<@End
					GROUP BY Item) AS prod
				 ON itm.[Address]=prod.Item
			WHERE itm.Status='A'

		SELECT @Day = @Day + 1
	END
	
	SELECT * FROM @CurrentMonthRpt
END

GO


