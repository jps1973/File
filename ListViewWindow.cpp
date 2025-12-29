// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
static HWND g_hWndListView;
static LPTSTR g_lpszParentFolderPath;

BOOL IsListViewWindow( HWND hWndSupplied )
{
	BOOL bResult = FALSE;

	// See if supplied window is list view window
	if( hWndSupplied == g_hWndListView )
	{
		// Supplied window is list view window

		// Update return value
		bResult = TRUE;

	} // End of supplied window is list view window

	return bResult;

} // End of function IsListViewWindow

int ListViewWindowAutoSizeAllColumns()
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through all columns
	for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Update list view column structure for current column

		// Auto-size current column in list view window
		if( SendMessage( g_hWndListView, LVM_SETCOLUMNWIDTH, ( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER ) )
		{
			// Successfully auto-size current column in list view window

			// Update return value
			nResult ++;

		} // End of successfully auto-size current column in list view window
		else
		{
			// Unable to auto-size current column in list view window

			// Force exit from loop
			nWhichColumn = LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS;

		} // End of unable to auto-size current column in list view window

	}; // End of loop through all columns

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

	// See if this is the modified column
	if( lParamColumn == LIST_VIEW_WINDOW_MODIFIED_COLUMN_ID )
	{
		// This is the modified column

		// Invert result (to show most recent items first)
		nResult = ( 0 - nResult );

	} // End of this is the modified column

	// Free string memory
	delete [] lpszItem1;
	delete [] lpszItem2;

	return nResult;

} // End of function ListViewWindowCompare

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance, HFONT hFont )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView = CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, LIST_VIEW_WINDOW_CLASS_NAME, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 100, 100, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView )
	{
		// Successfully created list view window
		int nWhichColumn;
		LPCTSTR lpszColumnTitles [] = LIST_VIEW_WINDOW_TITLES;
		LVCOLUMN lvColumn;

		// Allocate global string memory
		g_lpszParentFolderPath = new char[ STRING_LENGTH + sizeof( char ) ];

		// Clear parent folder path
		g_lpszParentFolderPath[ 0 ] = ( char )NULL;

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initialise list view column structure
		lvColumn.mask	= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
		lvColumn.cx		= LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH;

		// Set list view window font
		SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

		// Set extended list view window style
		SendMessage( g_hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )NULL, ( LPARAM )LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Loop through all columns
		for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
		{
			// Update list view column structure for current column
			lvColumn.pszText = ( LPSTR )( lpszColumnTitles[ nWhichColumn ] );

			// Add current column to list view window
			SendMessage( g_hWndListView, LVM_INSERTCOLUMN, ( WPARAM )nWhichColumn, ( LPARAM )&lvColumn );

		}; // End of loop through all columns

		// Auto-size all list view window columns
		ListViewWindowAutoSizeAllColumns();

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window

	return bResult;

} // End of function ListViewWindowCreate

