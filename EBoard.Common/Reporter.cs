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
		private readonly Logger logger = LogManager.GetCurrentClassLogger();

		private const string DefaultReportFileTemplate = @"C:\MonthlyReport\Template\MonthlyReportTemplate.xlsx";

		private const string DefaultReportFilenameFormat = @"MonthlyReport-{0}-{1:D2}.xlsx";

		private const string DefaultPathToReportFile = @"C:\MonthlyReport";

		private const string ReportFileTempateParam = @"ReportFileTemplate";

		private const string ReportFilenameFormatParam = @"ReportFilenameFormat";

		private const string PathToReportFileParam = @"PathToReportFile";

		private const string NameOfDataTabInExcel = "Data";

		private readonly SqlConnection connection;
		public Reporter(SqlConnection conn)
		{
			connection = conn;
		}

		public Tuple<string, bool> CreateReportFile(int year, int month)
		{
			using (var command = new SqlCommand(
				$@"Select ReportId,IsFileCreated From MonthReportMstr Where Status='A' And YearMonth='{year}{month:D2}'", connection))
			{
				using (var reader = command.ExecuteReader())
				{
					if (!reader.Read())
					{
						//logger.Warn("The report for {0}-{1} has not been created yet.", year, month);
						throw new ReportNotCreatYetException($"{year}-{month}");
					}
					
					var isFileCreated = reader.GetBoolean(1);
					if (isFileCreated)
					{
						//logger.Warn("The report file for {0}-{1} has been created already.", year, month);
						throw new ReportFileAlreadyCreatedException($"{year}-{month}");
					}

					var reportId = reader.GetGuid(0).ToString();
					reader.Close();

					return CreateReportExcel(reportId, year, month);
				}
			}
		}
		
		public Tuple<string, bool> CreateReportFile(string reportId)
		{
			// Do not consider the Status since provided the report id
			using (var command = new SqlCommand(
				$@"Select IsFileCreated,YearMonth From MonthReportMstr Where ReportId=CAST('{reportId}' AS uniqueidentifier)", connection))
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

					return CreateReportExcel(reportId, year, month);
				}
			}
		}
		
		/// <summary>
		/// Create Excel for the specified monthly report
		/// </summary>
		/// <param name="reportId"></param>
		/// <param name="year"></param>
		/// <param name="month"></param>
		/// <returns>The path to Excel file and the result if successfully updated the mstr table.</returns>
		private Tuple<string, bool> CreateReportExcel(string reportId, int year, int month)
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
					var msg = $"The template for monthly report not found. [{param.Value}]";
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
				var sql = $@"Select * From MonthReportMstr Where ReportId='{reportId}'";
				var reportDs = new DataSet();
				new SqlDataAdapter(sql, connection).Fill(reportDs);
				app.WriteData(worksheet, reportDs.Tables[0], true, startRow);

				// 4.2 Data from MonthReportShitDet

				// 4.3 Data from WorkersInShift

				// TODO: Item name in MonitorItem and MonthReportDet not match!
				// 4.2 Data from MonthReportDet
				startRow += (reportDs.Tables[0].Rows.Count + 2);
				var detDs = new DataSet();
				sql =
					$@"Select ShiftId,Item,DisplayName As ItemName,Subtotal From MonthReportDet,MonitorItem Where Item=ItemId And MonthReportDet.Status='A' And ReportId='{reportId}'";
				new SqlDataAdapter(sql, connection).Fill(detDs);
				app.WriteData(worksheet, detDs.Tables[0], false, startRow);

				// 4.3 Data from MonthWorkerReportDet
				startRow += (detDs.Tables[0].Rows.Count + 2);
				var workerDetDs = new DataSet();
				sql =
					$@"Select WorkerId,WorkerName,Item,DisplayName As ItemName,Subtotal From MonthWorkerReportDet,MonitorItem Where Item=ItemId And MonthWorkerReportDet.Status='A' And ReportId='{reportId}'";
				new SqlDataAdapter(sql, connection).Fill(workerDetDs);
				app.WriteData(worksheet, workerDetDs.Tables[0], false, startRow);

				workbook.Save();
				logger.Info($"The Excel file for the report '{year}-{month}/{reportId}' created.");
			}
			
			// 5. Update the mstr table
			var updateSql =
					$@"Update MonthReportMstr Set IsFileCreated=1,FilePath='{targetFilename}',FileCreateTime=GetDate() Where ReportId=Cast('{reportId}' As uniqueidentifier)";
			var ret = new Tuple<string, bool>(targetFilename, (new SqlCommand(updateSql, connection).ExecuteNonQuery() > 0));
			if (!ret.Item2)
			{
				logger.Error("Failed to update IsFileCreated flag for the monthly report '{0}'", reportId);
			}

			return ret;
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

		public DataSet GetMonthReportMstr(string reportId)
		{
			var sql = $@"Select * From MonthReportMstr Where ReportId=CAST('{reportId}' As uniqueidentifier)";
			var adapter = new SqlDataAdapter(sql, connection);
			var ds = new DataSet();
			adapter.Fill(ds);

			return ds;
		}

		public DataSet GetMonthReportStatDet(string reportId)
		{
			var ds = new DataSet();
			var sql =
				$@"Select ShiftId,DisplayName,Subtotal From MonthReportDet, MonitorItem Where Item=ItemId And MonthReportDet.Status='A' And ReportId=CAST('{reportId}' As uniqueidentifier) Order By ShiftId, DisplayName";
			var adapter = new SqlDataAdapter(sql, connection);
			adapter.Fill(ds, "StatDet");

			sql =
				$@"Select Distinct mrd.ShiftId,BeginTime,ActualBeginTime,EndTime From MonthReportDet mrd, ShiftStatMstr ssm Where mrd.ShiftId=ssm.ShiftId And mrd.Status='A' And ReportId=CAST('{reportId}' As uniqueidentifier) Order By BeginTime";
			adapter = new SqlDataAdapter(sql, connection);
			adapter.Fill(ds, "Shift");

			return ds;
		}

		public DataSet GetMonthReportWorkerDet(string reportId)
		{
			var ds = new DataSet();
			var sql =
				$@"Select WorkerId,WorkerName,DisplayName,Subtotal From MonthWorkerReportDet mwd, MonitorItem mi Where Item=ItemId And mwd.Status='A' And ReportId=CAST('{reportId}' As uniqueidentifier) Order By WorkerId,DisplayName";
			var adapter = new SqlDataAdapter(sql, connection);
			adapter.Fill(ds, "StatDet");

			sql =
				$@"Select Distinct WorkerId,WorkerName From MonthWorkerReportDet Where Status='A' And ReportId=CAST('{reportId}' As uniqueidentifier) Order By WorkerId";
			adapter = new SqlDataAdapter(sql, connection);
			adapter.Fill(ds, "Worker");

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
