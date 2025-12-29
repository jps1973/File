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
	int nResult = -1;

	// Attempt to find string on combo box window
	nResult = SendMessage( g_hWndComboBox, CB_FINDSTRINGEXACT, ( WPARAM )-1, ( LPARAM )lpszString );

	// See if string was found on combo box window
	if( nResult == CB_ERR )
	{
		// String was not found on combo box window

		// Add string to combo box window
		nResult = SendMessage( g_hWndComboBox, CB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszString );

	} // End of string was not found on combo box window

	return nResult;

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

BOOL ComboBoxWindowGetRect( LPRECT lpRect )
{
	// Get combo box window rect
	return GetWindowRect( g_hWndComboBox, lpRect );

} // End of function ComboBoxWindowGetRect

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

BOOL ComboBoxWindowLoad( LPCTSTR lpszFilePath )
{
	BOOL bResult = FALSE;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Succesfully opened file
		DWORD dwFileSize;

		// Get file size
		dwFileSize = GetFileSize( hFile, NULL );

		// Ensure that file size was got
		if( dwFileSize != INVALID_FILE_SIZE )
		{
			// Successfully got file size

			// Allocate string memory
			LPSTR lpszFileText = new char[ dwFileSize + sizeof( char ) ];

			// Read file text
			if( ReadFile( hFile, lpszFileText, dwFileSize, NULL, NULL ) )
			{
				// Successfully read file text
				LPTSTR lpszLine;

				// Terminate file text
				lpszFileText[ dwFileSize ] = ( char )NULL;

				// Get first line
				lpszLine = strtok( lpszFileText, NEW_LINE_TEXT );

				// Loop through lines in file
				while( lpszLine )
				{
					// Add line to combo box window
					if( SendMessage( g_hWndComboBox, CB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszLine ) >= 0 )
					{
						// Successfully added line to combo box window

						// Update return value
						bResult = TRUE;

						// Get first line
						lpszLine = strtok( NULL, NEW_LINE_TEXT );

					} // End of successfully added line to combo box window
					else
					{
						// Unable to add line to combo box window

						// Force exit from loop
						lpszLine = NULL;

						// Update return value
						bResult = FALSE;

					} // End of unable to add line to combo box window

				}; // End of loop through lines in file

			} // End of successfully read file text

			// Free string memory
			delete [] lpszFileText;

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of succesfully opened file

	return bResult;

} // End of function ComboBoxWindowLoad

BOOL ComboBoxWindowMove( int nLeft, int nTop, int nWidth, int nHeight )
{
	// Move combo box window
	return MoveWindow( g_hWndComboBox, nLeft, nTop, nWidth, nHeight, TRUE );

} // End of function ComboBoxWindowMove

BOOL ComboBoxWindowSave( LPCTSTR lpszFilePath )
{
	BOOL bResult = FALSE;

	HANDLE hFile;

	// Create file
	hFile = CreateFile( lpszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	// Ensure that file was created
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully created file
		int nItemCount;
		int nWhichItem;

		// Allocate string memory
		LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

		// Count items on list box window
		nItemCount = SendMessage( g_hWndComboBox, CB_GETCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Loop through items on list box window
		for( nWhichItem = 0; nWhichItem < nItemCount; nWhichItem ++ )
		{
			// Get item text
			if( SendMessage( g_hWndComboBox, CB_GETLBTEXT, ( WPARAM )nWhichItem, ( LPARAM )lpszItemText ) )
			{
				// Successfully got item text

				// Write item text to file
				if( WriteFile( hFile, lpszItemText, lstrlen( lpszItemText ), NULL, NULL ) )
				{
					// Successfully wrote item text to file

					// Write new line text to file
					WriteFile( hFile, NEW_LINE_TEXT, lstrlen( NEW_LINE_TEXT ), NULL, NULL );

					// Update return value
					bResult = TRUE;

				} // End of successfully wrote item text to file
				else
				{
					// Unable to write item text to file

					// Update return value
					bResult = FALSE;

					// Force exit from loop
					nWhichItem = nItemCount;

				} // End of unable to write item text to file

			} // End of successfully got item text
			else
			{
				// Unable to get item text

				// Update return value
				bResult = FALSE;

				// Force exit from loop
				nWhichItem = nItemCount;

			} // End of unable to get item text

		}; // End of loop through items on list box window

		// Free string memory
		delete [] lpszItemText;

		// Close file
		CloseHandle(hFile);

	} // End of successfully created file

	return bResult;

} // End of function ComboBoxWindowSave

int ComboBoxWindowSelectItem( int nWhichItem, void( *lpSelectionChangeFunction )( LPCTSTR lpszItemText ) )
{
	int nResult;

	// Select combo box item
	nResult = SendMessage( g_hWndComboBox, CB_SETCURSEL, ( WPARAM )nWhichItem, ( LPARAM )NULL );

	// Ensure that combo box item was selected
	if( nResult != CB_ERR )
	{
		// Successfully selected combo box item

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

	} // End of successfully selected combo box item

	return nResult;

} // End of function ComboBoxWindowSelectItem
