// ControlWindow.cpp

#include "ControlWindow.h"

int ControlWindowAutoSizeAllColumns( HWND hWndControl )
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through all columns
	for( nWhichColumn = 0; nWhichColumn < CONTROL_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Auto-size current column
		SendMessage( hWndControl, LVM_SETCOLUMNWIDTH, ( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER );

	}; // End of loop through all columns

	return nResult;

} // End of function ControlWindowAutoSizeAllColumns

HWND ControlWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	HWND hWndControl;

	// Create control window
	hWndControl = CreateWindowEx( CONTROL_WINDOW_EXTENDED_STYLE, CONTROL_WINDOW_CLASS_NAME, CONTROL_WINDOW_TEXT, CONTROL_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that control window was created
	if( hWndControl )
	{
		// Successfully created control window
		LVCOLUMN lvColumn;
		int nWhichColumn;
		LPCTSTR lpszColumnTitles [] = CONTROL_WINDOW_COLUMN_TITLES;

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initialise list view column structure
		lvColumn.mask	= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
		lvColumn.cx		= CONTROL_WINDOW_DEFAULT_COLUMN_WIDTH;

		// Set extended list view window style
		SendMessage( hWndControl, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )0, ( LPARAM )CONTROL_WINDOW_EXTENDED_STYLE );

		// Loop through all columns
		for( nWhichColumn = 0; nWhichColumn < CONTROL_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
		{
			// Update list view column structure for current column
			lvColumn.pszText = ( LPTSTR )( lpszColumnTitles[ nWhichColumn ] );

			// Add column to list view window
			SendMessage( hWndControl, LVM_INSERTCOLUMN, ( WPARAM )nWhichColumn, ( LPARAM )&lvColumn );

		}; // End of loop through all columns

	} // End of successfully created control window

	return hWndControl;

} // End of function ControlWindowCreate

BOOL ControlWindowGetItemPath( HWND hWndControl, int nWhichItem, LPCTSTR lpszParentFolderPath, LPTSTR lpszItemPath )
{
	BOOL bResult = FALSE;

	LVITEM lvItem;

	// Copy parent folder path into item path
	lstrcpy( lpszItemPath, lpszParentFolderPath );

	// Ensure that item path ends in a back-slash
	if( lpszParentFolderPath[ lstrlen( lpszParentFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
	{
		// Item path does not end in a back-slash

		// Append a back-slash onto item path
		lstrcat( lpszItemPath, ASCII_BACK_SLASH_STRING );

	} // End of item path does not end in a back-slash

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= CONTROL_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= ( lpszItemPath + lstrlen( lpszItemPath ) );

	// Get item from list view window
	bResult = SendMessage( hWndControl, LVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function ControlWindowGetItemPath

BOOL ControlWindowHandleNotifyMessage( HWND hWndControl, LPTSTR lpszParentFolderPath, WPARAM, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMLISTVIEW lpNmListView;

	// Get list view notify message handler
	lpNmListView = ( LPNMLISTVIEW )lParam;

	// Select control window notification code
	switch( lpNmListView->hdr.code )
	{
		case NM_DBLCLK:
		{
			// A double click notify message

			// Allocate string memory
			LPTSTR lpszFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get file path
			if( ControlWindowGetItemPath( hWndControl, lpNmListView->iItem, lpszParentFolderPath, lpszFilePath ) )
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
/*
			// Sort the list view
			ListView_SortItemsEx( g_hWndListView, &ListViewWindowCompare, lpNmListView->iSubItem );
*/
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
				if( ControlWindowGetItemPath( hWndControl, lpNmListView->iItem, lpszParentFolderPath, lpszItemPath ) )
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

	}; // End of selection for control window notification code

	return bResult;

} // End of function ControlWindowHandleNotifyMessage

int ControlWindowPopulate( HWND hWndControl, LPCTSTR lpszFolderPath )
{
	int nResult = 0;

	WIN32_FIND_DATA wfd;
	HANDLE hFileFind = INVALID_HANDLE_VALUE;

	// Allocate string memory
	LPTSTR lpszParentFolderPath		= new char[ STRING_LENGTH + sizeof( char ) ];
	LPTSTR lpszFullSearchPattern	= new char[ STRING_LENGTH + sizeof( char ) ];

	// Store parent folder path
	lstrcpy( lpszParentFolderPath, lpszFolderPath );

	// Ensure that parent folder path ends with a back-slash
	if( lpszParentFolderPath[ lstrlen( lpszParentFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
	{
		// Parent folder path does not end with a back-slash

		// Append a back-slash onto parent folder
		lstrcat( lpszParentFolderPath, ASCII_BACK_SLASH_STRING );

	} // End of parent folder path does not end with a back-slash

	// Copy parent folder path into full search pattern
	lstrcpy( lpszFullSearchPattern, lpszParentFolderPath );

	// Append all files filter onto full search pattern
	lstrcat( lpszFullSearchPattern, ALL_FILES_FILTER );

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
		lvItem.mask			= LVIF_TEXT;
		lvItem.cchTextMax	= STRING_LENGTH;
		lvItem.iItem		= 0;

		// Delete all items from control window
		SendMessage( hWndControl, LB_RESETCONTENT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Loop through all items
		do
		{
			// See if found item is a file
			if( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				// Found item is a file

				// Update list view item structure for found item name
				lvItem.iSubItem	= CONTROL_WINDOW_NAME_COLUMN_ID;
				lvItem.pszText	= wfd.cFileName;

				// Add found item to list view window
				SendMessage( hWndControl, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

				// Update return value
				nResult ++;

			} // End of found item is a file

		} while( FindNextFile( hFileFind, &wfd ) != 0 ); // End of loop through all items

		// Auto-size all columns
		ControlWindowAutoSizeAllColumns( hWndControl );

		// Close file find
		FindClose( hFileFind );

	} // End of successfully found first item

	return nResult;

} // End of function ControlWindowPopulate
