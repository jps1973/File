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

int ListViewWindowActionSelectedItems( BOOL( *lpActionFunction )( LPCTSTR lpszItemText, LPCTSTR lpszDestinationFolderPath ), LPCTSTR lpszDestinationFolderPath )
{
	int nResult = 0;

	int nItemCount;
	int nWhichItem;

	// Allocate string memory
	LPTSTR lpszItemName = new char[ STRING_LENGTH + sizeof( char ) ];

	// Count items on list view window
	nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Loop through items on list view window
	for( nWhichItem = 0; nWhichItem < nItemCount; nWhichItem ++ )
	{
		// See if current item is selected
		if( SendMessage( g_hWndListView, LVM_GETITEMSTATE, ( WPARAM )nWhichItem, ( LPARAM )LVIS_SELECTED ) & LVIS_SELECTED )
		{
			// Current item is selected

			// Get current item name
			if( ListViewWindowGetItemText( nWhichItem, LIST_VIEW_WINDOW_NAME_COLUMN_ID, lpszItemName ) )
			{
				// Successfully got current item name

				// Action current item
				if( ( *lpActionFunction )( lpszItemName, lpszDestinationFolderPath ) )
				{
					// Successfully actioned current item

					// Update return value
					nResult ++;

				} // End of successfully actioned current item

			} // End of successfully got current item name

		} // End of current item is selected

	}; // End of loop through items on list view window

	// Free string memory
	delete [] lpszItemName;

	return nResult;

} // End of function ListViewWindowActionSelectedItems

int ListViewWindowActionSelectedItems( HWND hWndOwner, LPCTSTR lpszSelectFolderTitle, BOOL( *lpActionFunction )( LPCTSTR lpszItemText, LPCTSTR lpszDestinationFolderPath ) )
{
	int nResult = 0;

	// Allocate string memory
	LPTSTR lpszDestinationFolderPath =  new char[ STRING_LENGTH + sizeof( char ) ];

	// Get destination folder path
	if( BrowseForFolder( lpszSelectFolderTitle, lpszDestinationFolderPath, hWndOwner ) )
	{
		// Successfully got destination folder path

		// Ensure that destination folder path ends with a back-slash
		if( lpszDestinationFolderPath[ lstrlen( lpszDestinationFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
		{
			// Destination folder path does not end with a back-slash

			// Append a back-slash onto destination folder path
			lstrcat( lpszDestinationFolderPath, ASCII_BACK_SLASH_STRING );

		} // End of destination folder path does not end with a back-slash

		// Action selected items
		nResult = ListViewWindowActionSelectedItems( lpActionFunction, lpszDestinationFolderPath );

	} // End of successfully got destination folder path

	// Free string memory
	delete [] lpszDestinationFolderPath;

	return nResult;

} // End of function ListViewWindowActionSelectedItems

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

		} // End of successfully got parent folder path into item path

	} // End of successfully got item text from list view window

	// Free string memory
	delete [] lpszItemText;

	return bResult;

} // End of function ListViewWindowGetItemPath

