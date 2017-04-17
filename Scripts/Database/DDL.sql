                                                                                                                                                                                                                                                                
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Create Table [GeneralParams]
(
[Category] varchar(50)  NOT NULL ,
[Name] varchar(50)  NOT NULL ,
[Value] nvarchar(200)  NOT NULL ,
[DispOrder] tinyint  NOT NULL DEFAULT ((0)),
[DispName] nvarchar(100)  NULL ,
[Memo] nvarchar(100)  NULL ,
[Hide] bit  NOT NU
Create Table [ItemHistoryData]
(
[ShiftId] uniqueidentifier  NOT NULL DEFAULT (newid()),
[ItemId] varchar(100)  NOT NULL ,
[Val] varchar(1000)  NULL ,
[UpdateTime] datetime  NOT NULL ,
)
ALTER TABLE ItemHistoryData ADD CONSTRAINT PK_ItemHistoryData PRIMARY
Create Table [ItemLatestStatus]
(
[ItemId] varchar(100)  NOT NULL ,
[Val] float  NOT NULL DEFAULT ((0)),
[LastUpdate] datetime  NOT NULL DEFAULT (getdate()),
[Quality] int  NULL ,
)
ALTER TABLE ItemLatestStatus ADD CONSTRAINT PK_MonitorItem PRIMARY KEY  ([
Create Table [MonitorItem]
(
[ItemId] varchar(100)  NOT NULL ,
[Address] nvarchar(100)  NOT NULL ,
[DisplayName] nvarchar(100)  NULL ,
[DataType] int  NOT NULL ,
[NeedAccumulate] bit  NOT NULL DEFAULT ((0)),
[UpdateHistory] bit  NOT NULL DEFAULT ((0)),
[In
Create Table [MonthReportDet]
(
[ReportId] uniqueidentifier  NOT NULL ,
[ShiftId] uniqueidentifier  NOT NULL ,
[Item] varchar(100)  NOT NULL ,
[Subtotal] int  NOT NULL DEFAULT ((0)),
[Status] char(1)  NOT NULL DEFAULT ('A'),
)
ALTER TABLE MonthReportDet AD
Create Table [MonthReportMstr]
(
[ReportId] uniqueidentifier  NOT NULL ,
[YearMonth] char(6)  NOT NULL ,
[BeginTime] datetime  NOT NULL ,
[EndTime] datetime  NOT NULL ,
[CreateTime] datetime  NOT NULL DEFAULT (getdate()),
[IsFileCreated] bit  NOT NULL DEFA
Create Table [MonthReportShiftDet]
(
[ReportId] uniqueidentifier  NOT NULL ,
[ShiftId] uniqueidentifier  NOT NULL ,
[BeginTime] datetime  NOT NULL ,
[ActualBeginTime] datetime  NOT NULL ,
[LastUpdateTime] datetime  NOT NULL ,
[EndTime] datetime  NULL ,
)
A
Create Table [MonthWorkerReportDet]
(
[ReportId] uniqueidentifier  NOT NULL ,
[WorkerId] varchar(50)  NOT NULL ,
[WorkerName] nvarchar(50)  NOT NULL ,
[Item] varchar(100)  NOT NULL ,
[Subtotal] int  NOT NULL DEFAULT ((0)),
[Status] char(1)  NOT NULL DEFAUL
Create Table [Role]
(
[RoleId] varchar(50)  NOT NULL DEFAULT (newid()),
[Name] nvarchar(50)  NOT NULL ,
[Status] char(1)  NOT NULL DEFAULT ('A'),
)
ALTER TABLE Role ADD CONSTRAINT PK_Role PRIMARY KEY  ([RoleId])                                             
Create Table [ShiftStatDet]
(
[ShiftId] uniqueidentifier  NOT NULL ,
[Item] varchar(100)  NOT NULL ,
[SubTotalBegin] int  NOT NULL ,
[SubTotalLast] int  NOT NULL ,
)
ALTER TABLE ShiftStatDet ADD CONSTRAINT PK_ShiftStatDet PRIMARY KEY  ([ShiftId], [Item])  
Create Table [ShiftStatMstr]
(
[ShiftId] uniqueidentifier  NOT NULL ,
[BeginTime] datetime  NOT NULL ,
[ActualBeginTime] datetime  NULL ,
[LastUpdateTime] datetime  NULL ,
[EndTime] datetime  NULL ,
[Status] char(1)  NOT NULL DEFAULT ('A'),
)
ALTER TABLE S
Create Table [sysdiagrams]
(
[name] nvarchar(128)  NOT NULL ,
[principal_id] int  NOT NULL ,
[diagram_id] int IDENTITY(1,1) NOT NULL ,
[version] int  NULL ,
[definition] varbinary(MAX)  NULL ,
)
ALTER TABLE sysdiagrams ADD CONSTRAINT PK__sysdiagr__C2B05B61
Create Table [User]
(
[UserId] uniqueidentifier  NOT NULL DEFAULT (newid()),
[LoginId] nvarchar(50)  NOT NULL ,
[Name] nvarchar(50)  NOT NULL ,
[Password] nvarchar(50)  NULL ,
[IDCard] varchar(100)  NULL ,
[IsProtected] bit  NOT NULL DEFAULT ((0)),
[Status
Create Table [UserInRole]
(
[UserId] uniqueidentifier  NOT NULL ,
[RoleId] varchar(50)  NOT NULL ,
)
ALTER TABLE UserInRole ADD CONSTRAINT PK_UseInRole PRIMARY KEY  ([UserId], [RoleId])                                                                       
Create Table [WorkersInShift]
(
[ShiftId] uniqueidentifier  NOT NULL ,
[LoginId] varchar(50)  NOT NULL ,
[LoginName] nvarchar(50)  NOT NULL ,
[LoginTime] datetime  NOT NULL DEFAULT (getdate()),
)
ALTER TABLE WorkersInShift ADD CONSTRAINT PK_WorkersInShift 

(15 rows affected)
Text                                                                                                                                                                                                                                                           
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
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
                                                                                                                                                                                                                                                      
			Commit Tran shiftMstr
                                                                                                                                                                                                                                     
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
                                                                                                                                                                                                                                                          

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
Text                                                                                                                                                                                                                                                           
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
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
                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
	Insert Into MonthReportShiftDet (ReportId, ShiftId, BeginTime, ActualBeginTime, LastUpdateTime, EndTime)
                                                                                                                                                    
		Select @ReportId, ShiftId, BeginTime, ActualBeginTime, LastUpdateTime, EndTime From ShiftStatMstr
                                                                                                                                                          
			Where Status = 'A' And BeginTime >= @BeginTime And EndTime < @EndTime
                                                                                                                                                                                     
		
                                                                                                                                                                                                                                                           
	Insert Into MonthReportDet (ReportId, ShiftId, Item, Subtotal, Status)
                                                                                                                                                                                      
		Select @ReportId, mstr.ShiftId, det.Item, (Sum(IsNull(det.SubTotalLast,0)) - Sum(IsNull(det.SubTotalBegin,0))) As Subtotal, 'A'
                                                                                                                            
		From ShiftStatMstr mstr, ShiftStatDet det
                                                                                                                                                                                                                  
		Where mstr.ShiftId = det.ShiftId And mstr.Status = 'A'
                                                                                                                                                                                                     
				And mstr.BeginTime >= @BeginTime And mstr.EndTime < @EndTime
                                                                                                                                                                                             
		Group By mstr.ShiftId, det.Item
                                                                                                                                                                                                                            

                                                                                                                                                                                                                                                             
	Return @@ROWCOUNT
                                                                                                                                                                                                                                           
END
                                                                                                                                                                                                                                                          

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
Text                                                                                                                                                                                                                                                           
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
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
                                                                                                                                                                   
		Select @ReportId, wrk.LoginId, wrk.LoginName, det.Item, (Sum(IsNull(det.SubTotalLast,0)) - Sum(IsNull(det.SubTotalBegin,0))) As Subtotal, 'A'
                                                                                                              
			From ShiftStatMstr mstr, ShiftStatDet det, WorkersInShift wrk
                                                                                                                                                                                             
			Where mstr.ShiftId = det.ShiftId And mstr.ShiftId = wrk.ShiftId And  mstr.Status = 'A'
                                                                                                                                                                    
					And mstr.BeginTime >= @BeginTime And mstr.EndTime < @EndTime
                                                                                                                                                                                            
			Group By wrk.LoginId, wrk.LoginName, det.Item
                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
	Return @@ROWCOUNT
                                                                                                                                                                                                                                           
END
                                                                                                                                                                                                                                                          

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
Text                                                                                                                                                                                                                                                           
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
-- =======================================================
                                                                                                                                                                                                   
-- Author:		ZDM
                                                                                                                                                                                                                                              
-- Create date: 2016-09-26
                                                                                                                                                                                                                                   
-- Description:
                                                                                                                                                                                                                                              
-- Create the monthly report by Shift and Worker both,
                                                                                                                                                                                                       
-- in the tables MonthReportMstr, MonthReportDet
                                                                                                                                                                                                             
-- and WorkerReportDet
                                                                                                                                                                                                                                       
-- =======================================================
                                                                                                                                                                                                   
CREATE PROCEDURE [dbo].[sp_CreateMonthlyReport]
                                                                                                                                                                                                              
	@YearMonth char(6)		-- The format should be looks like '201602' for report of Feb. 2016
                                                                                                                                                                     
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
                                                                                                                                                                                                                        
	Select @firstDay = IsNull(Value, '1') From GeneralParams Where Category = 'System' And Name='FirstDayForMonthReportTime'
                                                                                                                                    
	Select @shift1StartTime =IsNull(Value, '8:00:00') From GeneralParams Where Category = 'System' And Name='ShiftStartTime1'
                                                                                                                                   
	Select @beginTime = CAST(@year + '-' + @month + '-' + @firstDay + ' ' + @shift1StartTime AS datetime)
                                                                                                                                                       
	Select @endTime = DATEADD(m,1,@beginTime)
                                                                                                                                                                                                                   
	
                                                                                                                                                                                                                                                            
	Begin Tran
                                                                                                                                                                                                                                                  
		Insert Into MonthReportMstr (ReportId, YearMonth, BeginTime, EndTime, CreateTime, IsFileCreated, Status)
                                                                                                                                                   
				Values(@ReportId, @yearMonth, @beginTime, @endTime, GETDATE(), 0, 'A')
                                                                                                                                                                                   
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
                                                                                                                                                                                                                                                          

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
Text                                                                                                                                                                                                                                                           
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
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
                                                                                                                                                                                             
	SELECT @begin = CONVERT(datetime, LEFT(CONVERT(char(10), @shiftBegin, 111), 8) + '01', 111)	
                                                                                                                                                                
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
                                                                                                                                                                                                                        
		SELECT @noon = CAST(@todayStr as datetime) + CAST('12:00:00' As datetime)	-- Uses 12:00:00 as a flag to check the two shift
                                                                                                                                

                                                                                                                                                                                                                                                             
		-- Day shift
                                                                                                                                                                                                                                               
		SELECT @dayWorkers=dbo.GetWorkerNameByShift(ShiftId) FROM ShiftStatMstr WHERE BeginTime>=@begin AND BeginTime<@noon
                                                                                                                                        

                                                                                                                                                                                                                                                             
		INSERT INTO @CurrentMonthRpt([Day], DayWorkers)
                                                                                                                                                                                                            
			VALUES(@day, ISNULL(@dayWorkers,'') )
                                                                                                                                                                                                                     

                                                                                                                                                                                                                                                             
		UPDATE @CurrentMonthRpt SET DayBiogas=
                                                                                                                                                                                                                     
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0)) - SUM(ISNULL(det.SubTotalBegin,0))), 0) FROM ShiftStatDet det, ShiftStatMstr mstr
                                                                                                                         
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status='A'
                                                                                                                                                                                                       
						AND (det.Item='Biogas2GenSubtotal' OR det.Item='Biogas2TorchSubtotal')
                                                                                                                                                                                 
						AND mstr.BeginTime>=@begin AND mstr.BeginTime<@noon)			
                                                                                                                                                                                                
			WHERE  [Day]=@day
                                                                                                                                                                                                                                         
		
                                                                                                                                                                                                                                                           
		UPDATE @CurrentMonthRpt SET DayEngeryProduction=
                                                                                                                                                                                                           
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0)) - SUM(ISNULL(det.SubTotalBegin,0))), 0) FROM ShiftStatDet det, ShiftStatMstr mstr
                                                                                                                         
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status='A'
                                                                                                                                                                                                       
						AND (det.Item='EnergyProduction1' OR det.Item='EnergyProduction2')
                                                                                                                                                                                     
						AND mstr.BeginTime>=@begin AND mstr.BeginTime<@noon)
                                                                                                                                                                                                   
			WHERE  [Day]=@day
                                                                                                                                                                                                                                         

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
		-- Night shift
                                                                                                                                                                                                                                             
		SELECT @shiftId = (SELECT TOP 1 ShiftId FROM ShiftStatMstr WHERE BeginTime>=@noon AND BeginTime<@end)
                                                                                                                                                      

                                                                                                                                                                                                                                                             
		UPDATE @CurrentMonthRpt SET NightWorkers=dbo.GetWorkerNameByShift(@shiftId), NightBiogas=
                                                                                                                                                                  
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0)) - SUM(ISNULL(det.SubTotalBegin,0))), 0) FROM ShiftStatDet det, ShiftStatMstr mstr
                                                                                                                         
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status='A'
                                                                                                                                                                                                       
						AND (det.Item='Biogas2GenSubtotal' OR det.Item='Biogas2TorchSubtotal')
                                                                                                                                                                                 
						AND mstr.BeginTime>=@noon AND mstr.BeginTime<@end)
                                                                                                                                                                                                     
			WHERE  [Day]=@day
                                                                                                                                                                                                                                         

                                                                                                                                                                                                                                                             
		UPDATE @CurrentMonthRpt SET NightEngeryProduction=
                                                                                                                                                                                                         
			(SELECT ISNULL((SUM(ISNULL(det.SubTotalLast,0)) - SUM(ISNULL(det.SubTotalBegin,0))), 0) FROM ShiftStatDet det, ShiftStatMstr mstr
                                                                                                                         
				WHERE mstr.ShiftId=det.ShiftId AND mstr.Status='A'
                                                                                                                                                                                                       
						AND (det.Item='EnergyProduction1' OR det.Item='EnergyProduction2')
                                                                                                                                                                                     
						AND mstr.BeginTime>=@noon AND mstr.BeginTime<@end)
                                                                                                                                                                                                     
			WHERE  [Day]=@day
                                                                                                                                                                                                                                         

                                                                                                                                                                                                                                                             
		SELECT @day = @day + 1
                                                                                                                                                                                                                                     
	END
                                                                                                                                                                                                                                                         

                                                                                                                                                                                                                                                             
	WHILE @day <= @lastDay
                                                                                                                                                                                                                                      
	BEGIN
                                                                                                                                                                                                                                                       
		INSERT INTO @CurrentMonthRpt([Day],DayWorkers,DayBiogas,DayEngeryProduction,NightWorkers,NightBiogas,NightEngeryProduction)
                                                                                                                                
			VALUES(@day,'',0,0,'',0,0)
                                                                                                                                                                                                                                

                                                                                                                                                                                                                                                             
		SELECT @day = @day + 1
                                                                                                                                                                                                                                     
	END
                                                                                                                                                                                                                                                         
	
                                                                                                                                                                                                                                                            
	SELECT [Day], DayWorkers, DayBiogas, DayEngeryProduction, NightWorkers, NightBiogas, NightEngeryProduction FROM @CurrentMonthRpt
                                                                                                                            

                                                                                                                                                                                                                                                             
	SELECT (ISNULL(SUM(DayBiogas),0) + ISNULL(SUM(NightBiogas),0)) AS Biogas, (ISNULL(SUM(DayEngeryProduction),0) + ISNULL(SUM(NightEngeryProduction),0)) AS EngeryProduction FROM @CurrentMonthRpt
                                                             
