::@echo off
cd /d "%~dp0"

:: Add table
sqlcmd -E -d OPC -S .\WINCC -i t_AbnormalChange.sql -o c:\log\Update1.log

:: Update trigger
sqlcmd -E -d OPC -S .\WINCC -i tr_UpdateItemSubtotal.sql -o c:\log\Update2.log

:: Update stored procedure
sqlcmd -E -d OPC -S .\WINCC -i sp_GetCurrentMonthDataByDay.sql -o c:\log\Update3.log