using System;
using System.IO;
using System.Security.Cryptography;

namespace EBoard.Common
{
	public class Encryptor
	{
		private readonly byte[] key = {0x01, 0x08, 0x06, 0x00, 0x01, 0x06, 0x00, 0x02, 0x00, 0x06, 0x00, 0x06, 0x02, 0x07, 0x03, 0x16};

		private readonly byte[] iv = {0x06, 0x02, 0x07, 0x03, 0x01, 0x08, 0x06, 0x00, 0x01, 0x06, 0x00, 0x02, 0x00, 0x06, 0x00, 0x16};

		public string Encrypt(string plainText)
		{
			if (string.IsNullOrEmpty(plainText))
				return string.Empty;
			
			using (var rmCryptor = new RijndaelManaged())
			using (var ms = new MemoryStream())
			using (var cryptStream = new CryptoStream(ms, rmCryptor.CreateEncryptor(key, iv), CryptoStreamMode.Write))
			using (var sw = new StreamWriter(cryptStream))
			{
				sw.Write(plainText);
				sw.Flush();
				cryptStream.FlushFinalBlock();
				ms.Flush();
				
				return Convert.ToBase64String(ms.ToArray());
			}
		}

		public string Decrypt(string encryptedText)
		{
			if (string.IsNullOrEmpty(encryptedText))
				return string.Empty;

			using (var rmCryptor = new RijndaelManaged())
			using (var ms = new MemoryStream(Convert.FromBase64String(encryptedText)))
			using (var cryptStream = new CryptoStream(ms, rmCryptor.CreateDecryptor(key, iv), CryptoStreamMode.Read))
			using (var sr = new StreamReader(cryptStream))
			{
				return sr.ReadToEnd();
			}
		}
	}
}
