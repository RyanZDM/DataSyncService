@Echo off
Echo Stopping LEDSyncService...
net stop LEDSyncService
TASKKILL /F /IM LedSyncService.exe /T