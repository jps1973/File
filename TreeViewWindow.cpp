// TreeViewWindow.cpp

#include "TreeViewWindow.h"

// Global variables
HWND g_hWndTreeView;

BOOL IsTreeViewWindow( HWND hWnd )
{
	// See if supplied window is tree view window
	return( hWnd == g_hWndTreeView );

} // End of function IsTreeViewWindow

HTREEITEM TreeViewWindowAddTopLevelItem( LPCTSTR lpszItemPath )
{
	HTREEITEM htiResult = NULL;

	TV_INSERTSTRUCT tvis;
	int nIconIndex;

	// Get icon index
	nIconIndex = ImageListGetIconIndex( lpszItemPath );

	// Clear tree view insert structure
	ZeroMemory( &tvis, sizeof( tvis ) );

	// Initialise tree view insert structure
	tvis.hParent				= TVI_ROOT;
	tvis.hInsertAfter			= TVI_SORT;
	tvis.item.mask				= ( TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE );
	tvis.item.pszText			= ( LPTSTR )lpszItemPath;
	tvis.item.iImage			= nIconIndex;
	tvis.item.iSelectedImage	= nIconIndex;

	// Add top level item
	htiResult = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&tvis );
	
	// Ensure that top level item was added
	if( htiResult )
	{
		// Successfully added top level item

		// Update tree view insert structure for dummy sub item
		tvis.hParent		= htiResult;
		tvis.item.pszText	= ( LPTSTR )TREE_VIEW_WINDOW_DUMMY_ITEM_TEXT;

		// Add dummy sub item
		SendMessage( g_hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&tvis );

	} // End of successfully added top level item

	return htiResult;

} // End of function TreeViewWindowAddTopLevelItem

int TreeViewWindowAddSubFolders( HTREEITEM htiParentFolder )
{
	int nResult = 0;

	// Allocate string memory
	LPTSTR lpszParentFolderPath = new char[ STRING_LENGTH ];

	// Get parent folder path
	if( TreeViewWindowGetItemPath( htiParentFolder, lpszParentFolderPath ) )
	{
		// Successfully got parent folder path
		WIN32_FIND_DATA wfd;
		HANDLE hFind;

		// Allocate string memory
		LPTSTR lpszFullSearchPattern = new char[ STRING_LENGTH ];

		// Copy parent folder path into full search pattern
		lstrcpy( lpszFullSearchPattern, lpszParentFolderPath );

		// Append all files filter onto full search pattern
		lstrcat( lpszFullSearchPattern, ALL_FILES_FILTER );

		// Find first item
		hFind = FindFirstFile( lpszFullSearchPattern, &wfd );

		// Ensure that first item was found
		if(hFind != INVALID_HANDLE_VALUE )
		{
			// Successfully found first item
			TV_INSERTSTRUCT tvis;
			HTREEITEM htiSubFolder;
			int nIconIndex;

			// Allocate string memory
			LPTSTR lpszSubFolderPath = new char[ STRING_LENGTH ];

			// Clear tree view insert structure
			ZeroMemory( &tvis, sizeof( tvis ) );

			// Initialise tree view insert structure
			tvis.hInsertAfter			= TVI_SORT;
			tvis.item.mask				= ( TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE );

			// Delete sub items from tree view window
			TreeViewWindowDeleteAllSubItems( htiParentFolder );

			// Loop through all items
			do
			{
				// See if current item is a folder
				if ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					// Current item is a folder

					// Ensure that current item is not dots
					if( wfd.cFileName[ 0 ] != ASCII_FULL_STOP_CHARACTER )
					{
						// Current item is not dots

						// Copy parent folder path into sub-folder path
						lstrcpy( lpszSubFolderPath, lpszParentFolderPath );

						// Append sub-folder name onto sub-folder path
						lstrcat( lpszSubFolderPath, wfd.cFileName );

						// Get icon index
						nIconIndex = ImageListGetIconIndex( lpszSubFolderPath );

						// Update tree view insert structure for sub-folder item
						tvis.hParent				= htiParentFolder;
						tvis.item.pszText			= ( LPTSTR )wfd.cFileName;
						tvis.item.iImage			= nIconIndex;
						tvis.item.iSelectedImage	= nIconIndex;

						// Add sub-folder item
						htiSubFolder = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&tvis );

						// Ensure that sub-folder item was added
						if( htiSubFolder )
						{
							// Successfully added sub-folder item

							// Update tree view insert structure for dummy sub-item
							tvis.hParent				= htiSubFolder;
							tvis.item.pszText			= ( LPTSTR )TREE_VIEW_WINDOW_DUMMY_ITEM_TEXT;

							// Add dummy sub-item
							SendMessage( g_hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&tvis );

							// Update return value
							nResult ++;

						} // End of successfully added sub-folder item

					} // End of current item is not dots

				} // End of current item is a folder

			} while( FindNextFile( hFind, &wfd ) != 0 ); // End of loop through all items

			// Close file find
			FindClose( hFind );

			// Free string memory
			delete [] lpszSubFolderPath;

		} // End of successfully found first item
		else
		{
			// Unable to find first item

			// This means that we don't have access to the folder

			// Delete sub items from tree view window
			TreeViewWindowDeleteAllSubItems( htiParentFolder );

		} // End of unable to find first item

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
	g_hWndTreeView = ::CreateWindowEx( TREE_VIEW_WINDOW_EXTENDED_STYLE, TREE_VIEW_WINDOW_CLASS_NAME, TREE_VIEW_WINDOW_TEXT, TREE_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that tree view window was created
	if( g_hWndTreeView )
	{
		// Successfully created tree view window

		// Update return value
		bResult = TRUE;

	} // End of successfully created tree view window

	return bResult;

} // End of function TreeViewWindowCreate

