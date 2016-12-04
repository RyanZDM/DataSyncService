using System.Collections.Generic;

namespace EBoard.Common
{
	public class User
	{
		public string UserId { get; set; }

		public string LoginId { get; set; }

		public string Name { get; set; }

		/// <summary>
		/// Encrypted password in db
		/// </summary>
		public string Password { get; set; }

		public string IDCard { get; set; }

		public string Status { get; set; }

		public IList<Role> Roles { get; set; }
	}
}
