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

	// Clear tree view insert structure
	ZeroMemory( &tvis, sizeof( tvis ) );

	// Initialise tree view insert structure
	tvis.hParent				= TVI_ROOT;
	tvis.hInsertAfter			= TVI_SORT;
	tvis.item.mask				= ( TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE );
	tvis.item.pszText			= ( LPTSTR )lpszItemPath;
	tvis.item.iImage			= 0;
	tvis.item.iSelectedImage	= 1;

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

BOOL TreeViewWindowGetRect( LPRECT lpRect )
{
	// Get tree view window rect
	return ::GetWindowRect( g_hWndTreeView, lpRect );

} // End of function TreeViewWindowGetRect

BOOL TreeViewWindowHandleCommandMessage( WPARAM wParam, LPARAM, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select tree view window notification code
	switch( HIWORD( wParam ) )
	{
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
