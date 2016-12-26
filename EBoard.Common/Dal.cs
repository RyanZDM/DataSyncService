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
		private readonly Logger logger = LogManager.GetCurrentClassLogger();

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

			var sql =
				$@"Select LoginId,LoginName,LoginTime From WorkersInShift Where ShiftId=CAST('{shiftId}' as uniqueidentifier)";
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
			var sql =
				$@"Insert Into WorkersInShift (ShiftId,LoginId,LoginName,LoginTime) Values(CAST('{shiftId}' As uniqueidentifier),'{worker
					.LoginId}','{worker.Name}',GetDate())";
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
			var sql =
				$@"Update WorkersInShift Set LoginId='{newWorker.LoginId}',LoginName='{newWorker.Name}',LoginTime=GetDate() Where ShiftId=CAST('{shiftId}' AS uniqueidentifier) And LoginId='{oldLoginId}'";
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
			var sql = @"Select a.ItemId, CAST(a.Val As int) as Val, a.LastUpdate, a.Quality, b.Address From ItemLatestStatus a,MonitorItem b Where a.ItemID=b.ItemId";
			var adapter = new SqlDataAdapter(sql, connection);
			adapter.Fill(ds, "ItemLatestStatus");

			// Check if the data changed
			var latestTable = ds.Tables["ItemLatestStatus"];

			// No query need if no items in table
			if (latestTable.Rows.Count < 1)
				return null;

			// Get data from ShiftStatMstr table
			var shiftId = GetCurrentShiftId();

			sql =
				$@"Select CAST(ShiftId AS nvarchar(50)) ShiftId,BeginTime,ActualBeginTime,LastUpdateTime,EndTime,Status From ShiftStatMstr Where ShiftId=CAST('{shiftId}' as uniqueidentifier)";
			new SqlDataAdapter(sql, connection).Fill(ds, "ShiftStatMstr");

			var mstrTable = ds.Tables["ShiftStatMstr"];
			if ((mstrTable == null) || (mstrTable.Rows.Count < 1))
				return null;

			//// No need to load data if no data change after 'lastUpdate'
			//if (lastUpdate.HasValue)
			//{
			//	var newUpdate = (mstrTable.Rows[0]["LastUpdateTime"].GetType() != typeof(DBNull)) ? (DateTime)mstrTable.Rows[0]["LastUpdateTime"] : DateTime.MinValue;
			//	if (lastUpdate >= newUpdate)
			//	{
			//		logger.Info("No data change at time {0}, last={1].", lastUpdate.Value.ToLongTimeString(), newUpdate.ToLongTimeString());
			//		return null;
			//	}
			//}

			// Get data from ShiftStatMstr table
			var data = new ShiftStatInfo();
			UpdateProperties(mstrTable.Rows[0], data);

			// Get data from ShiftStatDet table
			sql =
				$@"SELECT Item, (IsNull(det.SubTotalLast,0) - IsNull(det.SubTotalBegin,0)) as SubTotal FROM ShiftStatDet det, ShiftStatMstr mstr Where mstr.ShiftId=det.ShiftId and mstr.ShiftId=CAST('{shiftId}' AS uniqueidentifier)";
			new SqlDataAdapter(sql, connection).Fill(ds, "ShiftStatDet");

			var detTable = ds.Tables["ShiftStatDet"];
			data.StatInfo = new Dictionary<string, int>();
			detTable.AsEnumerable().ToList().ForEach(row =>
			{
				try
				{
					data.StatInfo[row["Item"].ToString().ToLower()] = (int)row["SubTotal"];
				}
				catch (Exception ex)
				{
					logger.Error("Cannot convert the value [{0}] of [{1}] to int type from table ShiftStatDet. {2}", row["SubTotal"], row["Item"], ex);
				}
			});

			// Get total run time of generator from ShiftStatDet table, no need to subtract
			sql =
				$@"SELECT Item,IsNull(det.SubTotalLast,0) As SubTotalLast FROM ShiftStatDet det, ShiftStatMstr mstr Where mstr.ShiftId=det.ShiftId and mstr.ShiftId=CAST('{shiftId}' AS uniqueidentifier) And Item In ('{ShiftStatInfo
					.SubtotalRuntime1ColName}','{ShiftStatInfo.SubtotalRuntime2ColName}')";
			new SqlDataAdapter(sql, connection).Fill(ds, "TotalRunTime");
			var totalRuntimeTable = ds.Tables["TotalRunTime"];
			totalRuntimeTable.AsEnumerable().ToList().ForEach(row =>
			{
				try
				{
					data.StatInfo[row["Item"].ToString().ToLower().Substring(3)] = (int)row["SubTotalLast"];
				}
				catch (Exception ex)
				{
					logger.Error("Cannot convert the value [{0}] of [{1}] to int type from table ShiftStatDet. {2}", row["SubTotalLast"], row["Item"], ex);
				}
			});

			// Get data from ItemLatestStatus table
			data.MonitorItems = new Dictionary<string, int>();
			latestTable.AsEnumerable().ToList().ForEach(row =>
			{
				try
				{
					data.MonitorItems[row["ItemId"].ToString().ToLower()] = (int)row["Val"];
				}
				catch (Exception ex)
				{
					logger.Error("Cannot convert the value [{0}] of [{1}] to int type from table ItemLatestStatus. {2}", row["Val"], row["ItemId"], ex);
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
			return InternalGetUser($"LoginId ='{loginId}'");
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
			return InternalGetUser($"IDCard='{idCard}'");
		}

		private User InternalGetUser(string condition)
		{
			var sql = @"SELECT UserId,LoginId,Name,Password,IDCard,Status,IsProtected FROM [User]";
			if (!string.IsNullOrWhiteSpace(condition))
			{
				sql += $" WHERE {condition}";
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
				IsProtected = (bool)row["IsProtected"],
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
				$@"SELECT COUNT(UserID) FROM [User] WHERE Upper(Name)='{name.ToUpper()}'"
				: $@"SELECT COUNT(UserID) FROM [User] WHERE Name='{name}'";

			using (var cmd = connection.CreateCommand())
			{
				cmd.CommandText = sql;
				return (int)cmd.ExecuteScalar();
			}
		}


		public int GetUserCountByLoginId(string loginId)
		{
			var sql = $@"SELECT COUNT(UserID) FROM [User] WHERE LoginId='{loginId}'";

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
			var sql =
				$"INSERT INTO [User] (UserId,LoginId,Name,IDCard,Status) VALUES(CONVERT(uniqueidentifier,'{newId}'),'{user.LoginId}','{user.Name}','{user.IDCard ?? ""}','A')";

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
					var sql = $"DELETE FROM UserInRole WHERE UserId=CONVERT(uniqueidentifier,'{user.UserId}')";
					cmd.CommandText = sql;
					cmd.ExecuteNonQuery();

					sql = $"DELETE FROM [User] WHERE UserId=CONVERT(uniqueidentifier,'{user.UserId}')";
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
			var sql = $@"SELECT * FROM [User] WHERE UserId=CONVERT(uniqueidentifier,'{user.UserId}')";
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

			var sql = $"Select Count(*) From UserInRole Where RoleId='{roleId}' And UserId='{userId}'";
			var cmd = new SqlCommand(sql, connection);
			var count = (int)cmd.ExecuteScalar();
			if (count > 0)
				throw new Exception($"用户已经属于该角色-'{roleId}'");

			sql = $"Insert Into UserInRole (RoleId,UserId) Values ('{roleId}','{userId}')";
			cmd.CommandText = sql;
			return (cmd.ExecuteNonQuery() > 0);
		}

		public bool DeleteRole(string roleId, string userId)
		{
			if (string.IsNullOrWhiteSpace(roleId) || string.IsNullOrWhiteSpace(userId))
				throw new ArgumentNullException();

			var sql = (roleId == "*") ? $"Delete From UserInRole Where UserId=CAST('{userId}' AS uniqueidentifier)"
				: $"Delete From UserInRole Where RoleId='{roleId}' And UserId=CAST('{userId}' AS uniqueidentifier)";
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
					throw new UserNotFoundException($"Cannot found the user with login ID '{user.LoginId}'.");

				user.UserId = realUser.UserId;
			}

			user.Roles = new List<Role>();
			var sql =
				$@"Select Role.RoleId,Name From UserInRole, Role Where UserInRole.RoleId=Role.RoleId And Status='A' And UserId=CAST('{user
					.UserId}' As uniqueidentifier)";
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
			var sql =
				$@"Select [User].* From UserInRole,[User],Role Where UserInRole.UserId=[User].UserId And UserInRole.RoleId=[Role].RoleId And [Role].Status='A' And Role.RoleId=CAST('{role
					.RoleId}' As uniqueidentifier)";
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
					if (val is DBNull)
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

	public class OpcCommunicationBrokeException : Exception
	{
	}

	public class UserNotFoundException :Exception
	{
		public UserNotFoundException(string msg) : base(msg) { }
	}
}
