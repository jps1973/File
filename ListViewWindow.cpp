// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
static HWND g_hWndListView[ NUMBER_OF_COLUMNS ];
static int g_nColumnCount[ NUMBER_OF_COLUMNS ];
static LPTSTR g_lpszParentFolderPath[ NUMBER_OF_COLUMNS ];

BOOL IsListViewWindow( int nWhichListViewWindow, HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndListView[ nWhichListViewWindow ] );

} // End of function IsListViewWindow

int ListViewWindowAddColumn( int nWhichListViewWindow, LPCTSTR lpszTitle, int nWidth )
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
	nResult = SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_INSERTCOLUMN, ( WPARAM )g_nColumnCount[ nWhichListViewWindow ], (LPARAM )&lvColumn );

	// Ensure that column was added to list view window
	if( nResult >= 0 )
	{
		// Successfully added column to list view window

		// Update column count
		g_nColumnCount[ nWhichListViewWindow ] ++;

	} // End of successfully added column to list view window

	return nResult;

} // End of function ListViewWindowAddColumn

int ListViewWindowAddFile( int nWhichListViewWindow, LPCTSTR lpszFileName, DWORD dwMaximumFileNameLength )
{
	int nResult = -1;

	LVITEM lvItem;
	int nItemCount;

	// Count items on list view window
	nItemCount = SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_GETITEMCOUNT, ( WPARAM )NULL, (LPARAM )NULL );

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= dwMaximumFileNameLength;
	lvItem.iItem		= nItemCount;
	lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= ( LPTSTR )lpszFileName;

	// Add file to list view window
	nResult = SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	// Ensure that file was added to list view window
	if( nResult >= 0 )
	{
		// Successfully added file to list view window
	} // End of successfully added file to list view window

	return nResult;

} // End of function ListViewWindowAddFile

int ListViewWindowAutoSizeAllColumns( int nWhichListViewWindow )
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through all columns
	for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Auto-size current column
		if( SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_SETCOLUMNWIDTH, ( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER ) )
		{
			// Successfully auto-sized current column

			// Update return value
			nResult ++;

		} // End of successfully auto-sized current column

	} // End of loop through all columns

	return nResult;

} // End of function ListViewWindowAutoSizeAllColumns

int CALLBACK ListViewWindowCompare( int nWhichListViewWindow, LPARAM lParam1, LPARAM lParam2, LPARAM lParamColumn )
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
	if( SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_GETITEMTEXT, ( WPARAM )lParam1, ( LPARAM )&lvItem ) )
	{
		// Successfully got first item text

		// Update list view item structure for second item
		lvItem.iItem	= lParam2;
		lvItem.pszText	= lpszItem2;

		// Get second item text
		if( SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_GETITEMTEXT, ( WPARAM )lParam2, ( LPARAM )&lvItem ) )
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

BOOL ListViewWindowCreate( int nWhichListViewWindow, HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView[ nWhichListViewWindow ] = CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, LIST_VIEW_WINDOW_CLASS_NAME, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView[ nWhichListViewWindow ] )
	{
		// Successfully created list view window
		int nWhichColumn;
		HIMAGELIST hImageList;
		LPCTSTR lpszColumnTitles [] = LIST_VIEW_WINDOW_COLUMN_TITLES;

		// Allocate global memory
		g_lpszParentFolderPath[ nWhichListViewWindow ] = new char[ STRING_LENGTH + sizeof( char ) ];

		// Clear parent folder path
		g_lpszParentFolderPath[ nWhichListViewWindow ][ 0 ] = ( char )NULL;

		// Set extended list view window style
		SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )0, ( LPARAM )LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Initialise global variables
		g_nColumnCount[ nWhichListViewWindow ] = 0;

		// Get system image list
		hImageList = ImageListGetSystem();

		// Set list view window image list
		SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_SETIMAGELIST, ( WPARAM )LVSIL_SMALL, ( LPARAM )hImageList );

		// Loop through columns
		for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
		{
			// Add column to list view window
			ListViewWindowAddColumn( nWhichListViewWindow, lpszColumnTitles[ nWhichColumn ] );

		}; // End of loop through columns

		// Auto-size all columns
		ListViewWindowAutoSizeAllColumns( nWhichListViewWindow );

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window

	return bResult;

} // End of function ListViewWindowCreate

void ListViewWindowFreeMemory( int nWhichListViewWindow )
{
	// Free string memory
	delete [] g_lpszParentFolderPath[ nWhichListViewWindow ];

} // End of function ListViewWindowFreeMemory