END
                                                                                                                                                                                                                                                          

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
Text                                                                                                                                                                                                                                                           
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                                                                                                                                                                                                                                                             
-- =============================================
                                                                                                                                                                                                             
-- Author:		ZDM
                                                                                                                                                                                                                                              
-- Create date: 20170206
                                                                                                                                                                                                                                     
-- Description:	Get monthly data by the shift/item
                                                                                                                                                                                                           
-- =============================================
                                                                                                                                                                                                             
CREATE PROCEDURE [dbo].[sp_GetMonthReportData]
                                                                                                                                                                                                               
	@ReportId uniqueidentifier
                                                                                                                                                                                                                                  
AS
                                                                                                                                                                                                                                                           
BEGIN
                                                                                                                                                                                                                                                        
	-- SET NOCOUNT ON added to prevent extra result sets from
                                                                                                                                                                                                   
	-- interfering with SELECT statements.
                                                                                                                                                                                                                      
	SET NOCOUNT ON;
                                                                                                                                                                                                                                             

                                                                                                                                                                                                                                                             
 	DECLARE @MonthReport TABLE
                                                                                                                                                                                                                                 
	(
                                                                                                                                                                                                                                                           
		ShiftId uniqueidentifier,
                                                                                                                                                                                                                                  
		[Day] nvarchar(10),
                                                                                                                                                                                                                                        
		[Shift] nvarchar(10),
                                                                                                                                                                                                                                      
		Worker1 nvarchar(100),
                                                                                                                                                                                                                                     
		Worker2 nvarchar(100),
                                                                                                                                                                                                                                     
		EnergyProduction1 int,
                                                                                                                                                                                                                                     
		EnergyProduction2 int,
                                                                                                                                                                                                                                     
		Biogas2GenSubtotal int,
                                                                                                                                                                                                                                    
		Biogas2TorchSubtotal int,
                                                                                                                                                                                                                                  
		SubtotalRuntime1 int,
                                                                                                                                                                                                                                      
		SubtotalRuntime2 int
                                                                                                                                                                                                                                       
	);
                                                                                                                                                                                                                                                          
	
                                                                                                                                                                                                                                                            
	Insert Into @MonthReport (ShiftId,[Day],[Shift])
                                                                                                                                                                                                            
		Select ShiftId,Convert(varchar(10), BeginTime, 111), Case When Cast(Convert(Varchar(2), BeginTime, 114) As Int) < 12 Then 'Day' Else 'Night' End
                                                                                                           
			From MonthReportShiftDet Where ReportId=@ReportId Order By BeginTime
                                                                                                                                                                                      

                                                                                                                                                                                                                                                             
	Update @MonthReport Set EnergyProduction1=IsNull(Subtotal,0) From @MonthReport mr,MonthReportDet mrd
                                                                                                                                                        
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item='EnergyProduction1'
                                                                                                                                                                   

                                                                                                                                                                                                                                                             
	Update @MonthReport Set EnergyProduction2=IsNull(Subtotal,0) From @MonthReport mr,MonthReportDet mrd
                                                                                                                                                        
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item='EnergyProduction2'
                                                                                                                                                                   

                                                                                                                                                                                                                                                             
	Update @MonthReport Set Biogas2GenSubtotal=IsNull(Subtotal,0) From @MonthReport mr,MonthReportDet mrd
                                                                                                                                                       
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item='Biogas2GenSubtotal'
                                                                                                                                                                  

                                                                                                                                                                                                                                                             
	Update @MonthReport Set Biogas2TorchSubtotal=IsNull(Subtotal,0) From @MonthReport mr,MonthReportDet mrd
                                                                                                                                                     
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item='Biogas2TorchSubtotal'
                                                                                                                                                                

                                                                                                                                                                                                                                                             
	Update @MonthReport Set SubtotalRuntime1=IsNull(Subtotal,0) From @MonthReport mr, MonthReportDet mrd
                                                                                                                                                        
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item='SubtotalRuntime1'
                                                                                                                                                                    

                                                                                                                                                                                                                                                             
	Update @MonthReport Set SubtotalRuntime2=IsNull(Subtotal,0) From @MonthReport mr, MonthReportDet mrd
                                                                                                                                                        
		Where mr.ShiftId=mrd.ShiftId And mrd.ReportId=@ReportId And mrd.Item='SubtotalRuntime2'
                                                                                                                                                                    

                                                                                                                                                                                                                                                             
	Update @MonthReport Set Worker1=wk.LoginName
                                                                                                                                                                                                                
		From @MonthReport mr, (Select ShiftId,LoginName,(ROW_NUMBER() OVER(PARTITION by ShiftId ORDER BY ShiftId)) As [Row] From WorkersInShift) wk
                                                                                                                
			Where mr.ShiftId=wk.ShiftId And wk.Row=1
                                                                                                                                                                                                                  

                                                                                                                                                                                                                                                             
	Update @MonthReport Set Worker2=wk.LoginName
                                                                                                                                                                                                                
		From @MonthReport mr, (Select ShiftId,LoginName,(ROW_NUMBER() OVER(PARTITION by ShiftId ORDER BY ShiftId)) As [Row] From WorkersInShift) wk
                                                                                                                
			Where mr.ShiftId=wk.ShiftId And wk.Row=2
                                                                                                                                                                                                                  

                                                                                                                                                                                                                                                             
	Select ROW_NUMBER() OVER(Order By [Day]) As [Row],[Day],[Shift],Worker1,Worker2,EnergyProduction1,EnergyProduction2,Biogas2GenSubtotal,Biogas2TorchSubtotal,SubtotalRuntime1,SubtotalRuntime2 from @MonthReport
                                             
