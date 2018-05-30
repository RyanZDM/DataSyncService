@echo off
cd /d "%~dp0"

Echo Updating database...
sqlcmd -E -d OPC -S .\WINCC -i UpdateDB.sql -o c:\log\Patch180530.log
sqlcmd -E -d OPC -S .\WINCC -i 6.sp_GetCurrentMonthDataByDay.StoredProcedure.sql -o c:\log\Patch180530.log
sqlcmd -E -d OPC -S .\WINCC -i 7.sp_GetMonthReportData.StoredProcedure.sql -o c:\log\Patch180530.log

@echo Patch applied. >> C:\log\Patch180530.log

Echo Patch applied.

Timeout 10

:End