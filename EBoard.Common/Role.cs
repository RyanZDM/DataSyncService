using System.Collections.Generic;

namespace EBoard.Common
{
	public class Role
	{
		public string RoleId { get; set; }
		
		public string Name { get; set; }

		public ICollection<string> Users { get; set; }
	}
}