END
                                                                                                                                                                                                                                                          

                                                                                                                                                                                                                                                             
Text                                                                                                                                                                                                                                                           
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                                                                                                                                                                                                                                                             
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
                                                                                                                                                                                                                                     

                                                                                                                                                                                                                                                             
	-- If the data type is not boolean, should not have the value less than 0
                                                                                                                                                                                   
	If (@dateType <> 11) And @val <= 0 Return
                                                                                                                                                                                                                   

                                                                                                                                                                                                                                                             
	Select @needAccumulate = IsNull(NeedAccumulate,0) From MonitorItem Where ItemID = @item
                                                                                                                                                                     
	If @needAccumulate = 0 Return
                                                                                                                                                                                                                               
	
                                                                                                                                                                                                                                                            
	If ((@suffix = '.H') Or (@suffix = '.L'))
                                                                                                                                                                                                                   
		-- Need to combine the value of .H and .L to one single value. Assume the value of .H is a integer
                                                                                                                                                         
		-- For example: xxx.H = 12, xxx.L = 345, then the real value is 12345
                                                                                                                                                                                      
		Begin
                                                                                                                                                                                                                                                      
			Select @realItem = Left(@item, Len(@item) - 2)
                                                                                                                                                                                                            
			If (@suffix = '.H')
                                                                                                                                                                                                                                       
				Begin
                                                                                                                                                                                                                                                    
					Select @anotherSuffix = '.L'
                                                                                                                                                                                                                            
					Select @anotherItem = @realItem + @anotherSuffix		-- xxx.L
                                                                                                                                                                                              
					Select @anotherValue = Cast(Val As int) From ItemLatestStatus Where ItemId = @anotherItem
                                                                                                                                                               
					If (@@ROWCOUNT = 1)
                                                                                                                                                                                                                                     
						Select @realValue = CAST((CAST(@val As varchar(100)) + CAST(@anotherValue as varchar(100))) As int)
                                                                                                                                                    
					Else	-- Not found the value of xxx.L
                                                                                                                                                                                                                    
						Select @realValue = @val
                                                                                                                                                                                                                               
				End
                                                                                                                                                                                                                                                      
			Else
                                                                                                                                                                                                                                                      
				Begin
                                                                                                                                                                                                                                                    
					Select @anotherSuffix = '.H'
                                                                                                                                                                                                                            
					Select @anotherItem = @realItem + @anotherSuffix		-- xxx.H
                                                                                                                                                                                              
					Select @anotherValue = Cast(Val As int) From ItemLatestStatus Where ItemId = @anotherItem
                                                                                                                                                               
					If (@@ROWCOUNT = 1)
                                                                                                                                                                                                                                     
						Select @realValue = CAST((CAST(@anotherValue As varchar(100)) + CAST(@val as varchar(100))) As int)	
                                                                                                                                                   
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
                                                                                                                                                                                 

                                                                                                                                                                                                                                                             
	Declare @subtotalBegin int
                                                                                                                                                                                                                                  
	Declare @newSubtotalBegin int
                                                                                                                                                                                                                               
	Declare @subtotalLast int
                                                                                                                                                                                                                                   
	
                                                                                                                                                                                                                                                            
	Select @subtotalLast = SubTotalLast, @subtotalBegin = SubTotalBegin From ShiftStatDet Where ShiftId = @shiftId and Item = @realItem
                                                                                                                         
	If @@ROWCOUNT = 0	-- Record not exist
                                                                                                                                                                                                                       
		Begin
                                                                                                                                                                                                                                                      
			Insert Into ShiftStatDet (ShiftId, Item, SubTotalBegin, SubTotalLast) Values(@shiftId, @realItem, @realValue, @realValue)
                                                                                                                                 

                                                                                                                                                                                                                                                             
			-- Record the abnormal change
                                                                                                                                                                                                                             
			Insert Into AbnormalChange (ShiftId, ItemName, PreValue, NewValue, UpdateTime, CreateTime) Values (@shiftId, @realItem, -1, @realValue, @newTime, GetDate())
                                                                                              
			Return
                                                                                                                                                                                                                                                    
		End
                                                                                                                                                                                                                                                        

                                                                                                                                                                                                                                                             
	If (@realValue >= @subtotalLast)	-- The value change is reasonable
                                                                                                                                                                                          
		Begin
                                                                                                                                                                                                                                                      
			Update ShiftStatDet Set SubTotalLast = @realValue Where ShiftId = @shiftId And Item = @realItem
                                                                                                                                                           
		End
                                                                                                                                                                                                                                                        
	Else							-- The subtotal value must be reset, need to r-calculte the SubtotalBegin
                                                                                                                                                                        
		Begin
                                                                                                                                                                                                                                                      
			Select @newSubtotalBegin = @subtotalBegin - @subtotalLast	-- It is a neg value
                                                                                                                                                                            
			Update ShiftStatDet Set SubTotalBegin = @newSubtotalBegin, SubTotalLast = @realValue Where ShiftId = @shiftId And Item = @realItem
                                                                                                                        

                                                                                                                                                                                                                                                             
			-- Record the abnormal change
                                                                                                                                                                                                                             
			Insert Into AbnormalChange (ShiftId, ItemName, PreValue, NewValue, UpdateTime, CreateTime) Values (@shiftId, @realItem, @subtotalBegin, @newSubtotalBegin, @newTime, GetDate())
                                                                           
			Insert Into AbnormalChange (ShiftId, ItemName, PreValue, NewValue, UpdateTime, CreateTime) Values (@shiftId, @realItem, @subtotalLast, @realValue, @newTime, GetDate())
                                                                                   
		End	
                                                                                                                                                                                                                                                       
END
                                                                                                                                                                                                                                                          

                                                                                                                                                                                                                                                             
--- Generate DDL from database [OPC-Site] --- 
