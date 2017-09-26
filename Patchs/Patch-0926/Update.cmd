@echo off
cd /d "%~dp0"

Echo Updating database...
sqlcmd -E -d OPC -S .\WINCC -i UpdateDB.sql -o c:\log\Patch0928.log

@echo Patch applied. >> C:\log\Patch0928.log

Echo Patch applied.

Timeout 10

:End