@Echo off
Echo Stopping DataSyncService...
net stop DataSyncService
TASKKILL /F /IM DataSyncService.exe /T