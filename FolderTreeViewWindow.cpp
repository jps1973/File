// FolderTreeViewWindow.cpp

#include "FolderTreeViewWindow.h"

// Global variables
HWND g_hWndFolderTreeView;

BOOL IsFolderTreeViewWindow( HWND hWnd )
{
	// See if supplied window is folder tree view window
	return( hWnd == g_hWndFolderTreeView );

} // End of function IsFolderTreeViewWindow

int FolderTreeViewWindowAddSubFolders( HTREEITEM htiParent )
{
	int nResult = 0;

	// Allocate string memory
	LPTSTR lpszParentFolderPath = new char[ STRING_LENGTH + sizeof( char ) ];

	// Get parent folder path
	if( FolderTreeViewWindowGetItemPath( htiParent, lpszParentFolderPath ) )
	{
		// Successfully got parent folder path
		WIN32_FIND_DATA wfd;
		HANDLE hFileFind;

		// Allocate string memory
		LPTSTR lpszFullSearchPattern = new char[ STRING_LENGTH + sizeof( char ) ];

		// Ensure that parent folder path ends with a back-slash
		if( lpszParentFolderPath[ lstrlen( lpszParentFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
		{
			// Parent folder path does not end with a back-slash

			// Append a back-slash onto parent folder path
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
			HTREEITEM htiFolder;
			TVINSERTSTRUCT tvis;

			// Allocate string memory
			LPTSTR lpszFolderName = new char[ STRING_LENGTH + sizeof( char ) ];

			// Clear tree view insert structure
			ZeroMemory( &tvis, sizeof( tvis ) );

			// Initialise tree view insert structure
			tvis.item.mask		= TVIF_TEXT;
			tvis.item.pszText	= lpszFolderName;
			tvis.hParent		= htiParent;
			tvis.hInsertAfter	= TVI_SORT;

			// Loop through all items
			do
			{
				// See if item is a folder
				if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					// Item is a folder

					// Ensure that item is not dots
					if( wfd.cFileName[ 0 ] != ASCII_FULL_STOP_CHARACTER )
					{
						// Item is not dots

						// Update tree view insert structure for folder
						tvis.hParent = htiParent;

						// Update folder name for item
						lstrcpy( lpszFolderName, wfd.cFileName );

						// Insert folder
						htiFolder = ( HTREEITEM )SendMessage( g_hWndFolderTreeView, TVM_INSERTITEM, ( WPARAM )NULL, ( LPARAM )&tvis );

						// Ensure that folder was inserted
						if( htiFolder )
						{
							// Successfully inserted folder

							// Update tree view insert structure for dummy sub-item
							tvis.hParent = htiFolder;
							// Note that dummy sub-item will never be seen (it is only there to allow the parent
							// item to expand) so we won't update the name

							// Insert dummy sub-item
							SendMessage( g_hWndFolderTreeView, TVM_INSERTITEM, ( WPARAM )NULL, ( LPARAM )&tvis );

						} // End of successfully inserted folder

					} // End of item is not dots

				} // End of item is a folder

			}while( FindNextFile( hFileFind, &wfd ) != 0 );

			// Close file find
			FindClose( hFileFind );

			// Free string memory
			delete lpszFolderName;

		} // End of successfully found first item

		// Free string memory
		delete lpszFullSearchPattern;

	} // End of successfully got parent folder path
	
	// Free string memory
	delete [] lpszParentFolderPath;

	return nResult;

} // End of function FolderTreeViewWindowAddSubFolders

BOOL FolderTreeViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create folder tree view window
	g_hWndFolderTreeView = CreateWindowEx( FOLDER_TREE_VIEW_WINDOW_EXTENDED_STYLE, FOLDER_TREE_VIEW_WINDOW_CLASS_NAME, FOLDER_TREE_VIEW_WINDOW_TEXT, FOLDER_TREE_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that folder tree view window was created
	if( g_hWndFolderTreeView )
	{
		// Successfully created folder tree view window

		// Update return value
		bResult = TRUE;

	} // End of successfully created folder tree view window
	return bResult;

} // End of function FolderTreeViewWindowCreate

int FolderTreeViewWindowDeleteAllSubItems( HTREEITEM htiParent )
{
	int nResult = 0;

	HTREEITEM htiChild;

	// Loop through child items
	do
	{
		// Get next child item
		htiChild = ( HTREEITEM )SendMessage( g_hWndFolderTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CHILD, ( LPARAM )htiParent );

		// Ensure that next child item was got
		if( htiChild )
		{
			// Successfully got next child item

			// Delete child item
			if( SendMessage( g_hWndFolderTreeView, TVM_DELETEITEM, ( WPARAM )NULL, ( LPARAM )htiChild ) )
			{
				// Successfully deleted child item

				// Update return value
				nResult ++;

			} // End of successfully deleted child item

		} // End of successfully got next child item

	} while( htiChild ); // End of loop through child items

	return nResult;

} // End of function FolderTreeViewWindowDeleteAllSubItems

BOOL FolderTreeViewWindowGetItemPath( HTREEITEM htiCurrent, LPTSTR lpszItemPath, DWORD dwMaximumTextLength )
{
	BOOL bResult = TRUE; // Assume success

	TVITEM tvi;

	// Allocate string memory
	LPTSTR lpszTemporary = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear temporary string
	lpszTemporary[ 0 ] = ( char )NULL;

	// Clear tree view item structure
	ZeroMemory( &tvi, sizeof( tvi ) );

	// Initialise tree view item structure
	tvi.mask		= TVIF_TEXT;
	tvi.pszText		= lpszItemPath;
	tvi.cchTextMax	= dwMaximumTextLength;
	tvi.hItem		= htiCurrent;
	
	// Loop through all items
	while( tvi.hItem )
	{
		// Get item text into path
		if( SendMessage( g_hWndFolderTreeView, TVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&tvi ) )
		{
			// Successfully got item text into path

			// See if temporary string contains text
			if( lpszTemporary[ 0 ] )
			{
				// Temporary string contains text

				// Ensure that item path ends with a back-slash
				if( lpszItemPath[ lstrlen( lpszItemPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
				{
					// Item path does not end with a back-slash

					// Append a back-slash onto item path
					lstrcat( lpszItemPath, ASCII_BACK_SLASH_STRING );

				} // End of item path does not end with a back-slash

				// Append temporary string onto item path
				lstrcat( lpszItemPath, lpszTemporary );

			} // End of temporary string contains text

			// Copy item path into temporary string
			lstrcpy( lpszTemporary, lpszItemPath );

			// Get parent tree view item
			tvi.hItem = ( HTREEITEM )SendMessage( g_hWndFolderTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_PARENT, ( LPARAM )tvi.hItem );

		} // End of successfully got item text into path with a back-slash

		else
		{
			// Unable to get item text into path

			// Update return value
			bResult = FALSE;

			// Force exit from loop
			tvi.hItem = NULL;

		} // End of unable to get item text into path

	} // End of loop through all items

	// Free string memory
	delete [] lpszTemporary;

	return bResult;

} // End of function FolderTreeViewWindowGetItemPath

BOOL FolderTreeViewWindowGetItemText( HTREEITEM htiCurrent, LPTSTR lpszItemText, DWORD dwMaximumTextLength )
{
	BOOL bResult;

	TVITEM tvi;

	// Clear tree view item structure
	ZeroMemory( &tvi, sizeof( tvi ) );

	// Initialise tree view item structure
	tvi.mask		= TVIF_TEXT;
	tvi.pszText		= lpszItemText;
	tvi.cchTextMax	= dwMaximumTextLength;
	tvi.hItem		= htiCurrent;

	// Get item text
	bResult = SendMessage( g_hWndFolderTreeView, TVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&tvi );

	return bResult;

} // End of function FolderTreeViewWindowGetItemText

BOOL FolderTreeViewWindowGetRect( LPRECT lpRect )
{
	// Get folder tree view window rect
	return GetWindowRect( g_hWndFolderTreeView, lpRect );

} // End of function FolderTreeViewWindowGetRect

BOOL FolderTreeViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ), void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMTREEVIEW lpNmTreeView;

	// Get tree view notify message information
	lpNmTreeView = ( LPNMTREEVIEW )lParam;

	// Select tree view window notification code
	switch( lpNmTreeView->hdr.code )
	{
		case TVN_ITEMEXPANDING:
		{
			// An item expanding notify message

			// See if item is expanding (rather than collapsing)
			if( lpNmTreeView->action == TVE_EXPAND )
			{
				// Item is expanding (rather than collapsing)

				// Allocate string memory
				LPTSTR lpszItemPath = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get item path
				if( FolderTreeViewWindowGetItemPath( lpNmTreeView->itemNew.hItem, lpszItemPath ) )
				{
					// Successfully got item path

					// Delete all sub items from folder tree window
					FolderTreeViewWindowDeleteAllSubItems( lpNmTreeView->itemNew.hItem );

					// Add sub-folders to folder tree window
					FolderTreeViewWindowAddSubFolders( lpNmTreeView->itemNew.hItem );

					// Update return value
					bResult = TRUE;

				} // End of successfully got item path

				// Free string memory
				delete [] lpszItemPath;

			} // End of item is expanding (rather than collapsing)

			// Break out of switch
			break;

		} // End of an item expanding notify message
		case TVN_SELCHANGED:
		{
			// A column click notify message

			// Allocate string memory
			LPTSTR lpszItemPath = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get item path
			if( FolderTreeViewWindowGetItemPath( lpNmTreeView->itemNew.hItem, lpszItemPath ) )
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

		} // End of a column click notify message
		case NM_DBLCLK:
		{
			// A double click notification code
			HTREEITEM htiSelected;

			// Get selected tree item
			htiSelected = ( HTREEITEM )SendMessage( g_hWndFolderTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CARET, ( LPARAM )NULL );

			// Ensure that selected tree item was got
			if( htiSelected )
			{
				// Successfully got selected tree item

				// Allocate string memory
				LPTSTR lpszItemPath = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get item path
				if( FolderTreeViewWindowGetItemPath( htiSelected, lpszItemPath ) )
				{
					// Successfully got item path

					// Call double click function
					( *lpDoubleClickFunction )( lpszItemPath );

					// Update return value
					bResult = TRUE;

				} // End of successfully got item path

				// Free string memory
				delete [] lpszItemPath;

			} // End of successfully got selected tree item

			// Break out of switch
			break;

		} // End of a double click notification code
		case NM_RCLICK:
		{
			// A right click message
			HTREEITEM htiHighlighted;

			// Get highlighted tree item
			htiHighlighted = ( HTREEITEM )SendMessage( g_hWndFolderTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_DROPHILITE, ( LPARAM )0 );

			// Ensure that highlighted tree item was got
			if( htiHighlighted )
			{
				// Successfully got highlighted tree item

				// Select highlighted tree item
				SendMessage( g_hWndFolderTreeView, TVM_SELECTITEM, ( WPARAM )TVGN_CARET, ( LPARAM )htiHighlighted );

			} // End of successfully got highlighted tree item

			// Break out of switch
			break;

		} // End of a right click message
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for list view window notification code

	return bResult;

} // End of function FolderTreeViewWindowHandleNotifyMessage

HTREEITEM FolderTreeViewWindowInsertItem( LPCTSTR lpszItemText, HTREEITEM htiParent, HTREEITEM htiInsertAfter )
{
	HTREEITEM htiResult;

	TVINSERTSTRUCT tvis;

	// Clear tree view insert structure
	ZeroMemory( &tvis, sizeof( tvis ) );

	// Initialise tree view insert structure
	tvis.item.mask		= TVIF_TEXT;
	tvis.item.pszText	= ( LPTSTR )lpszItemText;
	tvis.hParent		= htiParent;
	tvis.hInsertAfter	= htiInsertAfter;

	// Insert item
	htiResult = ( HTREEITEM )SendMessage( g_hWndFolderTreeView, TVM_INSERTITEM, ( WPARAM )NULL, ( LPARAM )&tvis );

	return htiResult;

} // End of function FolderTreeViewWindowInsertItem

int FolderTreeViewWindowLoad( LPCTSTR lpszFileName, HTREEITEM htiParent, HTREEITEM htiInsertAfter )
{
	int nResult = 0;

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
				LPTSTR lpszFolderPath;
				HTREEITEM htiFolder;

				// Terminate file text
				lpszFileText[ dwFileSize ] = ( char )NULL;

				// Get first folder path
				lpszFolderPath = strtok( lpszFileText, NEW_LINE_TEXT );
				
				// Loop through all folder paths
				while( lpszFolderPath )
				{
					// Add folder path to folder tree view window
					htiFolder = FolderTreeViewWindowInsertItem( lpszFolderPath, htiParent, htiInsertAfter );

					// Ensure that folder path was added to folder tree view window
					if( htiFolder )
					{
						// Successfully added folder path to folder tree view window

						// Add dummy sub-item under folder path
						FolderTreeViewWindowInsertItem( FOLDER_TREE_VIEW_WINDOW_DUMMY_ITEM_TEXT, htiFolder, htiInsertAfter );

						// Update return value
						nResult ++;

						// Get next folder path
						lpszFolderPath = strtok( NULL, NEW_LINE_TEXT );

					} // End of successfully added folder path to folder tree view window
					else
					{
						// Unable to add folder path to folder tree view window

						// Force exit from loop
						lpszFolderPath = NULL;

					} // End of unable to add folder path to folder tree view window

				}; // End of loop through all folder paths

			} // End of successfully read file text

			// Free string memory
			delete [] lpszFileText;

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return nResult;

} // End of function FolderTreeViewWindowLoad

BOOL FolderTreeViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move folder tree view window
	return MoveWindow( g_hWndFolderTreeView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function FolderTreeViewWindowMove

int FolderTreeViewWindowSave( LPCTSTR lpszFileName, HTREEITEM htiParent )
{
	int nResult = 0;

	HANDLE hFile;

	// Create file
	hFile = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	// Ensure that file was created
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully created file
		HTREEITEM htiCurrent;
		DWORD dwItemTextLength;
		DWORD dwNewLineTextLength;

		// Allocate string memory
		LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

		// Store text lengths
		dwNewLineTextLength = lstrlen( NEW_LINE_TEXT );

		// Get first child item
		htiCurrent = ( HTREEITEM )SendMessage( g_hWndFolderTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CHILD, ( LPARAM )htiParent );
		
		// Loop through all child items
		while( htiCurrent )
		{
			// Get item text
			if( FolderTreeViewWindowGetItemText( htiCurrent, lpszItemText ) )
			{
				// Successfully got item text

				// Get item text length
				dwItemTextLength = lstrlen( lpszItemText );

				// Write item text to file
				if( WriteFile( hFile, lpszItemText, dwItemTextLength, NULL, NULL ) )
				{
					// Successfully wrote item text to file

					// Write new line text to file
					WriteFile( hFile, NEW_LINE_TEXT, dwNewLineTextLength, NULL, NULL );

					// Update return value
					nResult ++;

				} // End of successfully wrote item text to file
				else
				{
					// Unable to write item text to file

					// Force exit from loop
					htiCurrent = NULL;

				} // End of unable to write item text to file

				// Get next child item
				htiCurrent = ( HTREEITEM )SendMessage( g_hWndFolderTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_NEXT, ( LPARAM )htiCurrent );

			} // End of successfully got item text
			else
			{
				// Unable to get item text

				// Force exit from loop
				htiCurrent = NULL;

			} // End of unable to get item text

		}; // End of loop through all child items

		// Free string memory
		delete [] lpszItemText;

		// Close file
		CloseHandle( hFile );

	} // End of successfully created file

	return nResult;

} // End of function FolderTreeViewWindowSave

HWND FolderTreeViewWindowSetFocus()
{
	// Focus on folder tree view window
	return SetFocus( g_hWndFolderTreeView );

} // End of function FolderTreeViewWindowSetFocus

void FolderTreeViewWindowSetFont( HFONT hFont )
{
	// Set folder tree view window font
	SendMessage( g_hWndFolderTreeView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function FolderTreeViewWindowSetFont
