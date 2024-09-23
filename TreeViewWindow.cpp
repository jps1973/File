// TreeViewWindow.cpp

#include "TreeViewWindow.h"

// Global variables
static HWND g_hWndTreeView;

BOOL IsTreeViewWindow( HWND hWnd )
{
	// See if supplied window is tree view window
	return( hWnd == g_hWndTreeView );

} // End of function IsTreeViewWindow

HTREEITEM TreeViewWindowAddItem( LPCTSTR lpszItemText, HTREEITEM htiParent, HTREEITEM htiInsertAfter )
{
	HTREEITEM htiResult;

	TVINSERTSTRUCT tvis;

	// Clear tree view insert structure
	ZeroMemory( &tvis, sizeof( tvis ) );

	// Initialise tree view insert structure
	tvis.hParent		= htiParent;
	tvis.hInsertAfter	= htiInsertAfter;
	tvis.item.mask		= TVIF_TEXT;
	tvis.item.pszText	= ( LPTSTR )lpszItemText;

	// Insert item
	htiResult = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&tvis );

	// Ensure that item was inserted
	if( htiResult )
	{
		// Successfully inserted item

		// Update  tree view insert structure for dummy sub-item
		tvis.hParent = htiResult;

		// Insert  dummy sub-item
		SendMessage( g_hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&tvis );

	} // End of successfully inserted item

	return htiResult;

} // End of function TreeViewWindowAddItem

int TreeViewWindowAddSubFolders( HTREEITEM htiParent )
{
	int nResult = 0;

	// Allocate string memory
	LPTSTR lpszParentFolderPath = new char[ STRING_LENGTH ];

	// Get parent folder path
	if( TreeViewWindowGetItemPath( htiParent, lpszParentFolderPath ) )
	{
		// Successfully got parent folder path
		WIN32_FIND_DATA wfd;
		HANDLE hFind;
		TVINSERTSTRUCT tvis;
		HTREEITEM htiFolder;

		// Clear tree view insert structure
		ZeroMemory( &tvis, sizeof( tvis ) );

		// Initialise tree view insert structure
		tvis.hInsertAfter	= TVI_SORT;
		tvis.item.mask		= TVIF_TEXT;

		// Allocate string memory
		LPTSTR lpszFullSearchPattern = new char[ STRING_LENGTH ];

		// Ensure that parent folder path ends with a back-slash
		if( lpszParentFolderPath[ lstrlen( lpszParentFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
		{
			// Parent folder path does not end with a back-slash

			// Append a back-slash onto parent folder path
			lstrcat( lpszParentFolderPath, ASCII_BACK_SLASH_STRING );

		} // End of parent folder path does not end with a back-slash

		// Copy parent folder path into full search pattern
		lstrcpy( lpszFullSearchPattern, lpszParentFolderPath );

		// Append all files filter onto parent folder path
		lstrcat( lpszFullSearchPattern, ALL_FILES_FILTER );

		// Find first item
		hFind = FindFirstFile( lpszFullSearchPattern, &wfd );

		// Ensure that first item was found
		if( hFind != INVALID_HANDLE_VALUE ) 
		{
			// Successfully found first item

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

						// Update tree view insert structure for found folder
						tvis.hParent		= htiParent;
						tvis.item.pszText	= wfd.cFileName;

						// Insert folder
						htiFolder = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&tvis );

						// Ensure that folder was inserted
						if( htiFolder )
						{
							// Successfully inserted folder

							// Update  tree view insert structure for dummy sub-item
							tvis.hParent = htiFolder;

							// Insert  dummy sub-item
							SendMessage( g_hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&tvis );

							// Update return value
							nResult ++;

						} // End of successfully inserted folder

					} // End of found item is not dots

				} // End of found item is a folder

			} while( FindNextFile( hFind, &wfd ) != 0 ); // End of loop through all items

			// Close file find
			FindClose( hFind );

		}  // End of successfully found first item

		// Free string memory
		delete [] lpszFullSearchPattern;

	} // End of successfully got parent folder path

	// Free string memory
	delete [] lpszParentFolderPath;

	return nResult;

} // End of function TreeViewWindowAddSubFolders

