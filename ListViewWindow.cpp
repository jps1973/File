// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
HWND g_hWndListView;
LPTSTR g_lpszParentFolderPath;

BOOL IsListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndListView );

} // End of function IsListViewWindow

int ListViewWindowAddFiles( LPCTSTR lpszFolderPath, LPCTSTR lpszFileFilter )
{
	int nResult = 0;

	WIN32_FIND_DATA  wfd;
	HANDLE hFind;

	// Allocate string memory
	LPTSTR lpszFullSearchPattern = new char[ STRING_LENGTH ];

	// Copy folder path into global parent folder path
	lstrcpy( g_lpszParentFolderPath, lpszFolderPath );

	// Ensure that global parent folder path ends with a back-slash
	if( g_lpszParentFolderPath[ lstrlen( g_lpszParentFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
	{
		// Global parent folder path does not end with a back-slash

		// Append a back-slash onto global parent folder path
		lstrcat( g_lpszParentFolderPath, ASCII_BACK_SLASH_STRING );

	} // End of global parent folder path does not end with a back-slash

	// Copy global parent folder path into full search pattern
	lstrcpy( lpszFullSearchPattern, g_lpszParentFolderPath );

	// Append file filter onto full search pattern
	lstrcat( lpszFullSearchPattern, lpszFileFilter );

	// Find first item
	hFind = FindFirstFile( lpszFullSearchPattern, &wfd );

	// Ensure that first item was found
	if( hFind != INVALID_HANDLE_VALUE ) 
	{
		// Successfully found first item
		LVITEM lvItem;
		int nIconIndex;

		// Allocate string memory
		LPTSTR lpszFoundFilePath = new char[ STRING_LENGTH ];

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
			// See if current item is a file
			if( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				// Current item is a file

				// Copy parent folder path into found file path
				lstrcpy( lpszFoundFilePath, g_lpszParentFolderPath );

				// Append found file name onto found file path
				lstrcat( lpszFoundFilePath, wfd.cFileName );

				// Get icon index for found file
				nIconIndex = ImageListGetIconIndex( lpszFoundFilePath );

				// Update list view item structure for current file name
				lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
				lvItem.pszText		= wfd.cFileName;
				lvItem.iImage		= nIconIndex;

				// Add current file name to list view window
				lvItem.iItem = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

				// Ensure that current file name was added to list view window
				if( lvItem.iItem >= 0 )
				{
					// Successfully added current file name to list view window

					// Update list view item structure for next item
					lvItem.iItem ++;

					// Update return value
					nResult ++;

				} // End of successfully added current file name to list view window

			} // End of current item is a file

		} while( FindNextFile( hFind, &wfd ) != 0 ); // End of loop through all items

		// Auto-size all list view window columns
		ListViewWindowAutoSizeAllColumns();

		// Close file find
		FindClose( hFind );

		// Free string memory
		delete lpszFoundFilePath;

	} // End of successfully found first item

	// Free string memory
	delete [] lpszFullSearchPattern;

	return nResult;

} // End of function ListViewWindowAddFiles

int ListViewWindowAutoSizeAllColumns()
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through all columns
	for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Set current column width
		if( SendMessage( g_hWndListView, LVM_SETCOLUMNWIDTH, ( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER ) )
		{
			// Successfully set current column width

			// Update return value
			nResult ++;

		} // End of successfully set current column width
		else
		{
			// Unable to set current column width

			// Force exit from loop
			nWhichColumn = LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS;

		} // End of unable to set current column width

	}; // End of loop through all columns

	return nResult;

} // End of function ListViewWindowAutoSizeAllColumns

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView = ::CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, LIST_VIEW_WINDOW_CLASS_NAME, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView )
	{
		// Successfully created list view window
		LVCOLUMN lvColumn;
		LPCTSTR lpszColumnTitles [] = LIST_VIEW_WINDOW_COLUMN_TITLES;

		// Allocate global memory
		g_lpszParentFolderPath = new char[ STRING_LENGTH ];

		// Clear parent folder path
		g_lpszParentFolderPath[ 0 ] = ( char )NULL;

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initialise list view column structure
		lvColumn.mask		= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
		lvColumn.cx			= LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH;
		lvColumn.cchTextMax	= STRING_LENGTH;

		// Set extended list view window style
		SendMessage( g_hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )0, ( LPARAM )LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Loop through columns
		for( lvColumn.iSubItem = 0; lvColumn.iSubItem < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; lvColumn.iSubItem ++ )
		{

			// Update list view column structure for current column
			lvColumn.pszText = ( LPTSTR )lpszColumnTitles[ lvColumn.iSubItem ];

			// Add column to list view window
			SendMessage( g_hWndListView, LVM_INSERTCOLUMN, ( WPARAM )lvColumn.iSubItem, ( LPARAM )&lvColumn );

		}; // End of loop through columns

		// Auto-size all columns
		ListViewWindowAutoSizeAllColumns();

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window

	return bResult;

} // End of function ListViewWindowCreate

