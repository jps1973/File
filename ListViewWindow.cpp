// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
static HWND g_hWndListView;
static int g_nColumnCount;
static LPTSTR g_lpszParentFolderPath;

BOOL IsListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndListView );

} // End of function IsListViewWindow

int ListViewWindowAddColumn( LPCTSTR lpszTitle, int nWidth )
{
	int nResult = -1;

	LVCOLUMN lvColumn;

	// Clear list view column structure
	ZeroMemory( &lvColumn, sizeof( lvColumn ) );

	// Initialise list view column structure
	lvColumn.mask		= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
	lvColumn.cx			= nWidth;
	lvColumn.pszText	= ( LPTSTR )lpszTitle;

	// Add column to list view window
	nResult = SendMessage( g_hWndListView, LVM_INSERTCOLUMN, ( WPARAM )g_nColumnCount, (LPARAM )&lvColumn );

	// Ensure that column was added to list view window
	if( nResult >= 0 )
	{
		// Successfully added column to list view window

		// Update column count
		g_nColumnCount ++;

	} // End of successfully added column to list view window

	return nResult;

} // End of function ListViewWindowAddColumn

int ListViewWindowAddFile( LPCTSTR lpszFileName, DWORD dwMaximumFileNameLength )
{
	int nResult = -1;

	LVITEM lvItem;
	int nItemCount;

	// Count items on list view window
	nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, (LPARAM )NULL );

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= dwMaximumFileNameLength;
	lvItem.iItem		= nItemCount;
	lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= ( LPTSTR )lpszFileName;

	// Add file to list view window
	nResult = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	// Ensure that file was added to list view window
	if( nResult >= 0 )
	{
		// Successfully added file to list view window
	} // End of successfully added file to list view window

	return nResult;

} // End of function ListViewWindowAddFile

int ListViewWindowAutoSizeAllColumns()
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through all columns
	for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Auto-size current column
		if( SendMessage( g_hWndListView, LVM_SETCOLUMNWIDTH, ( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER ) )
		{
			// Successfully auto-sized current column

			// Update return value
			nResult ++;

		} // End of successfully auto-sized current column

	} // End of loop through all columns

	return nResult;

} // End of function ListViewWindowAutoSizeAllColumns

int CALLBACK ListViewWindowCompare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamColumn )
{
	int nResult = 0;

	LVITEM lvItem;

	// Allocate string memory
	LPTSTR lpszItem1 = new char[ STRING_LENGTH + sizeof( char ) ];
	LPTSTR lpszItem2 = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.iSubItem		= lParamColumn;
	lvItem.cchTextMax	= STRING_LENGTH;

	// Update list view item structure for first item
	lvItem.iItem	= lParam1;
	lvItem.pszText	= lpszItem1;

	// Get first item text
	if( SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )lParam1, ( LPARAM )&lvItem ) )
	{
		// Successfully got first item text

		// Update list view item structure for second item
		lvItem.iItem	= lParam2;
		lvItem.pszText	= lpszItem2;

		// Get second item text
		if( SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )lParam2, ( LPARAM )&lvItem ) )
		{
			// Successfully got second item text

			// Compare item texts
			nResult = lstrcmp( lpszItem1, lpszItem2 );

		} // End of successfully got second item text

	} // End of successfully got first item text

	// See if modified column is being sorted
	if( lParamColumn == LIST_VIEW_WINDOW_MODIFIED_COLUMN_ID )
	{
		// Modified column is being sorted

		// Invert result (to show most recent items first)
		nResult = ( -( nResult ) );

	} // End of modified column is being sorted

	// Free string memory
	delete [] lpszItem1;
	delete [] lpszItem2;

	return nResult;

} // End of function ListViewWindowCompare

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView = CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, LIST_VIEW_WINDOW_CLASS_NAME, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView )
	{
		// Successfully created list view window
		int nWhichColumn;
		LPCTSTR lpszColumnTitles [] = LIST_VIEW_WINDOW_COLUMN_TITLES;

		// Allocate global memory
		g_lpszParentFolderPath = new char[ STRING_LENGTH + sizeof( char ) ];

		// Clear parent folder path
		g_lpszParentFolderPath[ 0 ] = ( char )NULL;

		// Set extended list view window style
		SendMessage( g_hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )0, ( LPARAM )LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Initialise global variables
		g_nColumnCount = 0;

		// Loop through columns
		for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
		{
			// Add column to list view window
			ListViewWindowAddColumn( lpszColumnTitles[ nWhichColumn ] );

		}; // End of loop through columns

		// Auto-size all columns
		ListViewWindowAutoSizeAllColumns();

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window

	return bResult;

} // End of function ListViewWindowCreate

void ListViewWindowFreeMemory()
{
	// Free string memory
	delete [] g_lpszParentFolderPath;

} // End of function ListViewWindowFreeMemory

