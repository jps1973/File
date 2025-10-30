// ComboBoxWindow.cpp

#include "ComboBoxWindow.h"

// Global variables
static HWND g_hWndComboBox;

BOOL IsComboBoxWindow( HWND hWndSupplied )
{
	BOOL bResult = FALSE;

	// See if supplied window is combo box window
	if( hWndSupplied == g_hWndComboBox )
	{
		// Supplied window is combo box window

		// Update return value
		bResult = TRUE;

	} // End of supplied window is combo box window

	return bResult;

} // End of function IsComboBoxWindow

int ComboBoxWindowAddString( LPCTSTR lpszString )
{
	// Add string to combo box window
	return SendMessage( g_hWndComboBox, CB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszString );

} // End of function ComboBoxWindowAddString

BOOL ComboBoxWindowCreate( HWND hWndParent, HINSTANCE hInstance, HFONT hFont )
{
	BOOL bResult = FALSE;

	// Create combo box window
	g_hWndComboBox = CreateWindowEx( COMBO_BOX_WINDOW_EXTENDED_STYLE, COMBO_BOX_WINDOW_CLASS_NAME, COMBO_BOX_WINDOW_TEXT, COMBO_BOX_WINDOW_STYLE, 0, 0, 100, 100, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that combo box window was created
	if( g_hWndComboBox )
	{
		// Successfully created combo box window

		// Set combo box window font
		SendMessage( g_hWndComboBox, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

		// Update return value
		bResult = TRUE;

	} // End of successfully created combo box window

	return bResult;

} // End of function ComboBoxWindowCreate

BOOL ComboBoxWindowHandleCommandMessage( WPARAM wParam, LPARAM, void( *lpSelectionChangeFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult;

	// Select combo box window notification code
	switch( HIWORD( wParam ) )
	{
		case CBN_SELCHANGE:
		{
			// A combo box window selection change code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelected = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get selected item
			nSelectedItem = SendMessage( g_hWndComboBox, CB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Get selected item text
			if( SendMessage( g_hWndComboBox, CB_GETLBTEXT, ( WPARAM )nSelectedItem, ( LPARAM )lpszSelected ) )
			{
				// Successfully got selected item text

				// Call selection change function
				( *lpSelectionChangeFunction )( lpszSelected );

				// Update return value
				bResult = TRUE;

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelected;

			// Break out of switch
			break;

		} // End of a combo box window selection change notification code
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for combo box window notification code

	return bResult;

} // End of function ComboBoxWindowHandleCommandMessage

BOOL ComboBoxWindowMove( int nLeft, int nTop, int nWidth, int nHeight )
{
	// Move combo box window
	return MoveWindow( g_hWndComboBox, nLeft, nTop, nWidth, nHeight, TRUE );

} // End of function ComboBoxWindowMove

int ComboBoxWindowSelectItem( int nWhichItem, void( *lpSelectionChangeFunction )( LPCTSTR lpszItemText ) )
{
	int nResult;

	// Select combo box item
	nResult = SendMessage( g_hWndComboBox, CB_SETCURSEL, ( WPARAM )nWhichItem, ( LPARAM )NULL );

	// Ensure that combo box item was selected
	if( nResult != CB_ERR )
	{
		// Successfully selected combo box item

		// See if selection change function is valid
		if( lpSelectionChangeFunction )
		{
			// Selection change function is valid

			// Allocate string memory
			LPTSTR lpszSelected = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get selected item text
			if( SendMessage( g_hWndComboBox, CB_GETLBTEXT, ( WPARAM )nWhichItem, ( LPARAM )lpszSelected ) )
			{
				// Successfully got selected item text

				// Call selection change function
				( *lpSelectionChangeFunction )( lpszSelected );

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelected;

		} // End of selection change function is valid

	} // End of successfully selected combo box item

	return nResult;

} // End of function ComboBoxWindowSelectItem
