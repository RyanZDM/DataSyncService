using System.Data.Common;
using System.Data.SqlClient;
using System.Configuration;

namespace EBoard.Common
{
	public class DbFactory
	{
		private const string DefaultConnectionString =
			@"Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=OPC;Data Source=.";

		public static string ConnectionString
		{
			get { return ConfigurationManager.AppSettings["ConnectionString"] ?? DefaultConnectionString; }
		}

		public static SqlConnection GetConnection()
		{
			var conn = new SqlConnection(ConnectionString);
			conn.Open();
			return conn;
		}
	}
}
