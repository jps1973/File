// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
static HWND g_hWndListView;

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

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance, HFONT hFont )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView = CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, LIST_VIEW_WINDOW_CLASS_NAME, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 100, 100, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView )
	{
		// Successfully created list view window

		// Set list view window font
		SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window

	return bResult;

} // End of function ListViewWindowCreate

BOOL ListViewWindowMove( int nLeft, int nTop, int nWidth, int nHeight )
{
	// Move list view window
	return MoveWindow( g_hWndListView, nLeft, nTop, nWidth, nHeight, TRUE );

} // End of function ComboBoxWindowMove
