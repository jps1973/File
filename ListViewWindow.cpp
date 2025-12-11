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

BOOL ListViewWindowAddItem( LVITEM lvItem, WIN32_FIND_DATA wfd )
{
	BOOL bResult = FALSE;

	int nItem;
	int nImageListIndex;

	// Allocate string memory
	LPTSTR lpszDisplayText = new char[ STRING_LENGTH + sizeof( char ) ];

	// Get image list index
	nImageListIndex = SystemImageListGetItemIndex( wfd.cFileName );

	// See if item is a folder
	if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
	{
		// Item is a folder

		// Format display text for folder
		wsprintf( lpszDisplayText, LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_FORMAT_STRING, LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_PREFIX, wfd.cFileName );

	} // End of item is a folder
	else
	{
		// Item is a file

		// Update display text for file
		lstrcpy( lpszDisplayText, wfd.cFileName );

	} // End of item is a folder

	// Update list view item structure for item
	lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= lpszDisplayText;
	lvItem.iImage		= nImageListIndex;

	// Add found item to list view window
	nItem = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&lvItem );

	// Ensure that item was added to list view window
	if( nItem >= 0 )
	{
		// Successfully added item to list view window
		SYSTEMTIME stModified;

		// Allocate string memory
		LPTSTR lpszModified = new char[ STRING_LENGTH + sizeof( char ) ];

		// Get modified system time
		FileTimeToSystemTime( &( wfd.ftLastWriteTime ), &stModified );

		// Format modified text
		wsprintf( lpszModified, LIST_VIEW_WINDOW_MODIFIED_TEXT_FORMAT_STRING, stModified.wYear, stModified.wMonth, stModified.wDay, stModified.wHour, stModified.wMinute, stModified.wSecond );

		// Update list view item structure for modified column
		lvItem.iSubItem	= LIST_VIEW_WINDOW_MODIFIED_COLUMN_ID;
		lvItem.pszText	= lpszModified;

		// Set modified item text
		SendMessage( g_hWndListView, LVM_SETITEM, ( WPARAM )nItem, ( LPARAM )&lvItem );

		// Update return value
		bResult = TRUE;

		// Free string memory
		delete [] lpszModified;

	} // End of successfully added item to list view window

	// Free string memory
	delete [] lpszDisplayText;

	return bResult;

} // End of function ListViewWindowAddItem

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

		// Initialise system image list
		SystemImageListInit();

		// Set list view window image list
		SystemImageListSetListView( g_hWndListView, LVSIL_SMALL );

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

void ListViewWindowFreeMemory()
{
	// Free global string memory
	delete [] g_lpszParentFolderPath;

} // End of function ListViewWindowFreeMemory

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

		// Copy parent folder path into item path
		lstrcpy( lpszItemPath, g_lpszParentFolderPath );

		// See if item is a folder
		if( lpszItemText[ 0 ] == LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_PREFIX )
		{
			// Item is a folder

			// Append item text (after folder prefix) onto item path
			lstrcat( lpszItemPath, ( lpszItemText + sizeof( LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_PREFIX ) ) );

		} // End of item is a folder
		else
		{
			// Item is a file

			// Append item text onto item path
			lstrcat( lpszItemPath, lpszItemText );

		} // End of item is a file

		// Update return value
		bResult = TRUE;

	} // End of successfully got item text from list view window

	// Free string memory
	delete [] lpszItemText;

	return bResult;

} // End of function ListViewWindowGetItemPath

BOOL ListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
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

				// Display file path
				MessageBox( NULL, lpszFilePath, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

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

} // End of function ComboBoxWindowMove

int ListViewWindowPopulate( LPCTSTR lpszFolderPath )
{
	int nResult = 0;

	// Allocate string memory
	LPTSTR lpszParentFolderPath		= new char[ STRING_LENGTH + sizeof( char ) ];
	LPTSTR lpszFullSearchPattern	= new char[ STRING_LENGTH + sizeof( char ) ];

	// Store parent folder path
	lstrcpy( lpszParentFolderPath, lpszFolderPath );

	// Ensure that parent folder path ends in a back-slash
	if( lpszParentFolderPath[ lstrlen( lpszParentFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
	{
		// Parent folder path does not end in a back-slash

		// Append a back-slash onto parent folder path
		lstrcat( lpszParentFolderPath, ASCII_BACK_SLASH_STRING );

	} // End of parent folder path does not end in a back-slash

	// Copy parent folder path into full search pattern
	lstrcpy( lpszFullSearchPattern, lpszParentFolderPath );

	// Append all files filter onto full search pattern
	lstrcat( lpszFullSearchPattern, ALL_FILES_FILTER );

	WIN32_FIND_DATA wfd;
	HANDLE hFileFind;

	// Find first item
	hFileFind = FindFirstFile( lpszFullSearchPattern, &wfd );

	// Ensure that first item was found
	if( hFileFind != INVALID_HANDLE_VALUE )
	{
		// Successfully found first item
		LVITEM lvItem;

		// Clear list view item structure
		ZeroMemory( &lvItem, sizeof( lvItem ) );

		// Initialise list view item structure
		lvItem.mask			= ( LVIF_TEXT | LVIF_IMAGE );
		lvItem.cchTextMax	= STRING_LENGTH;
		lvItem.iItem		= 0;

		// Delete all items from list view window
		SendMessage( g_hWndListView, LVM_DELETEALLITEMS, ( WPARAM )NULL, ( LPARAM )NULL );

		// Update current folder
		SetCurrentDirectory( lpszParentFolderPath );
		// This will ensure that icons are correct

		// Loop through all items
		do
		{
			// See if found item is a folder
			if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// Found item is a folder

				// Ensure that found item is not dots
				if( wfd.cFileName[ 0 ] != ASCII_FULL_STOP_CHARACTER )
				{
					// Found item is not dots

					// Add item to list view window
					if( ListViewWindowAddItem( lvItem, wfd ) )
					{
						// Successfully added item to list view window

						// Update return value
						nResult ++;

					} // End of successfully added item to list view window

				} // End of found item is not dots

			} // End of found item is a folder
			else
			{
				// Found item is a file

				// Add item to list view window
				if( ListViewWindowAddItem( lvItem, wfd ) )
				{
					// Successfully added item to list view window

					// Update return value
					nResult ++;

				} // End of successfully added item to list view window

			} // End of found item is a file

		} while( FindNextFile( hFileFind, &wfd ) != 0 ); // End of loop through all items

		// Close file find
		FindClose( hFileFind );

		// Auto-size all list view window columns
		ListViewWindowAutoSizeAllColumns();

		// Update global parent folder path
		lstrcpy( g_lpszParentFolderPath, lpszParentFolderPath );

	} // End of successfully found first item

	// Free string memory
	delete [] lpszParentFolderPath;
	delete [] lpszFullSearchPattern;

	return nResult;

} // End of function ListViewWindowPopulate