BOOL ListViewWindowGetFilePath( int nWhichFile, LPTSTR lpszFilePath )
{
	BOOL bResult = FALSE;

	LVITEM lvItem;

	// Allocate string memory
	LPTSTR lpszFileName = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= lpszFileName;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nWhichFile;

	// Get file name
	if( SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )nWhichFile, ( LPARAM )&lvItem ) )
	{
		// Successfully got file name

		// Copy parent folder path info file path
		lstrcpy( lpszFilePath, g_lpszParentFolderPath );

		// See if item is a folder
		if( lpszFileName[ 0 ] == LIST_VIEW_WINDOW_FOLDER_PREFIX )
		{
			// Item is a folder

			// Append file name (after prefix) onto file path
			lstrcat( lpszFilePath, ( lpszFileName + sizeof( LIST_VIEW_WINDOW_FOLDER_PREFIX ) ) );

		} // End of item is a folder
		else
		{
			// Item is a file

			// Append file name onto file path
			lstrcat( lpszFilePath, lpszFileName );

		} // End of item is a file

		// Update return value
		bResult = TRUE;

	} // End of successfully got file name

	// Free string memory
	delete [] lpszFileName;

	return bResult;

} // End of function ListViewWindowGetFilePath

BOOL ListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return GetWindowRect( g_hWndListView, lpRect );

} // End of function ListViewWindowGetRect

