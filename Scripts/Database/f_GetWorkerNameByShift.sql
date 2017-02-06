USE [OPC-Site]
GO

/****** Object:  UserDefinedFunction [dbo].[GetWorkerNameByShift]    Script Date: 2/6/2017 3:57:46 PM ******/
DROP FUNCTION [dbo].[GetWorkerNameByShift]
GO

/****** Object:  UserDefinedFunction [dbo].[GetWorkerNameByShift]    Script Date: 2/6/2017 3:57:46 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

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
	SELECT @workerList = ''

	IF @shiftId IS NULL
		RETURN ''

	DECLARE cursorWorker CURSOR FOR
		SELECT LoginName FROM WorkersInShift WHERE ShiftId=@shiftId 

	OPEN cursorWorker
	FETCH NEXT FROM cursorWorker INTO @worker
	WHILE @@FETCH_STATUS = 0
	BEGIN
		SELECT @workerList = @workerList + @worker + ','
		FETCH NEXT FROM cursorWorker INTO @worker
	END

	CLOSE cursorWorker;  
	DEALLOCATE cursorWorker;  

	IF LEN(@workerList) > 0
		SELECT @workerList = LEFT(@workerList, LEN(@workerList) - 1)

	RETURN @workerList

END

GO


