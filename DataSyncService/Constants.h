#pragma once

#define LOG_FILENAME					_T("C:\\log\\DataSyncService.log")
#define DS_SERVICE_DISPLAYNAME			_T("Data Synchronization service")			// Service fullname
#define DS_SERVICE_DESCRIPTION			_T("Real time synchronize the device state via OPC server")			// Service description

#define DefaultConnectString			_T("Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=OPC;Data Source=.")
#define TimerTasksFileName				_T("TimerTasks.ini")