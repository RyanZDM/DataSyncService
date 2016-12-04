using System.Collections.Generic;

namespace EBoard.Common
{
	public class Role
	{
		public string RoleId { get; set; }
		
		public string Name { get; set; }

		public string Status { get; set; }

		public IList<User> Users { get; set; }
	}
}
