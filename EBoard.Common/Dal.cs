using NLog;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;

namespace EBoard.Common
{
	public class Dal
	{
		private readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

		private const string Biogas1ColName = "Biogas1";
		private const string Biogas2ColName = "Biogas2";
		private const string EnergyProduction1ColName = "EnergyProduction1";
		private const string EnergyProduction2ColName = "EnergyProduction2";
		private const string Runtime1ColName = "Runtime1";
		private const string Runtime2ColName = "Runtime2";
		private const string TotalRuntime1ColName = "TotalRuntime1";
		private const string TotalRuntime2ColName = "TotalRuntime2";

		private SqlConnection connection;

		public Dal(SqlConnection conn)
		{
			connection = conn;
		}

		public string GetCurrentShiftId()
		{
			var command = new SqlCommand("sp_GetCurrentShiftId", connection);
			command.CommandType = CommandType.StoredProcedure;
			var sqlParam = command.Parameters.Add("@ShiftId", SqlDbType.UniqueIdentifier);
			sqlParam.Direction = ParameterDirection.Output;
			command.ExecuteNonQuery();

			return sqlParam.Value.ToString();
		}


		public ShiftStatInfo GetProductionData(DateTime? lastUpdate = null)
		{
			var ds = new DataSet();
			var sql = @"Select a.ItemId, a.Val, a.LastUpdate, a.Quality, b.Address From ItemLatestStatus a,MonitorItem b Where a.ItemID=b.ItemId;Select Max(LastUpdate) As LastUpdate From ItemLatestStatus;";
			var adapter = new SqlDataAdapter(sql, connection);
			adapter.Fill(ds, "ItemLatestStatus");

			// Check if the data changed
			var latestTable = ds.Tables["ItemLatestStatus"];

			// No query need if no items in table
			if (latestTable.Rows.Count < 1)
				return null;

			// No need to load data if no data change after 'lastUpdate'
			var newUpdate = (DateTime)ds.Tables[1].Rows[0]["LastUpdate"];
			if (lastUpdate.HasValue && lastUpdate >= newUpdate)
				return null;

			// Get data from ShiftStatMstr table
			var shiftId = GetCurrentShiftId();
			var data = new ShiftStatInfo { LastUpdate = newUpdate };

			sql = string.Format(@"Select CAST(ShiftId AS nvarchar(50)) ShiftId,BeginTime,ActualBeginTime,EndTime,LastLoginId,LastLoginName,LastLoginTime,Status From ShiftStatMstr Where ShiftId=CAST('{0}' as uniqueidentifier)", shiftId);
			new SqlDataAdapter(sql, connection).Fill(ds, "ShiftStatMstr");

			var mstrTable = ds.Tables["ShiftStatMstr"];
			if ((mstrTable == null) || (mstrTable.Rows.Count < 1))
			{
				data.ShiftId = "";
				return data;
			}
						
			UpdateProperties(mstrTable.Rows[0], data);

			// Get data from ShiftStatDet table
			sql = string.Format(@"SELECT Item,SubTotal FROM ShiftStatDet, ShiftStatMstr Where ShiftStatMstr.ShiftId=ShiftStatDet.ShiftId and ShiftStatMstr.ShiftId=CAST('{0}' AS uniqueidentifier)", shiftId);
			new SqlDataAdapter(sql, connection).Fill(ds, "ShiftStatDet");

			var detTable = ds.Tables["ShiftStatDet"];
			data.StatInfo = new Dictionary<string, double>();
			detTable.AsEnumerable().ToList().ForEach(row =>
			{
				try
				{
					data.StatInfo[row["Item"].ToString().ToLower()] = (double)row["SubTotal"];
				}
				catch (Exception ex)
				{
					logger.Error("Cannot convert the value [{0}] of [{1}] to double type from table ProductionStatDet. {2}", row["SubTotal"], row["Item"], ex);
				}
			});

			// Add latest data into list
			data.MonitorItems = new Dictionary<string, double>();
			latestTable.AsEnumerable().ToList().ForEach(row =>
			{
				try
				{
					data.MonitorItems[row["ItemId"].ToString().ToLower()] = (double)row["Val"];
				}
				catch (Exception ex)
				{
					logger.Error("Cannot convert the value [{0}] of [{1}] to double type from table ItemLatestStatus. {2}", row["Val"], row["ItemId"], ex);
				}
			});
			
			return data;
		}

		public IList<string> GetGeneralParamCategory()
		{
			return new List<string>();
		}

		#region For User and Role
		public User GetUser(string loginId)
		{
			return InternalGetUser(string.Format("LoginId='{0}'", loginId));
		}

		public ICollection<User> GetUsers()
		{
			var users = new List<User>();
			var sql = @"SELECT UserId,LoginId,Name,Password,IDCard,Status FROM [User]";

			var ds = new DataSet();
			var adapter = new SqlDataAdapter(sql, connection);
			adapter.Fill(ds);

			if (ds.Tables[0].Rows.Count < 1)
				return users;

			foreach (DataRow row in ds.Tables[0].Rows)
			{
				var user = new User
				{
					UserId = row["UserId"].ToString(),
					LoginId = row["LoginId"].ToString(),
					Name = row["Name"].ToString(),
					Password = row["Password"].ToString(),
					IDCard = row["IDCard"].ToString(),
					Status = row["Status"].ToString()
				};

				users.Add(user);
			}

			return users;
		}