BOOL ListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmhdr;

	// Get notify message handler
	lpNmhdr = ( ( LPNMHDR )lParam );

	// Select list view window notification code
	switch( lpNmhdr->code )
	{
		case LVN_COLUMNCLICK:
		{
			// A column click notify message
			LPNMLISTVIEW lpNmListView;

			// Get list view notification message handler
			lpNmListView = ( ( LPNMLISTVIEW )lParam );

			// Sort the list view
			ListView_SortItemsEx( g_hWndListView, &ListViewWindowCompare, lpNmListView->iSubItem );

			// Break out of switch
			break;

		} // End of a column click notify message
		case LVN_ITEMCHANGED:
		{
			// A list view window item changed notification
			LPNMLISTVIEW lpNmListView;

			// Get list view notification message handler
			lpNmListView = ( ( LPNMLISTVIEW )lParam );

			// See if list view window item state has changed to selected
			if( ( lpNmListView->uNewState ^ lpNmListView->uOldState ) & LVIS_SELECTED )
			{
				// List view window item state has changed to selected

				// Allocate string memory
				LPTSTR lpszFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get file path
				if( ListViewWindowGetFilePath( lpNmListView->iItem, lpszFilePath ) )
				{
					// Successfully got file path

					// Show file path on status bar window
					bResult = ( *lpStatusFunction )( lpszFilePath );

				} // End of successfully got file path

				// Free string memory
				delete [] lpszFilePath;

			} // End of list view window item state has changed to selected

			// Break out of switch
			break;

		} // End of a list view window item changed notification
		case NM_DBLCLK:
		{
			// A double click notification
			int nSelectedItem;

			// Get selected item
			nSelectedItem = SendMessage( g_hWndListView, LVM_GETNEXTITEM, ( WPARAM )-1, ( LPARAM )LVNI_FOCUSED );

			// Ensure that selected item was got
			if( nSelectedItem >= 0 )
			{
				// Successfully got selected item

				// Allocate string memory
				LPTSTR lpszFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get selected item path
				if( ListViewWindowGetFilePath( nSelectedItem, lpszFilePath ) )
				{
					// Successfully got selected item path

					// Open selected item
					if( ( INT_PTR )ShellExecute( NULL, SHELL_EXECUTE_OPEN_COMMAND, lpszFilePath, NULL, NULL, SW_SHOWDEFAULT ) <= SHELL_EXECUTE_MINIMUM_SUCCESS_RETURN_VALUE )
					{
						// Unable to open item

						// Allocate string memory
						LPTSTR lpszErrorMessage = new char[ STRING_LENGTH + sizeof( char ) ];

						// Format error message
						wsprintf( lpszErrorMessage, UNABLE_TO_OPEN_FILE_ERROR_MESSAGE_FORMAT_STRING, lpszFilePath );

						// Display error message
						MessageBox( NULL, lpszErrorMessage, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

						// Free string memory
						delete [] lpszErrorMessage;

					} // End of unable to open item

				} // End of successfully got selected item path

				// Free string memory
				delete [] lpszFilePath;

			} // End of successfully got selected item

			// Break out of switch
			break;

		} // End of a double click notification
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for list view window notification code

	return bResult;

} // End of function ListViewWindowHandleNotifyMessage

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list view window
	return MoveWindow( g_hWndListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListViewWindowMove

int ListViewWindowPopulate( LPCTSTR lpszParentFolderPath )
{
	int nResult = 0;

	WIN32_FIND_DATA wfd;
	HANDLE hFileFind;

	// Allocate string memory
	LPTSTR lpszFullSearchPattern			= new char[ STRING_LENGTH + sizeof( char ) ];
	LPTSTR lpszTemporaryParentFolderPath	= new char[ STRING_LENGTH + sizeof( char ) ];

	// Store temporary parent folder path
	lstrcpy( lpszTemporaryParentFolderPath, lpszParentFolderPath );

	// Ensure that temporary parent folder path ends with a back-slash
	if( lpszTemporaryParentFolderPath[ lstrlen( lpszTemporaryParentFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
	{
		// Temporary parent folder path does not end with a back-slash

		// Append a back-slash onto temporary parent folder path
		lstrcat( lpszTemporaryParentFolderPath, ASCII_BACK_SLASH_STRING );

	} // End of temporary parent folder path does not end with a back-slash

	// Copy temporary parent folder path into full search pattern
	lstrcpy( lpszFullSearchPattern, lpszTemporaryParentFolderPath );

	// Append all files filter onto full search pattern
	lstrcat( lpszFullSearchPattern, ALL_FILES_FILTER );

	// Find first item
	hFileFind = FindFirstFile( lpszFullSearchPattern, &wfd );

	// Ensure that first item was found
	if( hFileFind != INVALID_HANDLE_VALUE )
	{
		// Successfully found first item
		int nInserted;
		LVITEM lvItem;
		FILETIME ftLocal;
		SYSTEMTIME stModified;

		// Allocate string memory
		LPTSTR lpszFolderText	= new char[ STRING_LENGTH + sizeof( char ) ];
		LPTSTR lpszModifiedText	= new char[ STRING_LENGTH + sizeof( char ) ];

		// Clear list view item structure
		ZeroMemory( &lvItem, sizeof( lvItem ) );

		// Initialise list view item structure
		lvItem.mask			= LVIF_TEXT;
		lvItem.cchTextMax	= STRING_LENGTH;
		lvItem.iItem		= 0;

		// Clear list view window
		SendMessage( g_hWndListView, LVM_DELETEALLITEMS, ( WPARAM )NULL, ( LPARAM )NULL );

		// Update global parent folder path
		lstrcpy( g_lpszParentFolderPath, lpszTemporaryParentFolderPath );

		// Loop through all items
		do
		{
			// Reset inserted value
			nInserted = -1;

			// See if current item is a folder
			if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// Current item is a folder

				// Ensure that current item is not dots
				if( wfd.cFileName[ 0 ] != ASCII_FULL_STOP_CHARACTER )
				{
					// Current item is not dots

					// Format folder text
					wsprintf( lpszFolderText, LIST_VIEW_WINDOW_FOLDER_TEXT_FORMAT_STRING, LIST_VIEW_WINDOW_FOLDER_PREFIX, wfd.cFileName );

					// Update list view item structure for folder
					lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
					lvItem.pszText		= lpszFolderText;

					// Add folder to list view window
					nInserted = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

					// Add folder to list view window

				} // End of current item is not dots

			} // End of current item is a folder
			else
			{
				// Current item is a file

				// Update list view item structure for file
				lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
				lvItem.pszText		= wfd.cFileName;

				// Add file to list view window
				nInserted = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

			} // End of current item is a file

			// Ensure that item was added to list view window
			if( nInserted >= 0 )
			{
				// Successfully added item to list view window

				// Get local file time
				FileTimeToLocalFileTime( &wfd.ftLastWriteTime, &ftLocal );

				// Get modified system time
				FileTimeToSystemTime( &ftLocal, &stModified );

				// Format modified text
				wsprintf( lpszModifiedText, LIST_VIEW_WINDOW_MODIFIED_TEXT_FORMAT_STRING, stModified.wYear, stModified.wMonth, stModified.wDay, stModified.wHour, stModified.wMinute, stModified.wSecond );

				// Update list view item structure for modified text
				lvItem.iItem		= nInserted;
				lvItem.iSubItem		= LIST_VIEW_WINDOW_MODIFIED_COLUMN_ID;
				lvItem.pszText		= lpszModifiedText;

				// Add modified text to list view window
				nInserted = SendMessage( g_hWndListView, LVM_SETITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

				// Update list view item structure for next item
				lvItem.iItem ++;

				// Update return value
				nResult ++;

			} // End of successfully added item to list view window

		} while( FindNextFile( hFileFind, &wfd ) != 0 ); // End of loop through all items

		// Close file find
		FindClose( hFileFind );

		// Auto-size all list view window columns
		ListViewWindowAutoSizeAllColumns();

		// Free string memory
		delete [] lpszFolderText;
		delete [] lpszModifiedText;

	} // End of successfully found first item

	// Free string memory
	delete [] lpszFullSearchPattern;
	delete [] lpszTemporaryParentFolderPath;

	return nResult;

} // End of function ListViewWindowPopulate

HWND ListViewWindowSetFocus()
{
	// Focus on list view window
	return SetFocus( g_hWndListView );

} // End of function ListViewWindowSetFocus

void ListViewWindowSetFont( HFONT hFont )
{
	// Set list view window font
	SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListViewWindowSetFont
