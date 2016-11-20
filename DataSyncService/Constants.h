#pragma once

#define LOG_FILENAME					_T("C:\\log\\DataSyncService.log")
#define DS_SERVICE_DISPLAYNAME			_T("Data Synchronization service")			// Service fullname
#define DS_SERVICE_DESCRIPTION			_T("Real time synchronize the device state via OPC server")			// Service description

#define DefaultConnectString			_T("Driver={SQL Server};Server=.;Database=OPC;Trusted_Connection=yes;")
#define TimerTasksFileName				_T("TimerTasks.ini")

#define LOCK_WAIT_TIMEOUT				1000			// 1s

#define ERR_SUCCESS						0
#define ERR_LOCK_TIMEOUT				-200

#define DEFAULT_QRY_INTERVAL			30000		// 30 seconds
#define DEFAULT_LOG_FLAG				TRUE		// false
#define DEFAULT_KEEP_DB_CONNECT			TRUE
#define DEFAULT_ITEMS_EACHTIME			100

#define DEFAULT_SHIFT_START_1			_T("8:00:00")
#define DEFAULT_SHIFT_START_2			_T("20:00:00")