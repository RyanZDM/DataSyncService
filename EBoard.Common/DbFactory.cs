using System.Data.Common;
using System.Data.SqlClient;

namespace EBoard.Common
{
	public class DbFactory
	{
		private const string ConnectionString =
			@"Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=OPC;Data Source=.";

		public static SqlConnection GetConnection()
		{
			var conn = new SqlConnection(ConnectionString);
			conn.Open();
			return conn;
		}
	}
}
