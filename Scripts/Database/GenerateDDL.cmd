@echo off
cd /d "%~dp0"

Set Database=OPC-Site

sqlcmd -E -d %Database% -S .\Carestream -i GenerateDDL.sql -o DDL.sql

@echo --- Generate DDL from database [%Database%] --- >> DDL.sql
