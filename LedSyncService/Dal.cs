using NLog;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;

namespace LedSyncService
{
	public class Dal
	{
		private readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

		private SqlConnection connection;

		public Dal(SqlConnection conn)
		{
			connection = conn;
		}

		public StatData GetStatData()
		{
			var data = new StatData();

			// temp
			var random = new Random(DateTime.Now.Second);
			data.CurrentBiogasUsed = random.Next();
			data.TotalBiogasUsed = data.CurrentBiogasUsed * 2;
			data.CurrentEnergyGenerated = random.Next();
			data.TotalEnergyGenerated = data.CurrentEnergyGenerated * 2;

			//var shiftId = GetCurrentShiftId();

			//var sql = string.Format(@"Select IsNull(Sum(IsNull(SubTotalBegin, 0)), 0) As Start, IsNull(Sum(IsNull(SubTotalLast, 0)), 0) As Total From ShiftStatDet Where ShiftId =Cast('{0}' As uniqueidentifier) And Item In ('EnergyProduction1', 'EnergyProduction2')", shiftId);
			//var cmd = new SqlCommand(sql, connection);
			//using (var reader = cmd.ExecuteReader())
			//{
			//	if (!reader.Read())
			//	{
			//		logger.Error("No result return for the sql [{0}]", sql);
			//		return null;
			//	}
				
			//	data.TotalEnergyGenerated = float.Parse(reader.GetValue(1).ToString());		// The data type get from database might be double, cannot directly convert it to float unless (float)(double)reader.GetValue(1)
			//	data.CurrentEnergyGenerated = data.TotalEnergyGenerated - float.Parse(reader.GetValue(0).ToString());

			//	reader.Close();
			//}

			//sql = string.Format(@"Select IsNull(Sum(IsNull(SubTotalBegin, 0)), 0) As Start, IsNull(Sum(IsNull(SubTotalLast, 0)), 0) As Total From ShiftStatDet Where ShiftId =Cast('{0}' As uniqueidentifier) And Item In ('Biogas2GenSubtotal','Biogas2TorchSubtotal')", shiftId);
			//cmd = new SqlCommand(sql, connection);
			//using (var reader = cmd.ExecuteReader())
			//{
			//	if (!reader.Read())
			//	{
			//		logger.Error("No result return for the sql [{0}]", sql);
			//		return null;
			//	}

			//	data.TotalBiogasUsed = float.Parse(reader.GetValue(1).ToString());
			//	data.CurrentBiogasUsed = data.TotalBiogasUsed - float.Parse(reader.GetValue(0).ToString());

			//	reader.Close();
			//}

			return data;
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
		/// Gets the ID of current shift.
		/// Will automatically create the current shift is not created in db yet
		/// </summary>
		/// <returns></returns>
		private string GetCurrentShiftId()
		{
			var command = new SqlCommand("sp_GetCurrentShiftId", connection);
			command.CommandType = CommandType.StoredProcedure;
			var sqlParam = command.Parameters.Add("@ShiftId", SqlDbType.UniqueIdentifier);
			sqlParam.Direction = ParameterDirection.Output;
			command.ExecuteNonQuery();

			return sqlParam.Value.ToString();
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