BOOL TreeViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create tree view window
	g_hWndTreeView = ::CreateWindowEx( TREE_VIEW_WINDOW_EXTENDED_STYLE, TREE_VIEW_WINDOW_CLASS_NAME, TREE_VIEW_WINDOW_TEXT, TREE_VIEW_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that tree view window was created
	if( g_hWndTreeView )
	{
		// Successfully created tree view window

		// Update return value
		bResult = TRUE;

	} // End of successfully created tree view window

	return bResult;

} // End of function TreeViewWindowCreate

BOOL TreeViewWindowDeleteAllChildItems( HTREEITEM htiParent )
{
	BOOL bResult = TRUE; // Assume success

	HTREEITEM htiChild;

	// Get first child-item
	htiChild = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CHILD, ( LPARAM )htiParent );

	// Loop through all child-items
	while( htiChild )
	{
		// Delete child item
		if( SendMessage( g_hWndTreeView, TVM_DELETEITEM, ( WPARAM )NULL, ( LPARAM )htiChild ) )
		{
			// Successfully deleted child item

			// Get next child-item
			htiChild = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CHILD, ( LPARAM )htiParent );

		} // End of successfully deleted child item
		else
		{
			// Unable to delete child item

			// Update return value
			bResult = FALSE;

			// Force exit from loop
			htiChild = NULL;

		} // End of unable to delete child item

	}; // End of loop through all child-items

	return bResult;

} // End of function TreeViewWindowDeleteAllChildItems

BOOL TreeViewWindowGetItemPath( HTREEITEM htiRequired, LPTSTR lpszItemPath )
{
	BOOL bResult = TRUE; // Assume success

	TVITEM tvi;

	// Allocate string memory
	LPTSTR lpszCurrentItemText	= new char[ STRING_LENGTH ];
	LPTSTR lpszTemporary		= new char[ STRING_LENGTH ];

	// Clear tree view item structure
	ZeroMemory( &tvi, sizeof( tvi ) );

	// Initialise tree view item structure for required item
	tvi.mask		= TVIF_TEXT;
	tvi.pszText		= lpszCurrentItemText;
	tvi.cchTextMax	= STRING_LENGTH;
	tvi.hItem		= htiRequired;

	// Clear item path
	lpszItemPath[ 0 ] = ( char )NULL;

	// Loop through all items
	while( tvi.hItem )
	{
		// Get current item text
		if( SendMessage( g_hWndTreeView, TVM_GETITEM, ( WPARAM )TVGN_CARET, ( LPARAM )&tvi ) )
		{
			// Successfully got current item text

			// Copy item path into temporary string
			lstrcpy( lpszTemporary, lpszItemPath );

			// Copy current item text into item path
			lstrcpy( lpszItemPath, lpszCurrentItemText );

			// Ensure that item path ends in a back-slash
			if( lpszItemPath[ lstrlen( lpszItemPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
			{
				// Item path does not end in a back-slash

				// Append a back-slash onto item path
				lstrcat( lpszItemPath, ASCII_BACK_SLASH_STRING );

			} // End of item path does not end in a back-slash

			// Append temporary string onto item path
			lstrcat( lpszItemPath, lpszTemporary );

			// Get parent item
			tvi.hItem = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_PARENT, ( LPARAM )tvi.hItem );

		} // End of successfully got current item text
		else
		{
			// Unable to get current item text

			// Update return value
			bResult = FALSE;

			// Force exit from loop
			tvi.hItem = NULL;

		} // End of unable to get current item text

	}; // End of loop through all items

	// Free string memory
	delete [] lpszCurrentItemText;
	delete [] lpszTemporary;

	return bResult;

} // End of function TreeViewWindowGetItemPath

BOOL TreeViewWindowGetItemText( HTREEITEM htiRequired, LPTSTR lpszItemText )
{
	BOOL bResult = FALSE;

	TVITEM tvi;

	// Clear tree view item structure
	ZeroMemory( &tvi, sizeof( tvi ) );

	// Initialise tree view item structure
	tvi.mask		= TVIF_TEXT;
	tvi.pszText		= lpszItemText;
	tvi.cchTextMax	= STRING_LENGTH;
	tvi.hItem		= htiRequired;

	// Get item text
	bResult = SendMessage( g_hWndTreeView, TVM_GETITEM, ( WPARAM )TVGN_CARET, ( LPARAM )&tvi );

	return bResult;

} // End of function TreeViewWindowGetItemText

BOOL TreeViewWindowGetRect( LPRECT lpRect )
{
	// Get tree view window rect
	return ::GetWindowRect( g_hWndTreeView, lpRect );

} // End of function TreeViewWindowGetRect

BOOL TreeViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) )
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
			// A double click notification code
			HTREEITEM htiSelected;

			// Allocate string memory
			LPTSTR lpszItemPath = new char[ STRING_LENGTH ];

			// Get selected item
			htiSelected = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CARET, ( LPARAM )NULL );

			// Get item path
			if( TreeViewWindowGetItemPath( htiSelected, lpszItemPath ) )
			{
				// Successfully got item path

				// Call double click function
				( *lpDoubleClickFunction )( lpszItemPath );

				// Update return value
				bResult = TRUE;

			} // End of successfully got item path

			// Break out of switch
			break;

		} // End of a double click notification code
		case TVN_ITEMEXPANDING:
		{
			// A tree view window item expanding notification code
			LPNMTREEVIEW lpNmTreeView;

			// Get tree view notify message handler
			lpNmTreeView = ( LPNMTREEVIEW )lParam;

			// Delete child items
			if( TreeViewWindowDeleteAllChildItems( lpNmTreeView->itemNew.hItem ) )
			{
				// Successfully deleted child items

				// Add sub-folders
				if( TreeViewWindowAddSubFolders( lpNmTreeView->itemNew.hItem ) )
				{
					// Successfully added sub-folders

					// Update return value
					bResult = TRUE;

				} // End of successfully added sub-folders

			} // End of successfully deleted child items

			// Break out of switch
			break;

		} // End of a tree view window item expanding notification code
		case TVN_SELCHANGED:
		{
			// A tree view window selection changed notification code
			LPNMTREEVIEW lpNmTreeView;

			// Allocate string memory
			LPTSTR lpszItemPath = new char[ STRING_LENGTH ];

			// Get tree view notify message handler
			lpNmTreeView = ( LPNMTREEVIEW )lParam;

			// Get item path
			if( TreeViewWindowGetItemPath( lpNmTreeView->itemNew.hItem, lpszItemPath ) )
			{
				// Successfully got item path

				// Call selection changed function
				( *lpSelectionChangedFunction )( lpszItemPath );

				// Update return value
				bResult = TRUE;

			} // End of successfully got item path

			// Break out of switch
			break;

		} // End of a tree view window selection changed notification code
		default:
		{
			// Default tree view window notification code

			// Break out of switch
			break;

		} // End of default tree view window notification code

	}; // End of selection for tree view window notification code

	return bResult;

} // End of function TreeViewWindowHandleNotifyMessage

