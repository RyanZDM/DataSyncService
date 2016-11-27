using System;
using System.Data;
using System.Data.SqlClient;

namespace EBoard.Common
{
	public class Reporter
	{
		private SqlConnection connection;
		public Reporter(SqlConnection conn)
		{
			connection = conn;
		}

		public void CreateReportFile(int year, int month)
		{
			using (var command = new SqlCommand(string.Format(@"Select ReportId,IsFileCreated From MonthReportMstr Where Status='A' And YearMonth='{0}{1:D2}'", year, month), connection))
			{
				using (var reader = command.ExecuteReader())
				{
					if (!reader.Read())
						throw new ReportNotCreatYetException();
					
					var isFileCreated = reader.GetBoolean(1);
					if (isFileCreated)
						throw new ReportFileAlreadyCreatedException();
					
					CreateReportExcel(reader.GetString(0));
				}
			}
		}
		
		public void CreateReportFile(string reportId)
		{
			// Do not consider the Status since provided the report id
			using (var command = new SqlCommand(string.Format(@"Select IsFileCreated From MonthReportMstr Where ReportId=CAST('{0}' AS uniqueidentifier)", reportId), connection))
			{
				using (var reader = command.ExecuteReader())
				{
					if (!reader.Read())
						throw new ReportNotFoundException();
					
					var isFileCreated = reader.GetBoolean(0);
					if (isFileCreated)
						throw new ReportFileAlreadyCreatedException();
					
					CreateReportExcel(reportId);
				}
			}
		}
		
		private bool CreateReportExcel(string reportId)
		{
			// TODO:
			// 1. Get the target folder to save report
			
			// 2. Get the file naming convention and generate the actual file name
			
			// 3. Find out the path to predefined template from GeneralParms table
			
			// 4. Find out the template and copy it to target folder
			//    If not found or the template file is not defined, directly a new ExcelHelper
			
			// 5. Open that Excel, if not find the special tab, create it
			
			// 6. Get monthly report info from database and write them to the target tab in Excel
			
			var excelHelper = new ExcelApp();
			return false;
		}

		public DataSet GetCurrentMonthDataByDay()
		{
			var command = new SqlCommand
			{
				Connection = connection,
				CommandType = CommandType.StoredProcedure,
				CommandText = "sp_GetCurrentMonthDataByDay"
			};

			var adapter = new SqlDataAdapter(command);
			var ds = new DataSet();
			adapter.Fill(ds);

			return ds;
		}
	}
	
	public class ReportNotCreatYetException : Exception
	{
	}
	
	public class ReportNotFoundException : Exception
	{
	}
	
	public class ReportFileAlreadyCreatedException : Exception
	{
	}
	
}
