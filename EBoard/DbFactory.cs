﻿using System.Data.Common;
using System.Data.SqlClient;

namespace EBoard
{
	public class DbFactory
	{
		private const string ConnectionString =
			@"Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=OPC;Data Source=.\carestream";

		public static SqlConnection GetConnection()
		{
			var conn = new SqlConnection(ConnectionString);
			conn.Open();
			return conn;
		}
	}
}
