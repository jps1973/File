// TabControlWindow.cpp

#include "TabControlWindow.h"

// Global variables
static HWND g_hWndTabControl;
static HWND g_hWndActiveControl;
static int g_nNextTabNumber;

BOOL IsTabControlWindow( HWND hWnd )
{
	// See if supplied window is tab control window
	return( hWnd == g_hWndTabControl );

} // End of function IsTabControlWindow

int TabControlWindowAddTab( HINSTANCE hInstance )
{
	int nResult = -1;

	// Allocate string memory
	LPTSTR lpszTabName = new char[ STRING_LENGTH + sizeof( char ) ];

	// Format tab name
	wsprintf( lpszTabName, TAB_CONTROL_WINDOW_NEW_TAB_NAME_FORMAT_STRING, g_nNextTabNumber );

	// Ensure that tab name does not already exist
	while( TabControlWindowDoesTabExist( lpszTabName ) )
	{
		// Update next tab number
		g_nNextTabNumber ++;

		// Format tab name
		wsprintf( lpszTabName, TAB_CONTROL_WINDOW_NEW_TAB_NAME_FORMAT_STRING, g_nNextTabNumber );

	}; // End of loop to ensure that tab name does not already exist

	// Add tab
	nResult = TabControlWindowAddTab( hInstance, lpszTabName );

	// Ensure that tab was added
	if( nResult >= 0 )
	{
		// Successfully added tab

		// Update next tab number
		g_nNextTabNumber ++;

	} // End of successfully added tab

	// Free string memory
	delete [] lpszTabName;

	return nResult;

} // End of function TabControlWindowAddTab

