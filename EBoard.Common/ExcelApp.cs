using System;
using System.Data;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using Excel = Microsoft.Office.Interop.Excel;

namespace EBoard.Common
{
	/// <summary>
	/// Need to call Dispose() after used this class to avoid the memory leak
	/// </summary>
	public class ExcelApp : IDisposable
	{
		private static readonly object Nothing = Missing.Value;

		private Excel.Application app;

		private Excel.Workbooks workbooks;

		public ExcelApp()
		{
			app = new Excel.Application { Visible = false };
			workbooks = app.Workbooks;
		}


		/// <summary>  
		/// Creates an Excel file
		/// </summary>  
		/// <param name="fileName"></param>  
		public Excel.Workbook CreateExcel(string fileName)
		{
			var workbook = workbooks.Add(Nothing);
			workbook.SaveAs(fileName, Nothing, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Excel.XlSaveAsAccessMode.xlNoChange, Type.Missing, Type.Missing, Type.Missing);
			return workbook;
		}

		public Excel.Workbook CreateOrOpenExcel(string fileName)
		{
			var fileExist = File.Exists(fileName);
			return fileExist ? OpenExcel(fileName) : CreateExcel(fileName);
		}

		/// <summary>  
		/// Opens an Excel file
		/// </summary>  
		/// <param name="filename">file name</param>  
		public Excel.Workbook OpenExcel(string filename)
		{
			return workbooks.Open(filename, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing, Nothing);
		}

		public Excel.Worksheet CreateWorksheet(Excel.Workbook workbook, string sheetName)
		{
			if ((workbook == null) || string.IsNullOrWhiteSpace(sheetName))
				throw new ArgumentNullException();

			var worksheet = workbook.Sheets.Add(Nothing, workbook.Sheets[workbook.Sheets.Count], 1, Excel.XlSheetType.xlWorksheet);
			worksheet.Name = sheetName;

			return worksheet;
		}

		public Excel.Worksheet CreateOrOpenWorksheet(Excel.Workbook workbook, string sheetName)
		{
			if ((workbook == null) || string.IsNullOrWhiteSpace(sheetName))
				throw new ArgumentNullException();

			foreach (dynamic worksheet in workbook.Worksheets)
			{
				if (string.Equals(worksheet.Name, sheetName, StringComparison.OrdinalIgnoreCase))
				{
					return worksheet;
				}
			}

			// Create since not found
			return CreateWorksheet(workbook, sheetName);
		}

		/// <summary>
		/// Writes the whole table data into a worksheet including the header
		/// </summary>
		/// <param name="worksheet"></param>
		/// <param name="table"></param>
		/// <param name="clearAllFirst">Indicates if to clear all data before writing the data</param>
		/// <param name="startRowForHeader">The number of row to write the header</param>
		/// <returns>Count of records wrote except for the header</returns>
		public int WriteData(Excel.Worksheet worksheet, DataTable table, bool clearAllFirst = true, int startRowForHeader = 1)
		{
			if ((worksheet == null) || table == null)
				throw new ArgumentNullException();

			if (clearAllFirst)
			{
				worksheet.UsedRange.Columns.Clear();
			}

			for (var i = 0; i < table.Columns.Count; i++)
			{
				var colNum = i + 1;
				var column = table.Columns[i];
				worksheet.Cells[startRowForHeader, colNum] = column.ColumnName;

				for (var row = 0; row < table.Rows.Count; row++)
				{
					var val = table.Rows[row][i];
					var type = val.GetType();
					if (type != typeof(DBNull))
					{
						if (type != typeof(Guid))
						{
							worksheet.Cells[row + startRowForHeader + 1, colNum] = val;
						}
						else
						{
							worksheet.Cells[row + startRowForHeader + 1, colNum] = val.ToString();
						}
					}
				}
			}
						
			return table.Rows.Count;
		}


		public void Close()
		{
			Cleanup();
		}

		private void Cleanup()
		{
			if (workbooks != null)
			{
				foreach (dynamic workbook in workbooks)
				{
					workbook.Close(false, Type.Missing, Type.Missing);
				}
			}
			
			app?.Quit();
		}

		#region IDisposable Support
		private bool disposedValue; // To detect redundant calls

		protected virtual void Dispose(bool disposing)
		{
			if (!disposedValue)
			{
				if (disposing)
				{
					Cleanup();
				}
				
				if (workbooks != null)
				{
					Marshal.FinalReleaseComObject(workbooks);
					workbooks = null;
				}

				if (app != null)
				{
					Marshal.FinalReleaseComObject(app);
					app = null;
				}

				GC.Collect();
				GC.WaitForPendingFinalizers();

				disposedValue = true;
			}
		}

		// TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
		// ~ExcelApp() {
		//   // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
		//   Dispose(false);
		// }

		// This code added to correctly implement the disposable pattern.
		public void Dispose()
		{
			// Do not change this code. Put cleanup code in Dispose(bool disposing) above.
			Dispose(true);
			// TODO: uncomment the following line if the finalizer is overridden above.
			// GC.SuppressFinalize(this);
		}
		#endregion
	}
}