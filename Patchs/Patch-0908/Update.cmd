@echo off
cd /d "%~dp0"

Echo Updating database...
sqlcmd -E -d OPC -S .\WINCC -i tr_UpdateItemSubtotal.sql -o c:\log\Patch0908.log

sqlcmd -E -d OPC -S .\WINCC -i UpdateParams.sql -o c:\log\Patch0908.log

@echo Patch applied. >> C:\log\Patch0908.log

Echo Patch applied.

Timeout 10

:End