int TreeViewWindowDeleteAllSubItems( HTREEITEM htiParentItem )
{
	int nResult = 0;

	HTREEITEM htiSubItem;

	// Get first sub-item
	htiSubItem = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CHILD, ( LPARAM )htiParentItem );

	// Loop through all sub-items
	while( htiSubItem )
	{
		// Delete sub-item
		if( SendMessage( g_hWndTreeView, TVM_DELETEITEM, ( WPARAM )0, ( LPARAM )htiSubItem ) )
		{
			// Successfully deleted sub-item

			// Update return value
			nResult ++;

			// Get next sub-item
			htiSubItem = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CHILD, ( LPARAM )htiParentItem );

		} // End of successfully deleted sub-item
		else
		{
			// Unable to delete sub-item

			// Force exit from loop
			htiSubItem = NULL;

		} // End of unable to delete sub-item

	}; // End of loop through all sub-items

	return nResult;

} // End of function TreeViewWindowDeleteAllSubItems

BOOL TreeViewWindowExpandItem( HTREEITEM htiItem )
{
	// Expand item
	return SendMessage( g_hWndTreeView, TVM_EXPAND, ( WPARAM )TVE_EXPAND, ( LPARAM )htiItem );

} // End of function TreeViewWindowExpandItem
BOOL TreeViewWindowGetItemPath( HTREEITEM htiItem, LPTSTR lpszItemPath )
{
	BOOL bResult = FALSE;

	TVITEM tvItem;

	// Allocate string memory
	LPTSTR lpszTemp = new char[ STRING_LENGTH ];

	// Clear tree view item structure
	ZeroMemory( &tvItem, sizeof( tvItem ) );

	// Initialise tree view item structure
	tvItem.mask			= TVIF_TEXT;
	tvItem.pszText		= lpszItemPath;
	tvItem.cchTextMax	= STRING_LENGTH;
	tvItem.hItem		= htiItem;

	// Clear item path
	lpszItemPath[ 0 ] = ( char )NULL;

	// Loop through all items
	while( tvItem.hItem )
	{
		// Copy item path onto temp string
		lstrcpy( lpszTemp, lpszItemPath );

		// Get item text into item path
		if( SendMessage( g_hWndTreeView, TVM_GETITEM, ( WPARAM )0, ( LPARAM )&tvItem ) )
		{
			// Successfully got item text into item path

			// Ensure thet item path ends with a back-slash
			if( lpszItemPath[ lstrlen( lpszItemPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
			{
				// Item path does not end with a back-slash

				// Append a back-slash onto item path
				lstrcat( lpszItemPath, ASCII_BACK_SLASH_STRING );

			} // End of item path does not end with a back-slash

			// Append temp string onto item path
			lstrcat( lpszItemPath, lpszTemp );

			// Update return value
			bResult = TRUE;

			// Get parent tree item
			tvItem.hItem = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_PARENT, ( LPARAM )tvItem.hItem );

		} // End of successfully got item text into item path
		else
		{
			// Unable to get item text into item path

			// Force exit from loop
			tvItem.hItem = NULL;

		} // End of unable to get item text into item path

	}; // End of loop through all items

	// Free string memory
	delete [] lpszTemp;

	return bResult;

} // End of function TreeViewWindowGetItemPath

BOOL TreeViewWindowGetItemText( HTREEITEM htiItem, LPTSTR lpszItemText )
{
	BOOL bResult;

	TVITEM tvItem;

	// Clear tree view item structure
	ZeroMemory( &tvItem, sizeof( tvItem ) );

	// Initialise tree view item structure
	tvItem.mask			= TVIF_TEXT;
	tvItem.pszText		= lpszItemText;
	tvItem.cchTextMax	= STRING_LENGTH;
	tvItem.hItem		= htiItem;

	// Get item text
	bResult = SendMessage( g_hWndTreeView, TVM_GETITEM, ( WPARAM )0, ( LPARAM )&tvItem );

	return bResult;

} // End of function TreeViewWindowGetItemText

BOOL TreeViewWindowGetRect( LPRECT lpRect )
{
	// Get tree view window rect
	return ::GetWindowRect( g_hWndTreeView, lpRect );

} // End of function TreeViewWindowGetRect

BOOL TreeViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpSelectionChangedFunction )( LPCTSTR lpszItemPath ) )
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmHdr;

	// Get notify message handler
	lpNmHdr = ( LPNMHDR )lParam;

	// Select tree view window notification code
	switch( lpNmHdr->code )
	{
		case NM_RCLICK:
		{
			// A right click notification code
			HTREEITEM htiHighlighted;

			// Get highlighted tree item
			htiHighlighted = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_DROPHILITE, ( LPARAM )0 );

			// Ensure that highlighted tree item was got
			if( htiHighlighted )
			{
				// Successfully got highlighted tree item

				// Select highlighted tree item
				SendMessage( g_hWndTreeView, TVM_SELECTITEM, ( WPARAM )TVGN_CARET, ( LPARAM )htiHighlighted );

			} // End of successfully got highlighted tree item

			// Break out of switch
			break;

		} // End of a right click notification code
		case TVN_ITEMEXPANDING:
		{
			// A tree view item expanding notification code
			LPNMTREEVIEW lpNmTreeView;

			// Get tree view notification message handler
			lpNmTreeView = ( LPNMTREEVIEW )lParam;

			// Add sub-folders to tree view window
			TreeViewWindowAddSubFolders( lpNmTreeView->itemNew.hItem );

			// Break out of switch
			break;

		} // End of a tree view item expanding notification code
		case TVN_SELCHANGED:
		{
			// A tree view selection changed notification code
			LPNMTREEVIEW lpNmTreeView;

			// Allocate string memory
			LPTSTR lpszItemPath = new char[ STRING_LENGTH ];

			// Get tree view notification message handler
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

			// Free string memory
			delete [] lpszItemPath;

			// Break out of switch
			break;

		} // End of a tree view selection changed notification code
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for tree view window notification code

	return bResult;

} // End of function TreeViewWindowHandleCommandMessage

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

void TreeViewWindowSetImageList( HIMAGELIST hImageList )
{
	// Set image lists
	::SendMessage( g_hWndTreeView, TVM_SETIMAGELIST, ( WPARAM )TVSIL_NORMAL, ( LPARAM )hImageList );
	::SendMessage( g_hWndTreeView, TVM_SETIMAGELIST, ( WPARAM )TVSIL_STATE, ( LPARAM )hImageList );

} // End of function TreeViewWindowSetImageList
