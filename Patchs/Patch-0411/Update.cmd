@echo off
cd /d "%~dp0"

sqlcmd -E -d OPC -S .\WINCC -i t_MonthReportShiftDet.sql -o c:\log\Update0411-1.log

sqlcmd -E -d OPC -S .\WINCC -i sp_GetCurrentShiftId.sql -o c:\log\Update0411-2.log

sqlcmd -E -d OPC -S .\WINCC -i f_GetWorkerNameByShift.sql -o c:\log\Update0411-3.log

sqlcmd -E -d OPC -S .\WINCC -i tr_UpdateItemSubtotal.sql -o c:\log\Update0411-4.log

sqlcmd -E -d OPC -S .\WINCC -i Correction.sql -o c:\log\Update0411-5.log

@echo Patch applied. > C:\log\Update0411-0.log

Echo Patch applied.
Pause