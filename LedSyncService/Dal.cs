//using NLog;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;

namespace LedSyncService
{
	public class Dal
	{
		//private readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

		private SqlConnection connection;

		public Dal(SqlConnection conn)
		{
			connection = conn;
		}

		/// <summary>
		/// Gets all parameters from GeneratlParameters table
		/// </summary>
		/// <returns></returns>
		public IList<GeneralParamter> GetGeneralParameters()
		{
			var sql = @"SELECT Category,Name,Value,DispOrder,DispName,Memo,Hide,IsEncrypted,IsProtected FROM GeneralParams";
			var ds = new DataSet();
			var parameters = new List<GeneralParamter>();
			var adapter = new SqlDataAdapter(sql, connection);
			adapter.Fill(ds);
			foreach (DataRow row in ds.Tables[0].Rows)
			{
				var param = new GeneralParamter();
				UpdateProperties(row, param);
				parameters.Add(param);
			}

			return parameters;
		}

		/// <summary>
		/// Automatically sets the value of each column in row to target obj
		/// if there is a property of target obj which name is also belongs to row columns
		/// </summary>
		/// <param name="row"></param>
		/// <param name="obj"></param>
		/// <returns></returns>
		private int UpdateProperties(DataRow row, object obj)
		{
			if (row == null || obj == null)
				return 0;

			// Cannot get column info if the Table is null
			var table = row.Table;
			if (table == null)
				return 0;

			var columns = table.Columns;
			var properties = obj.GetType().GetProperties()
										.Where(p => p.CanWrite && !(p.PropertyType.IsGenericType));

			var count = 0;
			foreach (var prop in properties)
			{
				if (!columns.Contains(prop.Name))
					continue;

				var val = row[prop.Name];
				{
					if (val.GetType() == typeof(DBNull))
					{
						var propType = prop.PropertyType;
						val = propType.IsValueType ? Activator.CreateInstance(propType) : null;
					}
				}

				prop.SetValue(obj, val);
				count++;
			}

			return count;
		}
	}	
}
