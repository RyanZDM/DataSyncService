@echo off
cd /d "%~dp0"

:: Update stored procedure
sqlcmd -E -d OPC -S .\WINCC -i sp_GetCurrentShiftId.sql -o c:\log\Update0209-1.log

sqlcmd -E -d OPC -S .\WINCC -i RemoveDuplicate.sql -o c:\log\Update0209-2.log


Echo Patch applied.
Pause