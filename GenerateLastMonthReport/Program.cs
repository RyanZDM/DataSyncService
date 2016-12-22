using System;
using EBoard.Common;
using System.Data.SqlClient;
using System.Data;
using NLog;

namespace GenerateLastMonthReport
{
	class Program
	{
		private static readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

		static int Main(string[] args)
		{
			var now = DateTime.Now;
			var year = now.Year;
			var month = now.Month;
			var yearMonth = now.ToString("yyyyMM");
			try
			{
				var connection = DbFactory.GetConnection();
				var cmd = new SqlCommand("sp_CreateMonthlyReport", connection)
				{
					CommandType = CommandType.StoredProcedure
				};

				var yearMonthParam = cmd.Parameters.AddWithValue("@YearMonth", yearMonth);

				var reportIdParam = cmd.Parameters.Add(new SqlParameter
				{
					ParameterName = "@ReportId",
					SqlDbType = SqlDbType.UniqueIdentifier,
					Direction = ParameterDirection.Output
				});

				var returnParam = cmd.Parameters.Add(new SqlParameter
				{
					ParameterName = "@return",
					SqlDbType = SqlDbType.Int,
					Direction = ParameterDirection.ReturnValue
				});

				cmd.ExecuteNonQuery();

				var reportId = reportIdParam.Value.ToString();
				var result = (int)returnParam.Value;

				logger.Info("Create the monthly report for {9}, result={1}, ReportId='{2}'", yearMonth, result, reportId);

				var sql = string.Format(@"Select IsFIleCreated From MonthReportMstr Where ReportId=Convert('{0} As uniqueidentifier)", reportId);
				var isFileCreated = (bool)(new SqlCommand(sql, connection).ExecuteScalar());
				logger.Info("Excel file of monthly report for {0} {1}.", yearMonth, isFileCreated ? "is created" : "is not created yet");

				if (isFileCreated)
					return 1;

				// Create Excel file
				var reporter = new Reporter(connection);
				reporter.CreateReportFile(reportId);

				sql = string.Format(@"Update MonthReportMstr Set IsFileCreated=1,FilePath='{0}',FileCreateTime=GetDate() Where ReportId=Convert('{1} As uniqueidentifier)", "todo", reportId);
				if (new SqlCommand(sql, connection).ExecuteNonQuery() < 1)
				{
					logger.Error("Failed to update IsFileCreated flag for the monthly report '{0}'", reportId);
					return 0;
				}

				return 1;
			}
			catch (Exception ex)
			{
				logger.Error(ex, "Error occurred while creating monthly report for {0}", yearMonth);
				return -1;
			}
		}
	}
}