int ListViewWwidowGetFilePath( int nWhichItem, LPTSTR lpszFilePath )
{
	int nResult;

	LVITEM lvItem;

	// Copy parent folder path into file path
	lstrcpy( lpszFilePath, g_lpszParentFolderPath );

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= ( lpszFilePath + lstrlen( lpszFilePath ) );
	lvItem.cchTextMax	= STRING_LENGTH;

	// Append item text onto file path
	nResult = SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )nWhichItem, ( LPARAM )&lvItem ); 

	return nResult;

} // End of function ListViewWwidowGetFilePath

int ListViewWwidowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText )
{
	int nResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= lpszItemText;
	lvItem.cchTextMax	= STRING_LENGTH;

	// Get item text
	nResult = SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )nWhichItem, ( LPARAM )&lvItem ); 

	return nResult;

} // End of function ListViewWwidowGetItemText

BOOL ListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return ::GetWindowRect( g_hWndListView, lpRect );

} // End of function ListViewWindowGetRect

BOOL ListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszFilePath ), void( *lpSelectionChangedFunction )( LPCTSTR lpszFilePath ) )
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmHdr;

	// Get notify message handler
	lpNmHdr = ( LPNMHDR )lParam;

	// Select tree view window notification code
	switch( lpNmHdr->code )
	{
		case NM_DBLCLK:
		{
			// A list view window double click notification code
			LPNMLISTVIEW lpNmListView;

			// Allocate string memory
			LPTSTR lpszFilePath = new char[ STRING_LENGTH ];

			// Get list view notify message information
			lpNmListView = ( LPNMLISTVIEW )lParam;

			// Get file path
			if( ListViewWwidowGetFilePath( lpNmListView->iItem, lpszFilePath ) )
			{
				// Successfully got file path

				// Call double click function
				( *lpDoubleClickFunction )( lpszFilePath );

				// Update return value
				bResult = TRUE;

			} // End of successfully got file path

			// Free string memory
			delete [] lpszFilePath;

			// Break out of switch
			break;

		} // End of a list view window double click notification code
		case LVN_ITEMCHANGED:
		{
			// A list view window item changed notification code
			LPNMLISTVIEW lpNmListView;

			// Get list view notify message information
			lpNmListView = ( LPNMLISTVIEW )lParam;

			// Ensure that item state has changed to selected
			if( ( lpNmListView->uNewState ^  lpNmListView->uOldState ) & LVIS_SELECTED )
			{
				// Item state has changed to selected

				// Allocate string memory
				LPTSTR lpszFilePath = new char[ STRING_LENGTH ];

				// Get file path
				if( ListViewWwidowGetFilePath( lpNmListView->iItem, lpszFilePath ) )
				{
					// Successfully got file path

					// Call selection changed function
					( *lpSelectionChangedFunction )( lpszFilePath );

					// Update return value
					bResult = TRUE;

				} // End of successfully got file path

				// Free string memory
				delete [] lpszFilePath;

			} // End of item state has changed to selected

			// Break out of switch
			break;

		} // End of a list view window item changed notification code
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for tree view window notification code

	return bResult;

} // End of function ListViewWindowHandleNotifyMessage

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list view window
	return ::MoveWindow( g_hWndListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListViewWindowMove

HWND ListViewWindowSetFocus()
{
	// Focus on list view window
	return ::SetFocus( g_hWndListView );

} // End of function ListViewWindowSetFocus

void ListViewWindowSetFont( HFONT hFont )
{
	// Set list view window font
	::SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListViewWindowSetFont

void ListViewWindowSetImageList( HIMAGELIST hImageList )
{
	// Set image lists
	SendMessage( g_hWndListView, LVM_SETIMAGELIST, ( WPARAM )LVSIL_NORMAL, ( LPARAM )hImageList );
	SendMessage( g_hWndListView, LVM_SETIMAGELIST, ( WPARAM )LVSIL_SMALL, ( LPARAM )hImageList );

} // End of function ListViewWindowSetImageList