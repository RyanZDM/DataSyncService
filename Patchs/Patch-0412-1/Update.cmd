@echo off
cd /d "%~dp0"

sqlcmd -E -d OPC -S .\WINCC -i sp_GetCurrentShiftId.sql -o c:\log\Update0412-1.log

@echo Patch applied. >> C:\log\Update0412-1.log

Echo Patch applied.
Pause