// FolderTreeViewWindow.cpp

#include "FolderTreeViewWindow.h"

// Global variables
HWND g_hWndFolderTreeView;

BOOL IsFolderTreeViewWindow( HWND hWnd )
{
	// See if supplied window is folder tree view window
	return( hWnd == g_hWndFolderTreeView );

} // End of function IsFolderTreeViewWindow

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

BOOL FolderTreeViewWindowHandleCommandMessage( WPARAM wParam, LPARAM, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select folder tree view window notification code
	switch( HIWORD( wParam ) )
	{
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for folder tree view window notification code

	return bResult;

} // End of function FolderTreeViewWindowHandleCommandMessage

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

		/*
		DWORD dwTextLength;
		dwTextLength = GetWindowTextLength(hEdit);
		// No need to bother if there's no text.
		if(dwTextLength > 0)
		{
		LPSTR pszText;
		DWORD dwBufferSize = dwTextLength + 1;
		pszText = GlobalAlloc(GPTR, dwBufferSize);
		if(pszText != NULL)
		{
		if(GetWindowText(hEdit, pszText, dwBufferSize))
		{
		DWORD dwWritten;
		if(WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
		bSuccess = TRUE;
		}
		GlobalFree(pszText);
		}
		}
		*/

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