BOOL ListViewWindowGetItemPath( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemPath )
{
	BOOL bResult = FALSE;

	LVITEM lvItem;

	// Allocate string memory
	LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= lpszItemText;

	// Get item text from list view window
	if( SendMessage( g_hWndListView, LVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem ) )
	{
		// Successfully got item text from list view window

		// Get parent folder path into item path
		if( GetCurrentDirectory( STRING_LENGTH, lpszItemPath ) )
		{
			// Successfully got parent folder path into item path

			// Ensure that item path ends with a back-slash
			if( lpszItemPath[ lstrlen( lpszItemPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
			{
				// Item path does not end with a back-slash

				// Append a back-slash onto item path
				lstrcat( lpszItemPath, ASCII_BACK_SLASH_STRING );

			} // End of item path does not end with a back-slash

			// Append item text onto item path
			lstrcat( lpszItemPath, lpszItemText );

			// Update return value
			bResult = TRUE;

		} // End of successfully got parent folder path into item path

	} // End of successfully got item text from list view window

	// Free string memory
	delete [] lpszItemText;

	return bResult;

} // End of function ListViewWindowGetItemPath

BOOL ListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, BOOL( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMLISTVIEW lpNmListView;

	// Get list view notify message handler
	lpNmListView = ( LPNMLISTVIEW )lParam;

	// Select list view window notification code
	switch( lpNmListView->hdr.code )
	{
		case NM_DBLCLK:
		{
			// A double click notify message

			// Allocate string memory
			LPTSTR lpszFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get file path
			if( ListViewWindowGetItemPath( lpNmListView->iItem, lpNmListView->iSubItem, lpszFilePath ) )
			{
				// Successfully got file path

				// Call double click function
				bResult = ( *lpDoubleClickFunction )( lpszFilePath );

			} // End of successfully got file path

			// Free string memory
			delete [] lpszFilePath;

			// Break out of switch
			break;

		} // End of a double click notify message
		case LVN_COLUMNCLICK:
		{
			// A column click notify message

			// Sort the list view
			ListView_SortItemsEx( g_hWndListView, &ListViewWindowCompare, lpNmListView->iSubItem );

			// Break out of switch
			break;

		} // End of a column click notify message
		case LVN_ITEMCHANGED:
		{
			// A list view item changed notify message

			// See if item state has changed to selected
			if( ( lpNmListView->uNewState ^ lpNmListView->uOldState ) & LVIS_SELECTED )
			{
				// Item state has changed to selected

				// Allocate string memory
				LPTSTR lpszItemPath = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get item path
				if( ListViewWindowGetItemPath( lpNmListView->iItem, lpNmListView->iSubItem, lpszItemPath ) )
				{
					// Successfully got item path

					// Call status function
					bResult = ( *lpStatusFunction )( lpszItemPath );

				} // End of successfully got item path

				// Free string memory
				delete [] lpszItemPath;

			} // End of item state has changed to selected

			// Break out of switch
			break;

		} // End of a list view item changed notify message
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

BOOL ListViewWindowMove( int nLeft, int nTop, int nWidth, int nHeight )
{
	// Move list view window
	return MoveWindow( g_hWndListView, nLeft, nTop, nWidth, nHeight, TRUE );

} // End of function ListViewWindowMove

int ListViewWindowPopulate()
{
	int nResult = 0;

	WIN32_FIND_DATA wfd;
	HANDLE hFileFind;

	// Find first item in current folder
	hFileFind = FindFirstFile( ALL_FILES_FILTER, &wfd );

	// Ensure that first item in current folder was found
	if( hFileFind != INVALID_HANDLE_VALUE )
	{
		// Successfully found first item in current folder
		LVITEM lvItem;
		SYSTEMTIME stModified;

		// Allocate string memory
		LPTSTR lpszModified = new char[ STRING_LENGTH + sizeof( char ) ];

		// Clear list view item structure
		ZeroMemory( &lvItem, sizeof( lvItem ) );

		// Initialise list view item structure
		lvItem.mask			= LVIF_TEXT;
		lvItem.cchTextMax	= STRING_LENGTH;
		lvItem.iItem		= 0;

		// Delete all items from list view window
		SendMessage( g_hWndListView, LVM_DELETEALLITEMS, ( WPARAM )NULL, ( LPARAM )NULL );

		// Loop through all items
		do
		{
			// See if current item is a folder
			if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// Current item is a folder

				// Ensure that current item is not dots
				if( wfd.cFileName[ 0 ] != ASCII_FULL_STOP_CHARACTER )
				{
					// Current item is not dots

					// Update list view item structure for found item name
					lvItem.iSubItem	= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
					lvItem.pszText	= wfd.cFileName;

					// Add found item to list view window
					lvItem.iItem = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

					// Ensure that found item was added to list view window
					if( lvItem.iItem >= 0 )
					{
						// Successfully added found item to list view window

						// Get modified system time
						FileTimeToSystemTime( &( wfd.ftLastWriteTime ), &stModified );

						// Format modified text
						wsprintf( lpszModified, LIST_VIEW_WINDOW_MODIFIED_TEXT_FORMAT_STRING, stModified.wYear, stModified.wMonth, stModified.wDay, stModified.wHour, stModified.wMinute, stModified.wSecond );

						// Update list view item structure for found item name
						lvItem.iSubItem	= LIST_VIEW_WINDOW_MODIFIED_COLUMN_ID;
						lvItem.pszText	= lpszModified;

						// Add found item to list view window
						SendMessage( g_hWndListView, LVM_SETITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

						// Update list view item structure for next item
						lvItem.iItem ++;

					} // End of successfully added found item to list view window

				} // End of current item is not dots

			} // End of current item is a folder
			else
			{
				// Current item is a file

				// Update list view item structure for found item name
				lvItem.iSubItem	= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
				lvItem.pszText	= wfd.cFileName;

				// Add found item to list view window
				lvItem.iItem = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

				// Ensure that found item was added to list view window
				if( lvItem.iItem >= 0 )
				{
					// Successfully added found item to list view window

					// Get modified system time
					FileTimeToSystemTime( &( wfd.ftLastWriteTime ), &stModified );

					// Format modified text
					wsprintf( lpszModified, LIST_VIEW_WINDOW_MODIFIED_TEXT_FORMAT_STRING, stModified.wYear, stModified.wMonth, stModified.wDay, stModified.wHour, stModified.wMinute, stModified.wSecond );

					// Update list view item structure for found item name
					lvItem.iSubItem	= LIST_VIEW_WINDOW_MODIFIED_COLUMN_ID;
					lvItem.pszText	= lpszModified;

					// Add found item to list view window
					SendMessage( g_hWndListView, LVM_SETITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

					// Update list view item structure for next item
					lvItem.iItem ++;

				} // End of successfully added found item to list view window

			} // End of current item is a file

		} while( FindNextFile( hFileFind, &wfd ) != 0 ); // End of loop through all items

		// Auto-size all list view window columns
		ListViewWindowAutoSizeAllColumns();

		// Close file find
		FindClose( hFileFind );

		// Free string memory
		delete [] lpszModified;

	} // End of successfully found first item in current folder

	return nResult;

} // End of function ListViewWindowPopulate
