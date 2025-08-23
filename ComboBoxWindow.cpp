// ComboBoxWindow.cpp

#include "ComboBoxWindow.h"

// Global variables
static HWND g_hWndComboBox;

BOOL IsComboBoxWindow( HWND hWnd )
{
	// See if supplied window is combo box window
	return( hWnd == g_hWndComboBox );

} // End of function IsComboBoxWindow

int ComboBoxWindowAddString( LPCTSTR lpszString )
{
	// Add string to combo box window
	return SendMessage( g_hWndComboBox, CB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszString );

} // End of function ComboBoxWindowAddString

BOOL ComboBoxWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create combo box window
	g_hWndComboBox = CreateWindowEx( COMBO_BOX_WINDOW_EXTENDED_STYLE, COMBO_BOX_WINDOW_CLASS_NAME, COMBO_BOX_WINDOW_TEXT, COMBO_BOX_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that combo box window was created
	if( g_hWndComboBox )
	{
		// Successfully created combo box window

		// Update return value
		bResult = TRUE;

	} // End of successfully created combo box window
	return bResult;

} // End of function ComboBoxWindowCreate

int ComboBoxWindowGetItemCount()
{
	// Add string to combo box window
	return SendMessage( g_hWndComboBox, CB_GETCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function ComboBoxWindowGetItemCount

BOOL ComboBoxWindowGetRect( LPRECT lpRect )
{
	// Get combo box window rect
	return GetWindowRect( g_hWndComboBox, lpRect );

} // End of function ComboBoxWindowGetRect

BOOL ComboBoxWindowHandleCommandMessage( WPARAM wParam, LPARAM, BOOL( *lpSelectionChangeFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select combo box window notification code
	switch( HIWORD( wParam ) )
	{
		case CBN_SELCHANGE:
		{
			// A combo box window selection change notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelected = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get selected item
			nSelectedItem = SendMessage( g_hWndComboBox, CB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Get selected item text
			if( SendMessage( g_hWndComboBox, CB_GETLBTEXT, ( WPARAM )nSelectedItem, ( LPARAM )lpszSelected ) )
			{
				// Successfully got selected item text

				// Show selected item text on status bar window
				( *lpSelectionChangeFunction )( lpszSelected );

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

BOOL ComboBoxWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move combo box window
	return MoveWindow( g_hWndComboBox, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ComboBoxWindowMove

int ComboBoxWindowLoad( LPCTSTR lpszFileName )
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
				LPTSTR lpszLine;

				// Terminate file text
				lpszFileText[ dwFileSize ] = ( char )NULL;

				// Get first line in file text
				lpszLine = strtok( lpszFileText, NEW_LINE_TEXT );

				// Loop through all lines in file text
				while( lpszLine )
				{
					// Add line to combo box window
					if( SendMessage( g_hWndComboBox, CB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszLine ) >= 0 )
					{
						// Successfully added line to combo box window

						// Update return value
						nResult ++;

						// Get next line in file text
						lpszLine = strtok( NULL, NEW_LINE_TEXT );

					} // End of successfully added line to combo box window
					else
					{
						// Unable to add line to combo box window

						// Force exit from loop
						lpszLine = NULL;

					} // End of unable to add line to combo box window

				}; // End of loop through all lines in file text

			} // End of successfully read file text

			// Free string memory
			delete [] lpszFileText;

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return nResult;

} // End of function ComboBoxWindowLoad

int ComboBoxWindowPopulate( LPCTSTR lpszFileName )
{
	int nResult = 0;

	// Clear combo box window
	SendMessage( g_hWndComboBox, CB_RESETCONTENT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Load file
	nResult = ComboBoxWindowLoad( lpszFileName );

	return nResult;

} // End of function ComboBoxWindowPopulate

int ComboBoxWindowSave( LPCTSTR lpszFileName )
{
	int nResult = 0;

	HANDLE hFile;

	// Create file
	hFile = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	// Ensure that file was created
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully created file
		int nItemCount;
		int nWhichItem;

		// Allocate string memory
		LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

		// Count items on combo box window
		nItemCount = SendMessage( g_hWndComboBox, CB_GETCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Loop through items on combo box window
		for( nWhichItem = 0; nWhichItem < nItemCount; nWhichItem ++ )
		{
			// Get item text
			if( SendMessage( g_hWndComboBox, CB_GETLBTEXT, ( WPARAM )nWhichItem, ( LPARAM )lpszItemText) != CB_ERR )
			{
				// Successfully got item text

				// Write item text to file
				if( WriteFile( hFile, lpszItemText, lstrlen( lpszItemText ), NULL, NULL ) )
				{
					// Successfully wrote item text to file

					// Write new line text to file
					WriteFile( hFile, NEW_LINE_TEXT, lstrlen( NEW_LINE_TEXT ), NULL, NULL );

					// Update return value
					nResult ++;

				} // End of successfully wrote item text to file
				else
				{
					// Unable to write item text to file

					// Force exit from loop
					nWhichItem = nItemCount;

				} // End of unable to write item text to file

			} // End of successfully got item text
			else
			{
				// Unable to get item text

				// Force exit from loop
				nWhichItem = nItemCount;

			} // End of unable to get item text

		}; // End of loop through items on combo box window

		// Free string memory
		delete [] lpszItemText;

		// Close file
		CloseHandle( hFile );

	} // End of successfully created file

	return nResult;

} // End of function ComboBoxWindowSave

BOOL ComboBoxWindowSelectItem( int nWhichItem, BOOL( *lpSelectionChangeFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select item
	if( SendMessage( g_hWndComboBox, CB_SETCURSEL, ( WPARAM )nWhichItem, ( LPARAM )NULL ) != CB_ERR )
	{
		// Successfully selected item

		// Allocate string memory
		LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

		// Get item text
		if( SendMessage( g_hWndComboBox, CB_GETLBTEXT, ( WPARAM )nWhichItem, ( LPARAM )lpszItemText) != CB_ERR )
		{
			// Successfully got item text

			// Call selection change function
			bResult = ( *lpSelectionChangeFunction )( lpszItemText );

		} // End of successfully got item text

		// Free string memory
		delete [] lpszItemText;

	} // Successfully selected item

	return bResult;

} // End of function ComboBoxWindowSelectItem

HWND ComboBoxWindowSetFocus()
{
	// Focus on combo box window
	return SetFocus( g_hWndComboBox );

} // End of function ComboBoxWindowSetFocus

void ComboBoxWindowSetFont( HFONT hFont )
{
	// Set combo box window font
	SendMessage( g_hWndComboBox, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ComboBoxWindowSetFont