int TabControlWindowAddTab( HINSTANCE hInstance, LPCTSTR lpszTabName )
{
	int nResult = -1;

	HWND hWndControl;

	// Create control window
	hWndControl = ControlWindowCreate( g_hWndTabControl, hInstance );

	// Ensure that control window was created
	if( hWndControl )
	{
		// Successfully created control window
		TAB_CONTROL_WINDOW_DATA tcwData;
		int nTabCount;
		HFONT hFont;

		// Clear tab control window data structure
		ZeroMemory( &tcwData, sizeof( tcwData ) );

		// Initialise tab control window data structure
		tcwData.tcItemHeader.mask		= ( TCIF_TEXT | TCIF_PARAM );
		tcwData.hWndControl				= hWndControl;
		tcwData.tcItemHeader.cchTextMax	= STRING_LENGTH;
		tcwData.tcItemHeader.pszText	= ( LPTSTR )lpszTabName;
		wsprintf( tcwData.cData, "Data for tab %s.", lpszTabName );

		// Get font
		hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

		// Set control window font
		SendMessage( hWndControl, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

		// Add text to control window
		SendMessage( hWndControl, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszTabName );

		// Count tabs
		nTabCount = SendMessage( g_hWndTabControl, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Add tab
		nResult = SendMessage( g_hWndTabControl, TCM_INSERTITEM, ( WPARAM )nTabCount, ( LPARAM )( LPTCITEMHEADER )( &tcwData ) );

	} // End of successfully created control window

	return nResult;

} // End of function TabControlWindowAddTab

int TabControlWindowCountTabs()
{
	// Count tabs
	return SendMessage( g_hWndTabControl, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function TabControlWindowCountTabs

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create tab control window
	g_hWndTabControl = CreateWindowEx( TAB_CONTROL_WINDOW_EXTENDED_STYLE, TAB_CONTROL_WINDOW_CLASS_NAME, TAB_CONTROL_WINDOW_TEXT, TAB_CONTROL_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that tab control window was created
	if( g_hWndTabControl )
	{
		// Successfully created tab control window
		int nExtraBytes;

		// Calculate extra bytes
		nExtraBytes = ( sizeof( TAB_CONTROL_WINDOW_DATA ) - sizeof( TCITEMHEADER ) );

		// Allocate extra memory for tab data
		if( SendMessage( g_hWndTabControl, TCM_SETITEMEXTRA, ( WPARAM )nExtraBytes, ( LPARAM )NULL ) )
		{
			// Successfully allocated extra memory for tab data

			// Clear active control window
			g_hWndActiveControl = NULL;

			// Initialise next tab number
			g_nNextTabNumber = TAB_CONTROL_WINDOW_FIRST_NEW_TAB_NUMBER;

			// Update return value
			bResult = TRUE;

		} // End of successfully allocated extra memory for tab data

	} // End of successfully created tab control window

	return bResult;

} // End of function TabControlWindowCreate

BOOL TabControlWindowDeleteTab( int nWhichTab )
{
	// Delete tab
	return SendMessage( g_hWndTabControl, TCM_DELETEITEM, ( WPARAM )nWhichTab, ( LPARAM )NULL );

} // End of function TabControlWindowDeleteTab

BOOL TabControlWindowDoesTabExist( LPCTSTR lpszRequiredTabName )
{
	BOOL bResult = FALSE;

	TAB_CONTROL_WINDOW_DATA tcwData;
	int nTabCount;
	int nWhichTab;

	// Allocate string memory
	LPTSTR lpszCurrentTabName = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear tab control window data structure
	ZeroMemory( &tcwData, sizeof( tcwData ) );

	// Initialise tab control window data structure
	tcwData.tcItemHeader.mask		= TCIF_TEXT;
	tcwData.tcItemHeader.cchTextMax	= STRING_LENGTH;
	tcwData.tcItemHeader.pszText	= ( LPTSTR )lpszCurrentTabName;

	// Count tabs
	nTabCount = SendMessage( g_hWndTabControl, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Loop through tabs
	for( nWhichTab = 0; nWhichTab < nTabCount; nWhichTab ++ )
	{
		// Get tab control item
		if( SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nWhichTab, ( LPARAM )( LPTCITEMHEADER )( &tcwData ) ) )
		{
			// Successfully got tab control item

			// See if tab has the required name
			if( lstrcmpi( lpszCurrentTabName, lpszRequiredTabName ) == 0 )
			{
				// Tab has the required name

				// Update return value
				bResult = TRUE;

				// Force exit from loop
				nWhichTab = nTabCount;

			} // End of tab has the required name

		} // End of successfully got tab control item
		else
		{
			// Unable to get tab control item

			// Force exit from loop
			nWhichTab = nTabCount;

		} // End of unable to get tab control item

	}; // End of loop through all tabs

	// Free string memory
	delete [] lpszCurrentTabName;

	return bResult;

} // End of function TabControlWindowDoesTabExist

BOOL TabControlWindowGetRect( LPRECT lpRect )
{
	// Get tab control window rect
	return GetWindowRect( g_hWndTabControl, lpRect );

} // End of function TabControlWindowGetRect

int TabControlWindowGetSelectedItem()
{
	// Get selected item
	return SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function TabControlWindowGetSelectedItem

BOOL TabControlWindowGetTabName( int nWhichItem, LPTSTR lpszTabName )
{
	BOOL bResult = FALSE;

	TCITEM tcItem;

	// Clear tab control item structure
	ZeroMemory( &tcItem, sizeof( tcItem ) );

	// Initialise tab control item structure
	tcItem.mask			= TCIF_TEXT;
	tcItem.cchTextMax	= STRING_LENGTH;
	tcItem.pszText		= lpszTabName;

	// Get tab control item
	if( SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nWhichItem, ( LPARAM )&tcItem ) )
	{
		// Successfully got tab control item

		// Update return value
		bResult = TRUE;

	} // End of successfully got tab control item

	return bResult;

} // End of function TabControlWindowGetTabName

BOOL TabControlWindowHandleNotifyMessage( WPARAM, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszTabName ) )
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmhdr;

	// Get notify message handler
	lpNmhdr = ( ( LPNMHDR )lParam );

	// Select tab control window notification code
	switch( lpNmhdr->code )
	{
		case TCN_SELCHANGING:
		{
			// A selection changing notification code

			// Break out of switch
			break;

		}// End of a selection changing notification code
		case TCN_SELCHANGE:
		{
			// A selection change notification code
			int nSelectedTab;

			// Get selected tab
			nSelectedTab = SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Ensure that selected tab was got
			if( nSelectedTab >= 0 )
			{
				// Successfully got selected tab

				// Action selected tab
				bResult = TabControlWindowOnTabSelected( nSelectedTab, lpStatusFunction );

			} // End of successfully got selected tab

			// Break out of switch
			break;

		}// End of a selection change notification code
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for tab control window notification code

	return bResult;

} // End of function TabControlWindowHandleNotifyMessage

int TabControlWindowLoad( LPCTSTR lpszFileName, HINSTANCE hInstance )
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
			LPSTR lpszFileText = new char[ dwFileSize + sizeof( char ) ];

			// Read file
			if( ReadFile( hFile, lpszFileText, dwFileSize, NULL, NULL ) )
			{
				// Successfully read file
				LPTSTR lpszTab;

				// Terminate file text
				lpszFileText[ dwFileSize ] = ( char )NULL;

				// Get first tab
				lpszTab = strtok( lpszFileText, NEW_LINE_TEXT );

				// Loop through all tabs
				while( lpszTab )
				{
					// Add tab
					if( TabControlWindowAddTab( hInstance, lpszTab ) >= 0 )
					{
						// Successfully added tab

						// Update return value
						nResult ++;

						// Get next tab
						lpszTab = strtok( NULL, NEW_LINE_TEXT );

					} // End of successfully added tab
					else
					{
						// Unable to add tab

						// Force exit from loop
						lpszTab = NULL;

					} // End of unable to add tab

				}; // End of loop through all tabs

			} // End of successfully read file

		} // End of successfully got file size

		// Close file
		CloseHandle(hFile);

	} // End of successfully opened file
	return nResult;

} // End of function TabControlWindowLoad

