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

		/// <summary>
		/// Gets the ID of current shift.
		/// Will automatically create the current shift is not created in db yet
		/// </summary>
		/// <returns></returns>
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
		/// Gets the workers of current shift
		/// Note: The current shift will automatically created if it not exist yet
		/// </summary>
		/// <returns></returns>
		public SortedList<DateTime, User> GetWorkersOfShift(string shiftId = null)
		{
			var userList = new SortedList<DateTime, User>();
			if (string.IsNullOrWhiteSpace(shiftId))
				shiftId = GetCurrentShiftId();

			var sql = string.Format(@"Select LoginId,LoginName,LoginTime From WorkersInShift Where ShiftId=CAST('{0}' as uniqueidentifier)", shiftId);
			using (var command = new SqlCommand(sql, connection))
			{
				using (var reader = command.ExecuteReader())
				{
					while (reader.Read())
					{
						var user = new User
						{
							LoginId = reader.GetString(0),
							Name = reader.GetString(1)
						};

						userList.Add(reader.GetDateTime(2), user);
					}

					reader.Close();
				}
			}

			return userList;
		}

		/// <summary>
		/// Adds a new worker in shift
		/// </summary>
		/// <param name="shiftId"></param>
		/// <param name="worker"></param>
		/// <returns></returns>
		public bool AddWorkerInShift(string shiftId, User worker)
		{
			var sql = string.Format(@"Insert Into WorkersInShift (ShiftId,LoginId,LoginName,LoginTime) Values(CAST('{0}' As uniqueidentifier),'{1}','{2}',GetDate())", shiftId, worker.LoginId, worker.Name);
			using (var command = new SqlCommand(sql, connection))
			{
				return (command.ExecuteNonQuery() > 0);
			}
		}

		/// <summary>
		/// Updates the worker of a shift with new one
		/// </summary>
		/// <param name="shiftId"></param>
		/// <param name="oldLoginId"></param>
		/// <param name="newWorker"></param>
		/// <returns></returns>
		public bool UpdateWorkerInShift(string shiftId, string oldLoginId, User newWorker)
		{
			var sql = string.Format(@"Update WorkersInShift Set LoginId='{0}',LoginName='{1}',LoginTime=GetDate() Where ShiftId=CAST('{2}' AS uniqueidentifier) And LoginId='{3}'", newWorker.LoginId, newWorker.Name, shiftId, oldLoginId);
			using (var command = new SqlCommand(sql, connection))
			{
				return (command.ExecuteNonQuery() > 0);
			}
		}

		/// <summary>
		/// Gets the stat info of current shift
		/// </summary>
		/// <param name="lastUpdate">Do not get the data which is elder than lastUpdate</param>
		/// <returns></returns>
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

			// TODO: Check a special flag to see if the communication break and throw exception
			//throw new OPCCommunicationBrokeException();

			// Get data from ShiftStatMstr table
			var shiftId = GetCurrentShiftId();

			sql = string.Format(@"Select CAST(ShiftId AS nvarchar(50)) ShiftId,BeginTime,ActualBeginTime,LastUpdateTime,EndTime,Status From ShiftStatMstr Where ShiftId=CAST('{0}' as uniqueidentifier)", shiftId);
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
					logger.Error("Cannot convert the value [{0}] of [{1}] to double type from table ShiftStatDet. {2}", row["SubTotal"], row["Item"], ex);
				}
			});

			// Get total run time of generator from ShiftStatDet table, no need to subtract
			sql = string.Format(@"SELECT Item,IsNull(det.SubTotalLast,0.0) as SubTotalLast FROM ShiftStatDet det, ShiftStatMstr mstr Where mstr.ShiftId=det.ShiftId and mstr.ShiftId=CAST('{0}' AS uniqueidentifier) And Item In ('{1}','{2}')", shiftId, ShiftStatInfo.SubtotalRuntime1ColName, ShiftStatInfo.SubtotalRuntime2ColName);
			new SqlDataAdapter(sql, connection).Fill(ds, "TotalRunTime");
			var totalRuntimeTable = ds.Tables["TotalRunTime"];
			totalRuntimeTable.AsEnumerable().ToList().ForEach(row =>
			{
				try
				{
					data.StatInfo[row["Item"].ToString().ToLower().Substring(3)] = (double)row["SubTotalLast"];
				}
				catch (Exception ex)
				{
					logger.Error("Cannot convert the value [{0}] of [{1}] to double type from table ShiftStatDet. {2}", row["SubTotalLast"], row["Item"], ex);
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

		#region For User and Role
		/// <summary>
		/// Gets the user against the login id
		/// </summary>
		/// <param name="loginId"></param>
		/// <returns></returns>
		public User GetUser(string loginId)
		{
			return InternalGetUser(string.Format("LoginId ='{0}'", loginId));
		}

		/// <summary>
		/// Gets user list
		/// </summary>
		/// <returns></returns>
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

		/// <summary>
		/// Gets user according to the ID card
		/// </summary>
		/// <param name="idCard"></param>
		/// <returns></returns>
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

			return user;
		}

		/// <summary>
		/// Get user according to the name
		/// </summary>
		/// <param name="name"></param>
		/// <param name="caseInSensitive"></param>
		/// <returns></returns>
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

		/// <summary>
		/// Adds a new user
		/// </summary>
		/// <param name="user"></param>
		/// <returns></returns>
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

		/// <summary>
		/// Deltes a user
		/// </summary>
		/// <param name="user"></param>
		/// <returns></returns>
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

		/// <summary>
		/// Updates user info
		/// </summary>
		/// <param name="user"></param>
		/// <returns></returns>
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

		/// <summary>
		/// Gets all roles
		/// </summary>
		/// <returns></returns>
		public IList<Role> GetRoles()
		{
			var roles = new List<Role>();

			var sql = @"Select * from Role";
			var adapter = new SqlDataAdapter(sql, connection);
			var ds = new DataSet();
			adapter.Fill(ds);

			if (ds.Tables[0].Rows.Count < 1)
				return roles;

			foreach (DataRow row in ds.Tables[0].Rows)
			{
				var role = new Role
				{
					RoleId = row["RoleId"].ToString(),
					Name = row["Name"].ToString(),
					Status = row["Status"].ToString()
				};

				roles.Add(role);
			}

			return roles;
		}

		public bool AddRole(string roleId, string userId)
		{
			if (string.IsNullOrWhiteSpace(roleId) || string.IsNullOrWhiteSpace(userId))
				throw new ArgumentNullException();

			var sql = string.Format("Select Count(*) From UserInRole Where RoleId='{0}' And UserId='{1}'", roleId, userId);
			var cmd = new SqlCommand(sql, connection);
			var count = (int)cmd.ExecuteScalar();
			if (count > 0)
				throw new Exception(string.Format("用户已经属于该角色-'{0}'", roleId));

			sql = string.Format("Insert Into UserInRole (RoleId,UserId) Values ('{0}','{1}')", roleId, userId);
			cmd.CommandText = sql;
			return (cmd.ExecuteNonQuery() > 0);
		}

		public bool DeleteRole(string roleId, string userId)
		{
			if (string.IsNullOrWhiteSpace(roleId) || string.IsNullOrWhiteSpace(userId))
				throw new ArgumentNullException();

			var sql = string.Format("Delete From UserInRole Where RoleId='{0}' And UserId=CAST('{1}' AS uniqueidentifier)", roleId, userId);
			var cmd = new SqlCommand(sql, connection);
			return (cmd.ExecuteNonQuery() > 0);
		}

		/// <summary>
		/// Gets roles of a user belongs to
		/// </summary>
		/// <param name="user"></param>
		public IList<Role> GetUserRoles(User user)
		{
			if ((user == null) || string.IsNullOrWhiteSpace(user.UserId) || string.IsNullOrWhiteSpace(user.LoginId))
				throw new ArgumentNullException();

			if (string.IsNullOrWhiteSpace(user.UserId))
			{
				var realUser = GetUser(user.LoginId);
				if (realUser == null)
					throw new UserNotFoundException(string.Format("Cannot found the user with login ID '{0}'.", user.LoginId));

				user.UserId = realUser.UserId;
			}

			user.Roles = new List<Role>();
			var sql = string.Format(@"Select Role.RoleId,Name From UserInRole, Role Where UserInRole.RoleId=Role.RoleId And Status='A' And UserId=CAST('{0}' As uniqueidentifier)", user.UserId);
			var adapter = new SqlDataAdapter(sql, connection);
			var ds = new DataSet();
			adapter.Fill(ds);

			if (ds.Tables[0].Rows.Count < 1)
				return user.Roles;

			foreach (DataRow row in ds.Tables[0].Rows)
			{
				var role = new Role
				{
					RoleId = row["RoleId"].ToString(),
					Name = row["Name"].ToString(),
					Status = "A"
				};

				user.Roles.Add(role);
			}

			return user.Roles;
		}

		/// <summary>
		/// Gets users maping with a role
		/// </summary>
		/// <param name="role"></param>
		/// <returns></returns>
		public IList<User> GetUserRoles(Role role)
		{
			if ((role == null) || string.IsNullOrWhiteSpace(role.RoleId))
				throw new ArgumentNullException();

			role.Users = new List<User>();
			var sql = string.Format(@"Select [User].* From UserInRole,[User],Role Where UserInRole.UserId=[User].UserId And UserInRole.RoleId=[Role].RoleId And [Role].Status='A' And Role.RoleId=CAST('{0}' As uniqueidentifier)", role.RoleId);
			var adapter = new SqlDataAdapter(sql, connection);
			var ds = new DataSet();
			adapter.Fill(ds);

			if (ds.Tables[0].Rows.Count < 1)
				return role.Users;

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

				role.Users.Add(user);
			}

			return role.Users;
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

	public class OPCCommunicationBrokeException : Exception
	{
	}

	public class UserNotFoundException :Exception
	{
		public UserNotFoundException(string msg) : base(msg) { }
	}
}
