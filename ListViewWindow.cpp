// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
HWND g_hWndListView;

BOOL IsListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndListView );

} // End of function IsListViewWindow

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

			// Add column to list view window
			SendMessage( g_hWndListView, LVM_INSERTCOLUMN, ( WPARAM )lvColumn.iSubItem, ( LPARAM )&lvColumn );

		}; // End of loop through columns

		// Auto-size all columns
		ListViewWindowAutoSizeAllColumns();

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window

	return bResult;

} // End of function ListViewWindowCreate

BOOL ListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return ::GetWindowRect( g_hWndListView, lpRect );

} // End of function ListViewWindowGetRect

BOOL ListViewWindowHandleCommandMessage( WPARAM wParam, LPARAM, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select list view window notification code
	switch( HIWORD( wParam ) )
	{
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for list view window notification code

	return bResult;

} // End of function ListViewWindowHandleCommandMessage

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