BOOL TabControlWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	BOOL bResult = FALSE;

	// Move tab control window
	if( MoveWindow( g_hWndTabControl, nX, nY, nWidth, nHeight, bRepaint ) )
	{
		// Successfully moved tab control window

		// Move control window
		if( TabControlWindowMoveControlWindow() )
		{
			// Successfully moved control window

			// Update return value
			bResult = TRUE;

		} // End of successfully moved control window

	} // End of successfully moved tab control window

	return bResult;

} // End of function TabControlWindowMove

BOOL TabControlWindowMoveControlWindow()
{
	BOOL bResult = FALSE;

	RECT rc;
	int nControlWindowWidth;
	int nControlWindowHeight;

	// Get tab control window size
	GetClientRect( g_hWndTabControl, &rc );

	// Adjust rect for control window
	SendMessage( g_hWndTabControl, TCM_ADJUSTRECT, ( WPARAM )FALSE, ( LPARAM )&rc );

	// Calculate control window size
	nControlWindowWidth		= ( rc.right - rc.left );
	nControlWindowHeight	= ( rc.bottom - rc.top );

	// Move control window
	bResult = MoveWindow( g_hWndActiveControl, rc.left, rc.top, nControlWindowWidth, nControlWindowHeight, TRUE );

	return bResult;

} // End of function TabControlWindowMoveControlWindow

BOOL TabControlWindowOnTabSelected( int nWhichItem, BOOL( *lpStatusFunction )( LPCTSTR lpszTabName ) )
{
	BOOL bResult = FALSE;

	TAB_CONTROL_WINDOW_DATA tcwData;

	// Allocate string memory
	LPTSTR lpszTabName = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear tab control window data structure
	ZeroMemory( &tcwData, sizeof( tcwData ) );

	// Initialise tab control window data structure
	tcwData.tcItemHeader.mask		= ( TCIF_TEXT | TCIF_PARAM );
	tcwData.tcItemHeader.cchTextMax	= STRING_LENGTH;
	tcwData.tcItemHeader.pszText	= ( LPTSTR )lpszTabName;

	// Get tab control item
	if( SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nWhichItem, ( LPARAM )( LPTCITEMHEADER )( &tcwData ) ) )
	{
		// Successfully got tab control item

		// See if active control window is valid
		if( g_hWndActiveControl )
		{
			// Active control window is valid

			// Hide active control window
			ShowWindow( g_hWndActiveControl, SW_HIDE );

		} // End of active control window is valid

		// Update active control window
		g_hWndActiveControl = tcwData.hWndControl;

		// Move active control window
		TabControlWindowMoveControlWindow();

		// Show active control window
		ShowWindow( g_hWndActiveControl, SW_SHOW );

		// Show item text on status bar window
		( *lpStatusFunction )( tcwData.cData );

		// Update return value
		bResult = TRUE;

	} // End of successfully got tab control item

	// Free string memory
	delete [] lpszTabName;

	return bResult;

} // End of function TabControlWindowOnTabSelected