		public User GetUserByIdCard(string idCard)
		{
			return InternalGetUser(string.Format("IDCard='{0}'", idCard));
		}

		private User InternalGetUser(string condition)
		{
			var sql = @"SELECT UserId,LoginId,Name,Password,IDCard,Status FROM [User]";
			if (!string.IsNullOrWhiteSpace(condition))
			{
				sql += string.Format(" WHERE {0}", condition);
			}

			var ds = new DataSet();
			var adapter = new SqlDataAdapter(sql, connection);
			adapter.Fill(ds);

			if (ds.Tables[0].Rows.Count < 1)
				return null;

			var row = ds.Tables[0].Rows[0];
			var user = new User
			{
				UserId = row["UserId"].ToString(),
				LoginId = row["LoginId"].ToString(),
				Name = row["Name"].ToString(),
				Password = row["Password"].ToString(),
				IDCard = row["IDCard"].ToString(),
				Status = row["Status"].ToString()
			};

			user.Roles = new List<string>();
			var roleSql = string.Format(@"SELECT RoleId FROM UserInRole WHERE UserId=CONVERT(uniqueidentifier, '{0}')", user.UserId);
			adapter = new SqlDataAdapter(roleSql, connection);
			adapter.Fill(ds, "RoleList");
			foreach (DataRow r in ds.Tables["RoleList"].Rows)
			{
				user.Roles.Add(r[0].ToString());
			}

			return user;
		}

		public int GetUserCountByName(string name, bool caseInSensitive = true)
		{
			var sql = caseInSensitive ?
				string.Format(@"SELECT COUNT(UserID) FROM [User] WHERE Upper(Name)='{0}'", name.ToUpper()) :
				string.Format(@"SELECT COUNT(UserID) FROM [User] WHERE Name='{0}'", name);

			using (var cmd = connection.CreateCommand())
			{
				cmd.CommandText = sql;
				return (int)cmd.ExecuteScalar();
			}
		}

		public int GetUserCountByLoginId(string loginId)
		{
			var sql = string.Format(@"SELECT COUNT(UserID) FROM [User] WHERE LoginId='{0}'", loginId);

			using (var cmd = connection.CreateCommand())
			{
				cmd.CommandText = sql;
				return (int)cmd.ExecuteScalar();
			}
		}

		public User AddUser(User user)
		{
			if (string.IsNullOrWhiteSpace(user.LoginId) || string.IsNullOrWhiteSpace(user.Name))
				throw new ArgumentNullException("LoginId or Name");

			var count = GetUserCountByLoginId(user.LoginId);
			if (count > 0)
				throw new DuplicateNameException("The LoginId is duplicated.");

			var newId = Guid.NewGuid().ToString();
			var sql = string.Format("INSERT INTO [User] (UserId,LoginId,Name,IDCard,Status) VALUES(CONVERT(uniqueidentifier,'{0}'),'{1}','{2}','{3}','A')",
							newId, user.LoginId, user.Name, user.IDCard ?? "");

			using (var cmd = connection.CreateCommand())
			{
				cmd.CommandText = sql;
				cmd.ExecuteNonQuery();
			}

			user.UserId = newId;

			return user;
		}

		public bool DeleteUser(User user)
		{
			using (var trans = connection.BeginTransaction())
			{
				using (var cmd = connection.CreateCommand())
				{
					var sql = string.Format("DELETE FROM UserInRole WHERE UserId=CONVERT(uniqueidentifier,'{0}')", user.UserId);
					cmd.CommandText = sql;
					cmd.ExecuteNonQuery();

					sql = string.Format("DELETE FROM [User] WHERE UserId=CONVERT(uniqueidentifier,'{0}')", user.UserId);
					cmd.CommandText = sql;
					var ret = (cmd.ExecuteNonQuery() == 0);

					trans.Commit();

					return ret;
				}
			}
		}

		public User UpdateUser(User user)
		{
			// User dataset for updating User since dataset is able to check if the data is changed or not
			var sql = string.Format(@"SELECT * FROM [User] WHERE UserId=CONVERT(uniqueidentifier,'{0}')", user.UserId);
			var ds = new DataSet();
			var adapter = new SqlDataAdapter();
			adapter.SelectCommand = new SqlCommand(sql, connection);
			var scb = new SqlCommandBuilder(adapter);

			adapter.Fill(ds);
			if (ds.Tables[0].Rows.Count < 1)
				throw new DllNotFoundException();

			var columns = ds.Tables[0].Columns;
			var row = ds.Tables[0].Rows[0];
			
			var properties = typeof(User).GetProperties()
										.Where(p => p.CanRead && !(p.PropertyType.IsGenericType));

			foreach (var prop in properties)
			{
				if (!columns.Contains(prop.Name))
					continue;

				row[prop.Name] = prop.GetValue(user);
			}

			adapter.Update(ds);

			return user;
		}
		#endregion

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