BOOL ListViewWindowGetFilePath( int nWhichListViewWindow, int nWhichFile, LPTSTR lpszFilePath )
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
	if( SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_GETITEMTEXT, ( WPARAM )nWhichFile, ( LPARAM )&lvItem ) )
	{
		// Successfully got file name

		// Copy parent folder path info file path
		lstrcpy( lpszFilePath, g_lpszParentFolderPath[ nWhichListViewWindow ] );

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

BOOL ListViewWindowGetRect( int nWhichListViewWindow, LPRECT lpRect )
{
	// Get list view window rect
	return GetWindowRect( g_hWndListView[ nWhichListViewWindow ], lpRect );

} // End of function ListViewWindowGetRect

BOOL ListViewWindowHandleNotifyMessage( int nWhichListViewWindow, WPARAM, LPARAM lParam, BOOL( *lpSelectionChangedFunction )( int nWhichListViewWindow, LPCTSTR lpszItemText ), BOOL( *lpDoubleClickFunction )( int nWhichListViewWindow, LPCTSTR lpszItemText ) )
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
			ListView_SortItemsEx( g_hWndListView[ nWhichListViewWindow ], &ListViewWindowCompare, lpNmListView->iSubItem );

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
				if( ListViewWindowGetFilePath( nWhichListViewWindow, lpNmListView->iItem, lpszFilePath ) )
				{
					// Successfully got file path

					// Call selection changed function
					bResult = ( *lpSelectionChangedFunction )( nWhichListViewWindow, lpszFilePath );

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
			nSelectedItem = SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_GETNEXTITEM, ( WPARAM )-1, ( LPARAM )LVNI_FOCUSED );

			// Ensure that selected item was got
			if( nSelectedItem >= 0 )
			{
				// Successfully got selected item

				// Allocate string memory
				LPTSTR lpszFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get selected item path
				if( ListViewWindowGetFilePath( nWhichListViewWindow, nSelectedItem, lpszFilePath ) )
				{
					// Successfully got selected item path

					// Call double click function
					bResult = ( *lpDoubleClickFunction )( nWhichListViewWindow, lpszFilePath );

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

BOOL ListViewWindowMove( int nWhichListViewWindow, int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list view window
	return MoveWindow( g_hWndListView[ nWhichListViewWindow ], nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListViewWindowMove

int ListViewWindowPopulate( int nWhichListViewWindow, LPCTSTR lpszParentFolderPath )
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
		int nImageListIndex;

		// Allocate string memory
		LPTSTR lpszFolderText		= new char[ STRING_LENGTH + sizeof( char ) ];
		LPTSTR lpszModifiedText		= new char[ STRING_LENGTH + sizeof( char ) ];
		LPTSTR lpszFoundItemPath	= new char[ STRING_LENGTH + sizeof( char ) ];

		// Clear list view item structure
		ZeroMemory( &lvItem, sizeof( lvItem ) );

		// Initialise list view item structure
		lvItem.mask			= ( LVIF_TEXT | LVIF_IMAGE );
		lvItem.cchTextMax	= STRING_LENGTH;
		lvItem.iItem		= 0;

		// Clear list view window
		SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_DELETEALLITEMS, ( WPARAM )NULL, ( LPARAM )NULL );

		// Update global parent folder path
		lstrcpy( g_lpszParentFolderPath[ nWhichListViewWindow ], lpszTemporaryParentFolderPath );

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

					// Copy parent folder path into found item path
					lstrcpy( lpszFoundItemPath, g_lpszParentFolderPath[ nWhichListViewWindow ] );

					// Append found item name onto found item path
					lstrcat( lpszFoundItemPath, wfd.cFileName );

					// Get image list index
					nImageListIndex = ImageListGetIndex( lpszFoundItemPath );

					// Format folder text
					wsprintf( lpszFolderText, LIST_VIEW_WINDOW_FOLDER_TEXT_FORMAT_STRING, LIST_VIEW_WINDOW_FOLDER_PREFIX, wfd.cFileName );

					// Update list view item structure for folder
					lvItem.iSubItem	= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
					lvItem.pszText	= lpszFolderText;
					lvItem.iImage	= nImageListIndex;

					// Add folder to list view window
					nInserted = SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

					// Add folder to list view window

				} // End of current item is not dots

			} // End of current item is a folder
			else
			{
				// Current item is a file

				// Copy parent folder path into found item path
				lstrcpy( lpszFoundItemPath, g_lpszParentFolderPath[ nWhichListViewWindow ] );

				// Append found item name onto found item path
				lstrcat( lpszFoundItemPath, wfd.cFileName );

				// Get image list index
				nImageListIndex = ImageListGetIndex( lpszFoundItemPath );

				// Update list view item structure for file
				lvItem.iSubItem	= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
				lvItem.pszText	= wfd.cFileName;
				lvItem.iImage	= nImageListIndex;

				// Add file to list view window
				nInserted = SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

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
				nInserted = SendMessage( g_hWndListView[ nWhichListViewWindow ], LVM_SETITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

				// Update list view item structure for next item
				lvItem.iItem ++;

				// Update return value
				nResult ++;

			} // End of successfully added item to list view window

		} while( FindNextFile( hFileFind, &wfd ) != 0 ); // End of loop through all items

		// Close file find
		FindClose( hFileFind );

		// Auto-size all list view window columns
		ListViewWindowAutoSizeAllColumns( nWhichListViewWindow );

		// Free string memory
		delete [] lpszFolderText;
		delete [] lpszModifiedText;
		delete [] lpszFoundItemPath;

	} // End of successfully found first item

	// Free string memory
	delete [] lpszFullSearchPattern;
	delete [] lpszTemporaryParentFolderPath;

	return nResult;

} // End of function ListViewWindowPopulate

HWND ListViewWindowSetFocus( int nWhichListViewWindow )
{
	// Focus on list view window
	return SetFocus( g_hWndListView[ nWhichListViewWindow ] );

} // End of function ListViewWindowSetFocus

void ListViewWindowSetFont( int nWhichListViewWindow, HFONT hFont )
{
	// Set list view window font
	SendMessage( g_hWndListView[ nWhichListViewWindow ], WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListViewWindowSetFont
