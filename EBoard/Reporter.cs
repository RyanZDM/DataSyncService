using System;
using System.Data;
using System.Data.SqlClient;

namespace EBoard
{
	public class Reporter
	{
		private SqlConnection connection;
		public Reporter(SqlConnection conn)
		{
			connection = conn;
		}

		public void CreateReport(int month)
		{
			throw new NotImplementedException();
		}

		public DataSet GetCurrentMonthData()
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
}
