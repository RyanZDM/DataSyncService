
Update GeneralParams Set Value='2' Where Category='System' And [Name]='CreateMonthReportTime'
If @@ROWCOUNT < 1
Begin
	Insert Into GeneralParams ([Category], [Name], [Value], [DispOrder], [DispName], [Memo], [Hide], [IsEncrypted], [IsProtected]) 
		Values (N'System', N'CreateMonthReportTime', N'2', 9, N'�±�����ʱ��', N'ÿ���µļ��ţ���Чֵ1-31', 0, 0, 1)
End