BOOL ListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText )
{
	BOOL bResult = FALSE;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= lpszItemText;

	// Get item text from list view window
	bResult = SendMessage( g_hWndListView, LVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function ListViewWindowGetItemText

LRESULT ListViewWindowHandleNotifyMessage( HWND hWndMain, WPARAM wParam, LPARAM lParam, BOOL( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	LRESULT lResult = 0;

	LPNMLISTVIEW lpNmListView;

	// Static variables
	static LPTSTR s_lpszOriginalName = NULL;
	static BOOL s_bIsFolder;

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
				( *lpDoubleClickFunction )( lpszFilePath );

			} // End of successfully got file path

			// Free string memory
			delete [] lpszFilePath;

			// Break out of switch
			break;

		} // End of a double click notify message
		case LVN_BEGINLABELEDIT:
		{
			// A begin label edit function
			HWND hWndEdit;

			// Clear original name
			s_lpszOriginalName = NULL;

			// Get edit window
			hWndEdit = ( HWND )SendMessage( g_hWndListView, LVM_GETEDITCONTROL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Ensure that edit window was got
			if( hWndEdit )
			{
				// Successfully got edit window

				// Allocate string memory
				LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get edit window text
				if( GetWindowText( hWndEdit, lpszItemText, STRING_LENGTH ) )
				{
					// Successfully got edit window text

					// Allocate global memory
					s_lpszOriginalName = new char[ STRING_LENGTH + sizeof( char ) ];

					// See if item is a folder
					if( lpszItemText[ 0 ] == LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_PREFIX )
					{
						// Item is a folder

						// Copy item text ( after prefix) into original name
						lstrcpy( s_lpszOriginalName, ( lpszItemText + sizeof( LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_PREFIX ) ) );

						// Update static folder value
						s_bIsFolder = TRUE;

					} // End of item is a folder
					else
					{
						// Item is a file

						// Copy item text into original name
						lstrcpy( s_lpszOriginalName, lpszItemText );

						// Update static folder value
						s_bIsFolder = FALSE;

					} // End of item is a file

				} // End of successfully got edit window text

				// Free string memory
				delete [] lpszItemText;

			} // End of successfully got edit window

			// Break out of switch
			break;

		} // End of a begin label edit function
		case LVN_COLUMNCLICK:
		{
			// A column click notify message

			// Sort the list view
			ListView_SortItemsEx( g_hWndListView, &ListViewWindowCompare, lpNmListView->iSubItem );

			// Break out of switch
			break;

		} // End of a column click notify message
		case LVN_ENDLABELEDIT:
		{
			// An end label edit function
			HWND hWndEdit;

			// Get edit window
			hWndEdit = ( HWND )SendMessage( g_hWndListView, LVM_GETEDITCONTROL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Ensure that edit window was got
			if( hWndEdit )
			{
				// Successfully got edit window

				// Allocate string memory
				LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get edit window text
				if( GetWindowText( hWndEdit, lpszItemText, STRING_LENGTH ) )
				{
					// Successfully got edit window text

					// Ensure that text has changed
					if( lstrcmp( s_lpszOriginalName, lpszItemText ) != 0 )
					{
						// Text has changed

						// Rename item
						if( MoveFile( s_lpszOriginalName, lpszItemText ) )
						{
							// Successfully renamed item

							// See if item is a folder
							if( s_bIsFolder )
							{
								// Item is a folder
								NMLVDISPINFO *lpNmLvDispInfo;

								// Allocate string memory
								LPTSTR lpszFolderDisplayText = new char[ STRING_LENGTH + sizeof( char ) ];

								// Get list view information
								lpNmLvDispInfo = ( NMLVDISPINFO * )lParam;

								// Format folder display text
								wsprintf( lpszFolderDisplayText, LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_FORMAT_STRING, LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_PREFIX, lpszItemText );

								// Set item text to contain folder display text
								ListViewWindowSetItemText( lpNmLvDispInfo->item.iItem, LIST_VIEW_WINDOW_NAME_COLUMN_ID, lpszFolderDisplayText );

								// Free string memory
								delete [] lpszFolderDisplayText;

								// Keep return value as it is
								// We have updated the label, so don't want it updated again

							} // End of item is a folder
							else
							{
								// Item is a file

								// Update return value to update label
								lResult = TRUE;

							} // End of item is a file

						} // End of successfully renamed item

					} // End of text has changed

				} // End of successfully got edit window text

				// Free string memory
				delete [] lpszItemText;

			} // End of successfully got edit window

			// See if original name is valid
			if( s_lpszOriginalName )
			{
				// Original name is valid

				// Free string memory
				delete [] s_lpszOriginalName;

			} // End of original name is valid

			// Break out of switch
			break;

		} // End of an end label edit function
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
					( *lpStatusFunction )( lpszItemPath );

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

			// Call default procedure
			lResult = DefWindowProc( hWndMain, WM_NOTIFY, wParam, lParam );

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for list view window notification code

	return lResult;

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
		int nImageListIndex;

		// Allocate string memory
		LPTSTR lpszModified				= new char[ STRING_LENGTH + sizeof( char ) ];
		LPTSTR lpszFolderDisplayText	= new char[ STRING_LENGTH + sizeof( char ) ];

		// Clear list view item structure
		ZeroMemory( &lvItem, sizeof( lvItem ) );

		// Initialise list view item structure
		lvItem.mask			= ( LVIF_TEXT | LVIF_IMAGE );
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

					// Get image list index for item
					nImageListIndex = SystemImageListGetItemIndex( wfd.cFileName );

					// Format folder display text
					wsprintf( lpszFolderDisplayText, LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_FORMAT_STRING, LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_PREFIX, wfd.cFileName );

					// Update list view item structure for found item name
					lvItem.iSubItem	= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
					lvItem.pszText	= lpszFolderDisplayText;
					lvItem.iImage	= nImageListIndex;

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

						// Update return value
						nResult ++;

						// Update list view item structure for next item
						lvItem.iItem ++;

					} // End of successfully added found item to list view window

				} // End of current item is not dots

			} // End of current item is a folder
			else
			{
				// Current item is a file

				// Get image list index for item
				nImageListIndex = SystemImageListGetItemIndex( wfd.cFileName );

				// Update list view item structure for found item name
				lvItem.iSubItem	= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
				lvItem.pszText	= wfd.cFileName;
				lvItem.iImage	= nImageListIndex;

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

					// Update return value
					nResult ++;

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
		delete [] lpszFolderDisplayText;

	} // End of successfully found first item in current folder

	return nResult;

} // End of function ListViewWindowPopulate

BOOL ListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText )
{
	BOOL bResult = FALSE;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Add found item to list view window
	bResult = SendMessage( g_hWndListView, LVM_SETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function ListViewWindowSetItemText
