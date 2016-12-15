using NLog;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.Linq;

namespace EBoard.Common
{
	public class Reporter
	{
		private readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

		private const string DefaultReportFileTemplate = @"C:\MonthlyReport\Template\MonthlyReportTemplate.xlsx";

		private const string DefaultReportFilenameFormat = @"MonthlyReport-{0}-{1:D2}.xlsx";

		private const string DefaultPathToReportFile = @"C:\MonthlyReport";

		private const string ReportFileTempateParam = @"ReportFileTemplate";

		private const string ReportFilenameFormatParam = @"ReportFilenameFormat";

		private const string PathToReportFileParam = @"PathToReportFile";

		private const string NameOfDataTabInExcel = "Data";

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
					{
						//logger.Warn("The report for {0}-{1} has not been created yet.", year, month);
						throw new ReportNotCreatYetException(string.Format("{0}-{1}", year, month));
					}
					
					var isFileCreated = reader.GetBoolean(1);
					if (isFileCreated)
					{
						//logger.Warn("The report file for {0}-{1} has been created already.", year, month);
						throw new ReportFileAlreadyCreatedException(string.Format("{0}-{1}", year, month));
					}

					var reportId = reader.GetGuid(0).ToString();
					reader.Close();

					CreateReportExcel(reportId, year, month);
				}
			}
		}
		
		public void CreateReportFile(string reportId)
		{
			// Do not consider the Status since provided the report id
			using (var command = new SqlCommand(string.Format(@"Select IsFileCreated,YearMonth From MonthReportMstr Where ReportId=CAST('{0}' AS uniqueidentifier)", reportId), connection))
			{
				using (var reader = command.ExecuteReader())
				{
					if (!reader.Read())
						throw new ReportNotFoundException(reportId);
					
					var isFileCreated = reader.GetBoolean(0);
					if (isFileCreated)
					{
						//logger.Warn("The report file for {0} has been created already.", reportId);
						throw new ReportFileAlreadyCreatedException(reportId);
					}

					var yearMonth = reader.GetString(1);
					// Assume the string must be 6 digit	
					var year = int.Parse(yearMonth.Substring(0, 4));
					var month = int.Parse(yearMonth.Substring(4));
					reader.Close();

					CreateReportExcel(reportId, year, month);
				}
			}
		}
		
		private string CreateReportExcel(string reportId, int year, int month)
		{
			// 1. Get the target folder and file name format for saving report
			var dal = new Dal(connection);
			var generalParams = dal.GetGeneralParameters();
			var param = generalParams.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase) 
														&& string.Equals(p.Name, PathToReportFileParam, StringComparison.OrdinalIgnoreCase));
			var targetPath = (param != null) ? param.Value : DefaultPathToReportFile;
			if (!Directory.Exists(targetPath))
			{
				Directory.CreateDirectory(targetPath);
			}

			param = generalParams.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase)
														&& string.Equals(p.Name, ReportFilenameFormatParam, StringComparison.OrdinalIgnoreCase));
			var targetFilenameFormat = (param != null) ? param.Value : DefaultReportFilenameFormat;
			var targetFilename = string.Format(targetFilenameFormat, year, month, DateTime.Now); // Always provide the 3rd param even though the format not use it
			targetFilename = Path.Combine(targetPath, targetFilename);
			if (File.Exists(targetFilename))
			{
				logger.Info("Delete the reprot file [{0}] since it is already exist.", targetFilename);
				File.Delete(targetFilename);
			}

			// 3. Find out the predefined template and copy it to target folder
			param = generalParams.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase)
														&& string.Equals(p.Name, ReportFileTempateParam, StringComparison.OrdinalIgnoreCase));
			if (param != null)
			{
				if (!File.Exists(param.Value))
				{
					var msg = string.Format("The template for monthly report not found. [{0}]", param.Value);
					//logger.Error(msg);
					throw new FileNotFoundException(msg);
				}

				File.Copy(param.Value, targetFilename, true);
			}
			else
			{
				// Try to find the default template if it is not defined in databse
				if (File.Exists(DefaultReportFileTemplate))
				{
					File.Copy(DefaultReportFileTemplate, targetFilename, true);
				}
			}

			// 4. Create or open target Excel, get monthly report info from db and write them to the target tab in Excel
			using (var app = new ExcelApp())
			{
				var workbook = app.CreateOrOpenExcel(targetFilename);
				var worksheet = app.CreateOrOpenWorksheet(workbook, NameOfDataTabInExcel);

				// 4.1 Data from MonthReportMstr
				var startRow = 2;
				var sql = string.Format(@"Select * From MonthReportMstr Where ReportId='{0}'", reportId);
				var reportDs = new DataSet();
				new SqlDataAdapter(sql, connection).Fill(reportDs);
				app.WriteData(worksheet, reportDs.Tables[0], true, startRow);

				// 4.2 Data from MonthReportDet
				startRow += (reportDs.Tables[0].Rows.Count + 2);
				var detDs = new DataSet();
				sql = string.Format(@"Select ShiftId,Item,DisplayName As ItemName,Subtotal From MonthReportDet,MonitorItem Where Item=ItemId And MonthReportDet.Status='A' And ReportId='{0}'", reportId);
				new SqlDataAdapter(sql, connection).Fill(detDs);
				app.WriteData(worksheet, detDs.Tables[0], false, startRow);

				// 4.3 Data from MonthWorkerReportDet
				startRow += (detDs.Tables[0].Rows.Count + 2);
				var workerDetDs = new DataSet();
				sql = string.Format(@"Select WorkerId,WorkerName,Item,DisplayName As ItemName,Subtotal From MonthWorkerReportDet,MonitorItem Where Item=ItemId And MonthWorkerReportDet.Status='A' And ReportId='{0}'", reportId);
				new SqlDataAdapter(sql, connection).Fill(workerDetDs);
				app.WriteData(worksheet, workerDetDs.Tables[0], false, startRow);

				workbook.Save();
			}

			return targetFilename;
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

		/// <summary>
		/// Gets list of all monthly report
		/// </summary>
		/// <returns></returns>
		public SortedDictionary<int, SortedDictionary<int, string>> GetMonthltReportList()
		{
			var sql = "Select YearMonth, ReportId From MonthReportMstr Where Status='A'";
			var cmd = new SqlCommand(sql, connection);
			var reportList = new SortedDictionary<int, SortedDictionary<int, string>>();
			using (var reader = cmd.ExecuteReader())
			{
				while (reader.Read())
				{
					var yearMonth = reader.GetString(0);
					var year = int.Parse(yearMonth.Substring(0, 4));
					var month = int.Parse(yearMonth.Substring(4));
					var reportId = reader.GetGuid(1).ToString();

					if (!reportList.ContainsKey(year))
					{
						reportList.Add(year, null);
					}

					if (reportList[year] == null)
					{
						reportList[year] = new SortedDictionary<int, string>();
					}
					
					reportList[year][month] = reportId;
				}

				reader.Close();
			}

			return reportList;
		}
	}
	
	public class ReportNotCreatYetException : Exception
	{
		public ReportNotCreatYetException(string msg) : base(msg) { }
	}
	
	public class ReportNotFoundException : Exception
	{
		public ReportNotFoundException(string msg) : base(msg) { }
	}
	
	public class ReportFileAlreadyCreatedException : Exception
	{
		public ReportFileAlreadyCreatedException(string msg) : base(msg) { }
	}
	
}
