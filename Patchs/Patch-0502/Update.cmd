@echo off
cd /d "%~dp0"

Echo Stopping EBoard...
TASKKILL /F /IM EBoard.exe /T

Echo Stopping LEDSyncService...
net stop LEDSyncService
TASKKILL /F /IM LedSyncService.exe /T

Echo Stopping DataSyncService...
net stop DataSyncService
TASKKILL /F /IM DataSyncService.exe /T

Echo Updating database...
sqlcmd -E -d OPC -S .\WINCC -i UpdateParams.sql -o c:\log\Patch0502.log

Echo Coping files...

if exist "C:\Program Files (x86)\Electronic Board\*.*" (
	echo "found files in correct path" >> c:\log\Patch0502Files.log
)
cd Files
xcopy *.* "C:\Program Files (x86)\Electronic Board\" /R /Y / >> C:\Log\Patch0502Files.log
copy "C:\Program Files (x86)\Electronic Board\GenerateLastMonthReport.exe.config" "C:\Program Files (x86)\Electronic Board\GenerateMonthReport.exe.config" /y

cd ..
md D:\MonthlyReport
md D:\MonthlyReport\Template
xcopy MonthlyReportTemplate.xlsm D:\MonthlyReport\Template\ /R /Y >> C:\Log\Patch0502Files.log

@echo Patch applied. >> C:\log\Patch0502.log

Echo Patch applied.
Pause

:End