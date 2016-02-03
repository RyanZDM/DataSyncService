if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[sp_CreateReport]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[sp_CreateReport]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[GeneralParams]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[GeneralParams]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ItemHisDet]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ItemHisDet]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ItemHisMstr]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ItemHisMstr]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ItemLatestStatus]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ItemLatestStatus]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[MonitorItem]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[MonitorItem]
GO

CREATE TABLE [dbo].[GeneralParams] (
	[Category] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [nvarchar] (200) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DispOrder] [tinyint] NOT NULL ,
	[DispName] [nvarchar] (100) COLLATE Chinese_PRC_CI_AS NULL ,
	[Memo] [nvarchar] (100) COLLATE Chinese_PRC_CI_AS NULL ,
	[Hide] [bit] NOT NULL ,
	[IsEncrypted] [bit] NOT NULL ,
	[IsPrimary] [bit] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ItemHisDet] (
	[ReportID] [uniqueidentifier] NOT NULL ,
	[ItemID] [varchar] (100) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Val] [varchar] (1000) COLLATE Chinese_PRC_CI_AS NULL ,
	[LastUpdate] [datetime] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ItemHisMstr] (
	[ReportID] [uniqueidentifier] NOT NULL ,
	[CreateDate] [datetime] NOT NULL ,
	[Memo] [nvarchar] (100) COLLATE Chinese_PRC_CI_AS NULL ,
	[Status] [char] (1) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ItemLatestStatus] (
	[ItemID] [varchar] (100) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Val] [varchar] (1000) COLLATE Chinese_PRC_CI_AS NULL ,
	[LastUpdate] [datetime] NOT NULL,
	[Quality] [int]
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[MonitorItem] (
	[ItemID] [varchar] (100) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DataType] [int] NOT NULL ,
	[Status] [char] (1) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[GeneralParams] WITH NOCHECK ADD 
	CONSTRAINT [PK_GENERALPARAMS] PRIMARY KEY  CLUSTERED 
	(
		[Category],
		[Name]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[ItemHisDet] WITH NOCHECK ADD 
	CONSTRAINT [PK_ItemHisDet] PRIMARY KEY  CLUSTERED 
	(
		[ReportID],
		[ItemID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[ItemHisMstr] WITH NOCHECK ADD 
	CONSTRAINT [PK_ItemHisMstr] PRIMARY KEY  CLUSTERED 
	(
		[ReportID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[ItemLatestStatus] WITH NOCHECK ADD 
	CONSTRAINT [PK_MonitorItem] PRIMARY KEY  CLUSTERED 
	(
		[ItemID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[MonitorItem] WITH NOCHECK ADD 
	CONSTRAINT [PK_MonitoredItem] PRIMARY KEY  CLUSTERED 
	(
		[ItemID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[GeneralParams] WITH NOCHECK ADD 
	CONSTRAINT [DF__GeneralPa__DispO__375B2DB9] DEFAULT (0) FOR [DispOrder],
	CONSTRAINT [DF__GeneralPar__Hide__384F51F2] DEFAULT (0) FOR [Hide],
	CONSTRAINT [DF__GeneralPa__IsEnc__3943762B] DEFAULT (0) FOR [IsEncrypted],
	CONSTRAINT [DF__GeneralPa__IsPri__3A379A64] DEFAULT (0) FOR [IsPrimary]
GO

ALTER TABLE [dbo].[ItemHisMstr] WITH NOCHECK ADD 
	CONSTRAINT [DF_ItemHisMstr_CreateDate] DEFAULT (getdate()) FOR [CreateDate],
	CONSTRAINT [DF_ItemHisMstr_Status] DEFAULT ('A') FOR [Status]
GO

ALTER TABLE [dbo].[ItemLatestStatus] WITH NOCHECK ADD 
	CONSTRAINT [DF_MonitorItem_LastUpdate] DEFAULT (getdate()) FOR [LastUpdate]
GO

ALTER TABLE [dbo].[MonitorItem] WITH NOCHECK ADD 
	CONSTRAINT [DF_MonitoredItem_DataType] DEFAULT (3) FOR [DataType],
	CONSTRAINT [DF_MonitoredItem_Status] DEFAULT ('A') FOR [Status]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE PROCEDURE sp_CreateReport 
AS
DECLARE @ReportID UNIQUEIDENTIFIER
DECLARE @CreateDate datetime

SELECT @ReportID = NewID()
SELECT @CreateDate = GetDate()

INSERT INTO ItemHisMstr (ReportID, CreateDate, Status) VALUES (@ReportID, @CreateDate, 'A')
IF @@ERROR = 0
	INSERT INTO ItemHisDet (ReportID, ItemID, Val, LastUpdate)
		SELECT @ReportID, ItemID, Val, LastUpdate FROM ItemLatestStatus


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

-- Script generated on 2008-12-27 2:07
-- By: CORP\10055164
-- Server: (local)

BEGIN TRANSACTION            
  DECLARE @JobID BINARY(16)  
  DECLARE @ReturnCode INT    
  SELECT @ReturnCode = 0     
IF (SELECT COUNT(*) FROM msdb.dbo.syscategories WHERE name = N'[Uncategorized (Local)]') < 1 
  EXECUTE msdb.dbo.sp_add_category @name = N'[Uncategorized (Local)]'

  -- Delete the job with the same name (if it exists)
  SELECT @JobID = job_id     
  FROM   msdb.dbo.sysjobs    
  WHERE (name = N'CreatReport')       
  IF (@JobID IS NOT NULL)    
  BEGIN  
  -- Check if the job is a multi-server job  
  IF (EXISTS (SELECT  * 
              FROM    msdb.dbo.sysjobservers 
              WHERE   (job_id = @JobID) AND (server_id <> 0))) 
  BEGIN 
    -- There is, so abort the script 
    RAISERROR (N'Unable to import job ''CreatReport'' since there is already a multi-server job with this name.', 16, 1) 
    GOTO QuitWithRollback  
  END 
  ELSE 
    -- Delete the [local] job 
    EXECUTE msdb.dbo.sp_delete_job @job_name = N'CreatReport' 
    SELECT @JobID = NULL
  END 

BEGIN 

  -- Add the job
  EXECUTE @ReturnCode = msdb.dbo.sp_add_job @job_id = @JobID OUTPUT , @job_name = N'CreatReport', @owner_login_name = N'sa', @description = N'No description available.', @category_name = N'[Uncategorized (Local)]', @enabled = 1, @notify_level_email = 0, @notify_level_page = 0, @notify_level_netsend = 0, @notify_level_eventlog = 2, @delete_level= 0
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  -- Add the job steps
  EXECUTE @ReturnCode = msdb.dbo.sp_add_jobstep @job_id = @JobID, @step_id = 1, @step_name = N'Exec SP', @command = N'Exec sp_CreateReport', @database_name = N'OPCTest', @server = N'', @database_user_name = N'', @subsystem = N'TSQL', @cmdexec_success_code = 0, @flags = 0, @retry_attempts = 0, @retry_interval = 1, @output_file_name = N'', @on_success_step_id = 0, @on_success_action = 1, @on_fail_step_id = 0, @on_fail_action = 2
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 
  EXECUTE @ReturnCode = msdb.dbo.sp_update_job @job_id = @JobID, @start_step_id = 1 

  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  -- Add the job schedules
  EXECUTE @ReturnCode = msdb.dbo.sp_add_jobschedule @job_id = @JobID, @name = N'Execute every one hour', @enabled = 1, @freq_type = 4, @active_start_date = 20081227, @active_start_time = 0, @freq_interval = 1, @freq_subday_type = 8, @freq_subday_interval = 1, @freq_relative_interval = 0, @freq_recurrence_factor = 0, @active_end_date = 99991231, @active_end_time = 235959
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  -- Add the Target Servers
  EXECUTE @ReturnCode = msdb.dbo.sp_add_jobserver @job_id = @JobID, @server_name = N'(local)' 
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

END
COMMIT TRANSACTION          
GOTO   EndSave              
QuitWithRollback:
  IF (@@TRANCOUNT > 0) ROLLBACK TRANSACTION 
EndSave: 



INSERT INTO GeneralParams (Category, Name, Value) Values ('System', 'RemoteMachine', ' ')
INSERT INTO GeneralParams (Category, Name, Value) Values ('System', 'OPCServerProgID', ' ')
INSERT INTO GeneralParams (Category, Name, Value) Values ('System', 'QueryInterval', '1000')
INSERT INTO GeneralParams (Category, Name, Value) Values ('System', 'EnableLog', 'true')