BOOL TreeViewWindowLoad( LPCTSTR lpszFileName )
{
	BOOL bResult = FALSE;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file
		DWORD dwFileSize;

		// Get file size
		dwFileSize = GetFileSize( hFile, NULL );

		// Ensure that file size was got
		if( dwFileSize != INVALID_FILE_SIZE )
		{
			// Successfully got file size

			// Allocate string memory
			LPTSTR lpszFileText = new char[ dwFileSize + sizeof( char ) ];

			// Read file text
			if( ReadFile( hFile, lpszFileText, dwFileSize, NULL, NULL ) )
			{
				// Successfully read file text
				LPTSTR lpszFolder;

				// Terminate file text
				lpszFileText[ dwFileSize ] = ( char )NULL;

				// Get first folder
				lpszFolder = strtok( lpszFileText, NEW_LINE_TEXT );

				// Loop through all folders
				while( lpszFolder )
				{
					// Add folder to tree view window
					TreeViewWindowAddItem( lpszFolder );

					// Get next folder
					lpszFolder = strtok( NULL, NEW_LINE_TEXT );

				} // End of loop through all folders

				// Update return value
				bResult = TRUE;

			} // End of successfully read file text

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return bResult;

} // End of function TreeViewWindowLoad


BOOL TreeViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move tree view window
	return ::MoveWindow( g_hWndTreeView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function TreeViewWindowMove

HWND TreeViewWindowSetFocus()
{
	// Focus on tree view window
	return ::SetFocus( g_hWndTreeView );

} // End of function TreeViewWindowSetFocus

void TreeViewWindowSetFont( HFONT hFont )
{
	// Set tree view window font
	::SendMessage( g_hWndTreeView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function TreeViewWindowSetFont
