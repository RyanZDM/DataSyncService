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

		/// <summary>
		/// Gets the user of current shift
		/// Note: The current shift will automatically created if it not exist yet
		/// </summary>
		/// <returns></returns>
		public User GetUserOfCurrentShift()
		{
			User user = null;
			var shiftId = GetCurrentShiftId();
			var sql = string.Format(@"Select IsNull(LastLoginId,''),IsNull(LastLoginName,'') From ShiftStatMstr Where ShiftId=CAST('{0}' as uniqueidentifier)", shiftId);
			using (var command = new SqlCommand(sql, connection))
			{
				using (var reader = command.ExecuteReader())
				{
					if (reader.Read())
					{
						user = new User
						{
							LoginId = reader.GetString(0),
							Name = reader.GetString(1)
						};
					}
				}
			}

			return user;
		}

		public bool SetUserOfCurrentShift(User user)
		{
			var shiftId = GetCurrentShiftId();
			var sql = string.Format(@"Update ShiftStatMstr Set LastLoginId='{0}',LastLoginName='{1}',LastLoginTime=IsNull(LastLoginTime,GetDate()) Where ShiftId=CAST('{2}' AS uniqueidentifier)", user.LoginId, user.Name, shiftId);
			using (var command = new SqlCommand(sql, connection))
			{
				return (command.ExecuteNonQuery() > 0);
			}
		}

		public ShiftStatInfo GetShiftStatInfo(DateTime? lastUpdate = null)
		{
			var ds = new DataSet();
			var sql = @"Select a.ItemId, a.Val, a.LastUpdate, a.Quality, b.Address From ItemLatestStatus a,MonitorItem b Where a.ItemID=b.ItemId";
			var adapter = new SqlDataAdapter(sql, connection);
			adapter.Fill(ds, "ItemLatestStatus");

			// Check if the data changed
			var latestTable = ds.Tables["ItemLatestStatus"];

			// No query need if no items in table
			if (latestTable.Rows.Count < 1)
				return null;


			// Get data from ShiftStatMstr table
			var shiftId = GetCurrentShiftId();

			sql = string.Format(@"Select CAST(ShiftId AS nvarchar(50)) ShiftId,BeginTime,ActualBeginTime,LastUpdateTime,EndTime,LastLoginId,LastLoginName,LastLoginTime,Status From ShiftStatMstr Where ShiftId=CAST('{0}' as uniqueidentifier)", shiftId);
			new SqlDataAdapter(sql, connection).Fill(ds, "ShiftStatMstr");

			var mstrTable = ds.Tables["ShiftStatMstr"];
			if ((mstrTable == null) || (mstrTable.Rows.Count < 1))
				return null;

			// No need to load data if no data change after 'lastUpdate'
			if (lastUpdate.HasValue)
			{
				var newUpdate = (mstrTable.Rows[0]["LastUpdateTime"].GetType() != typeof(DBNull)) ? (DateTime)mstrTable.Rows[0]["LastUpdateTime"] : DateTime.MinValue;
				if (lastUpdate >= newUpdate)
					return null;
			}

			// Get data from ShiftStatMstr table
			var data = new ShiftStatInfo();
			UpdateProperties(mstrTable.Rows[0], data);

			// Get data from ShiftStatDet table
			sql = string.Format(@"SELECT Item,IsNull(det.SubTotalLast,0.0) - IsNull(det.SubTotalBegin,0.0) as SubTotal FROM ShiftStatDet det, ShiftStatMstr mstr Where mstr.ShiftId=det.ShiftId and mstr.ShiftId=CAST('{0}' AS uniqueidentifier)", shiftId);
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

			// Get data from ItemLatestStatus table
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