int TabControlWindowSave( LPCTSTR lpszFileName )
{
	int nResult = 0;

	HANDLE hFile;

	// Create file
	hFile = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	// Ensure that file was created
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully created file
		int nTabCount;
		int nWhichTab;
		TAB_CONTROL_WINDOW_DATA tcwData;

		// Allocate string memory
		LPTSTR lpszTabName = new char[ STRING_LENGTH + sizeof( char ) ];

		// Clear tab control window data structure
		ZeroMemory( &tcwData, sizeof( tcwData ) );

		// Initialise tab control window data structure
		tcwData.tcItemHeader.mask		= TCIF_TEXT;
		tcwData.tcItemHeader.cchTextMax	= STRING_LENGTH;
		tcwData.tcItemHeader.pszText	= ( LPTSTR )lpszTabName;

		// Count tabs
		nTabCount = SendMessage( g_hWndTabControl, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Loop through tabs
		for( nWhichTab = 0; nWhichTab < nTabCount; nWhichTab ++ )
		{
			// Get tab control item
			if( SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nWhichTab, ( LPARAM )( LPTCITEMHEADER )( &tcwData ) ) )
			{
				// Successfully got tab control item

				// Write tab name to file
				if( WriteFile( hFile, lpszTabName, lstrlen( lpszTabName ), NULL, NULL ) )
				{
					// Successfully wrote tab name to file

					// Write new line text to file
					WriteFile( hFile, NEW_LINE_TEXT, lstrlen( NEW_LINE_TEXT ), NULL, NULL );

					// Update return value
					nResult ++;

				} // End of successfully wrote tab name to file
				else
				{
					// Unable to write tab name to file

					// Force exit from loop
					nWhichTab = nTabCount;

				} // End of unable to write tab name to file

			} // End of successfully got tab control item
			else
			{
				// Unable to get tab control item

				// Force exit from loop
				nWhichTab = nTabCount;

			} // End of unable to get tab control item

		}; // End of loop through all tabs

		// Free string memory
		delete [] lpszTabName;

		// Close file
		CloseHandle( hFile );

	} // End of successfully created file

	return nResult;

} // End of function TabControlWindowSave

BOOL TabControlWindowSelectTab( int nWhichTab, BOOL( *lpStatusFunction )( LPCTSTR lpszTabName ) )
{
	BOOL bResult = FALSE;

	int nSelectedTab;

	// Select tab
	SendMessage( g_hWndTabControl, TCM_SETCURSEL, ( WPARAM )nWhichTab, ( LPARAM )NULL );
	// Note that the above message returns the index of the previously selected tab

	// Get selected tab
	nSelectedTab = SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

	// Ensure that the required tab is selected
	if( nWhichTab == nSelectedTab )
	{
		// The required tab is selected

		// Action selected tab
		if( TabControlWindowOnTabSelected( nWhichTab, lpStatusFunction ) )
		{
			// Successfully actioned selected tab

			// Update return value
			bResult = TRUE;

		} // End of successfully actioned selected tab

	} // End of the required tab is selected
	return bResult;

} // End of function TabControlWindowSelectTab

HWND TabControlWindowSetFocus()
{
	// Focus on tab control window
	return SetFocus( g_hWndTabControl );

} // End of function TabControlWindowSetFocus

void TabControlWindowSetFont( HFONT hFont )
{
	// Set tab control window font
	SendMessage( g_hWndTabControl, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function TabControlWindowSetFont
