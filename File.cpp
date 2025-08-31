// File.cpp

#include "File.h"

BOOL ComboBoxWindowSelectionChangeFunction( int nWhichColumn, LPCTSTR lpszItemText )
{
	BOOL bResult;

	int nItemCount;

	// Allocate string memory
	LPTSTR lpszStatusMessage = new char[ STRING_LENGTH + sizeof( char ) ];

	// Populate list view window
	nItemCount = ListViewWindowPopulate( nWhichColumn, lpszItemText );

	// Format status message
	wsprintf( lpszStatusMessage, LIST_VIEW_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING, lpszItemText, nItemCount );

	// Show status message on status bar window
	bResult = StatusBarWindowSetText( nWhichColumn, lpszStatusMessage );

	// Free string memory
	delete [] lpszStatusMessage;

	return bResult;

} // End of function ComboBoxWindowSelectionChangeFunction

int ComboBoxWindowPopulate( int nWhichComboBoxWindow )
{
	int nResult = 0;

	int nItemCount;

	// Populate combo box window
	nItemCount = ComboBoxWindowPopulate( nWhichComboBoxWindow, TEMPLATE_FILE_NAME );

	// Ensure that  combo box window contains items
	if( nItemCount == 0 )
	{
		// Combo box window is empty

		// Allocate string memory
		LPTSTR lpszFolderPath = new char[ STRING_LENGTH + sizeof( char ) ];

		// Get current folder path
		GetCurrentDirectory( STRING_LENGTH, lpszFolderPath );

		// Add current folder path to combo box window
		ComboBoxWindowAddString( nWhichComboBoxWindow, lpszFolderPath, NULL );

		// Free string memory
		delete [] lpszFolderPath;

	} // End of combo box window is empty

	// Select first item on combo box window
	ComboBoxWindowSelectItem( nWhichComboBoxWindow, 0, &ComboBoxWindowSelectionChangeFunction );

	return nResult;

} // End of function ComboBoxWindowPopulate

BOOL ListViewWindowDoubleClickFunction( int nWhichColumn, LPCTSTR lpszItemPath )
{
	BOOL bResult = FALSE;

	WIN32_FIND_DATA wfd;
	HANDLE hFileFind;

	// Attempt to find item
	hFileFind = FindFirstFile( lpszItemPath, &wfd );

	// Ensure that item exists
	if( hFileFind != INVALID_HANDLE_VALUE )
	{
		// Item exists

		// See if item is a folder
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			// Item is a folder

			// Add folder to combo box window
			if( ComboBoxWindowAddString( nWhichColumn, lpszItemPath, &ComboBoxWindowSelectionChangeFunction ) >= 1 )
			{
				// Successfully added folder to combo box window

				// Update return value
				bResult = TRUE;

			} // End of successfully added folder to combo box window

		} // End of item is a folder
		else
		{
			// Item is a file

			// Open selected item
			if( ( INT_PTR )ShellExecute( NULL, SHELL_EXECUTE_OPEN_COMMAND, lpszItemPath, NULL, NULL, SW_SHOWDEFAULT ) > SHELL_EXECUTE_MAXIMUM_FAILURE_RETURN_VALUE )
			{
				// Successfully opened item

				// Update return value
				bResult = TRUE;

			} // End of successfully opened item
			else
			{
				// Unable to open item

				// Allocate string memory
				LPTSTR lpszErrorMessage = new char[ STRING_LENGTH + sizeof( char ) ];

				// Format error message
				wsprintf( lpszErrorMessage, UNABLE_TO_OPEN_FILE_ERROR_MESSAGE_FORMAT_STRING, lpszItemPath );

				// Display error message
				MessageBox( NULL, lpszErrorMessage, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

				// Free string memory
				delete [] lpszErrorMessage;

			} // End of unable to open item

		} // End of item is a file

		// Close file find
		FindClose( hFileFind );

	} // End of item exists

	return bResult;

} // End of function ListViewWindowDoubleClickFunction

BOOL ListViewWindowSelectionChangeFunction( int nWhichColumn, LPCTSTR lpszItemPath )
{
	return StatusBarWindowSetText( nWhichColumn, lpszItemPath );

} // End of function ListViewWindowSelectionChangeFunction

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= GetModuleHandle( NULL );
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= MAIN_WINDOW_CLASS_ICON_NAME;

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMsg )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;
			HFONT hFont;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Get font
			hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

			// Create source combo box window
			if( ComboBoxWindowCreate( SOURCE_COLUMN_ID, hWndMain, hInstance ) )
			{
				// Successfully created source combo box window

				// Set source combo box window font
				ComboBoxWindowSetFont( SOURCE_COLUMN_ID, hFont );

				// Create source list view window
				if( ListViewWindowCreate( SOURCE_COLUMN_ID, hWndMain, hInstance ) )
				{
					// Successfully created source list view window

					// Set source list view window font
					ListViewWindowSetFont( SOURCE_COLUMN_ID, hFont );

					// Create destination combo box window
					if( ComboBoxWindowCreate( DESTINATION_COLUMN_ID, hWndMain, hInstance ) )
					{
						// Successfully created destination combo box window

						// Set destination combo box window font
						ComboBoxWindowSetFont( DESTINATION_COLUMN_ID, hFont );

						// Create destination list view window
						if( ListViewWindowCreate( DESTINATION_COLUMN_ID, hWndMain, hInstance ) )
						{
							// Successfully created destination list view window

							// Set destination list view window font
							ListViewWindowSetFont( DESTINATION_COLUMN_ID, hFont );

							// Create status bar window
							if( StatusBarWindowCreate( hWndMain, hInstance ) )
							{
								// Successfully created status bar window

								// Set status bar window font
								StatusBarWindowSetFont( hFont );

							} // End of successfully created status bar window

						} // End of successfully created destination list view window

					} // End of successfully created destination combo box window

				} // End of successfully created source list view window

			} // End of successfully created source combo box window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatus;
			RECT rcComboBox;
			int nStatusWindowHeight;
			int nComboBoxWindowHeight;
			int nListViewWindowHeight;
			int nListViewWindowTop;
			int nSourceWidth;
			int nDestinationWidth;
			int nDestinationLeft;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			StatusBarWindowSize();

			// Get status window size
			StatusBarWindowGetRect( &rcStatus );

			// Calculate window sizes
			nStatusWindowHeight		= ( rcStatus.bottom - rcStatus.top );
			nComboBoxWindowHeight	= ( nClientHeight - nStatusWindowHeight );
			nSourceWidth			= ( ( nClientWidth - BUTTON_WINDOW_WIDTH ) / NUMBER_OF_COLUMNS );
			nDestinationWidth		= ( nClientWidth - ( nSourceWidth + BUTTON_WINDOW_WIDTH ) );

			// Calculate window positions
			nDestinationLeft		= ( nSourceWidth + BUTTON_WINDOW_WIDTH );

			// Move combo box windows
			ComboBoxWindowMove( SOURCE_COLUMN_ID,		0,					0,	nSourceWidth,		nComboBoxWindowHeight,	TRUE );
			ComboBoxWindowMove( DESTINATION_COLUMN_ID,	nDestinationLeft,	0,	nDestinationWidth,	nComboBoxWindowHeight,	TRUE );

			// Get source combo box window size
			ComboBoxWindowGetRect( SOURCE_COLUMN_ID, &rcComboBox );

			// Calculate window sizes
			nComboBoxWindowHeight	= ( rcComboBox.bottom - rcComboBox.top );
			nListViewWindowHeight	= ( nClientHeight - ( nStatusWindowHeight + nComboBoxWindowHeight ) + WINDOW_BORDER_HEIGHT );

			// Calculate window positions
			nListViewWindowTop		= ( nComboBoxWindowHeight - WINDOW_BORDER_HEIGHT );

			// Move list view windows
			ListViewWindowMove( SOURCE_COLUMN_ID,		0,					nListViewWindowTop,	nSourceWidth,		nListViewWindowHeight,	TRUE );
			ListViewWindowMove( DESTINATION_COLUMN_ID,	nDestinationLeft,	nListViewWindowTop,	nDestinationLeft,	nListViewWindowHeight,	TRUE );

			// Set status bar window parts
			StatusBarWindowSetParts( nDestinationLeft );

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on source combo box window
			ComboBoxWindowSetFocus( SOURCE_COLUMN_ID );

			// Break out of switch
			break;

		} // End of an activate message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO FAR *lpMinMaxInfo;

			// Get min max info structure
			lpMinMaxInfo = ( MINMAXINFO FAR * )lParam;

			// Update min max info structure
			lpMinMaxInfo->ptMinTrackSize.x = MAIN_WINDOW_MINIMUM_WIDTH;
			lpMinMaxInfo->ptMinTrackSize.y = MAIN_WINDOW_MINIMUM_HEIGHT;

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_DROPFILES:
		{
			// A drop files message
			UINT uFileCount;
			HDROP hDrop;
			UINT uWhichFile;
			UINT uFileSize;

			// Allocate string memory
			LPTSTR lpszFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get drop handle
			hDrop = ( HDROP )wParam;

			// Count dropped files
			uFileCount = DragQueryFile( hDrop, ( UINT )-1, NULL, 0 );

			// Loop through dropped files
			for( uWhichFile = 0; uWhichFile < uFileCount; uWhichFile ++ )
			{
				// Get size of dropped file
				uFileSize = DragQueryFile( hDrop, uWhichFile, NULL, 0 );

				// Ensure that file size is valid
				if( uFileSize != 0 )
				{
					// File size is valid

					// Get file path
					if( DragQueryFile( hDrop, uWhichFile, lpszFilePath, ( uFileSize + sizeof( char ) ) ) )
					{
						// Successfully got file path

						// Add file path to source combo box window
						ComboBoxWindowAddString( SOURCE_COLUMN_ID, lpszFilePath, NULL );

					} // End of successfully got file path

				} // End of file size is valid

			}; // End of loop through dropped files

			// Free string memory
			delete [] lpszFilePath;

			// Break out of switch
			break;

		} // End of a drop files message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy main window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// See if command message is from source combo box window
					if( IsComboBoxWindow( SOURCE_COLUMN_ID, ( HWND )lParam ) )
					{
						// Command message is from source combo box window

						// Handle command message from source combo box window
						if( !( ComboBoxWindowHandleCommandMessage( SOURCE_COLUMN_ID, wParam, lParam, &ComboBoxWindowSelectionChangeFunction ) ) )
						{
							// Command message was not handled from source combo box window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

						} // End of command message was not handled from source combo box window

					} // End of command message is from source combo box window
					else if( IsComboBoxWindow( DESTINATION_COLUMN_ID, ( HWND )lParam ) )
					{
						// Command message is from source combo box window

						// Handle command message from source destination box window
						if( !( ComboBoxWindowHandleCommandMessage( DESTINATION_COLUMN_ID, wParam, lParam, &ComboBoxWindowSelectionChangeFunction ) ) )
						{
							// Command message was not handled from destination combo box window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

						} // End of command message was not handled from destination combo box window

					} // End of command message is from destination combo box window
					else
					{
						// Command message is not from a combo box window

						// Call default procedure
						lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

					} // End of command message is not from a combo box window

					// Break out of switch
					break;

				} // End of default command

			}; // End of selection for command

			// Break out of switch
			break;

		} // End of a command message
		case WM_SYSCOMMAND:
		{
			// A system command message

			// Select system command
			switch( LOWORD( wParam ) )
			{
				case IDM_HELP_ABOUT:
				{
					// A help about system command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about system command
				default:
				{
					// Default system command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_NOTIFY:
		{
			// A notify message
			LPNMHDR lpNmHdr;

			// Get notify message handler
			lpNmHdr = ( LPNMHDR )lParam;

			// See if notify message is from source list view window
			if( IsListViewWindow( SOURCE_COLUMN_ID, lpNmHdr->hwndFrom ) )
			{
				// Notify message is from source list view window

				// Handle notify message from source list view window
				if( !( ListViewWindowHandleNotifyMessage( SOURCE_COLUMN_ID, wParam, lParam, &ListViewWindowSelectionChangeFunction, &ListViewWindowDoubleClickFunction ) ) )
				{
					// Notify message was not handled from source list view window

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

				} // End of notify message was not handled from source list view window

			} // End of notify message is from source list view window
			else
			if( IsListViewWindow( DESTINATION_COLUMN_ID, lpNmHdr->hwndFrom ) )
			{
				// Notify message is from destination list view window

				// Handle notify message from destination list view window
				if( !( ListViewWindowHandleNotifyMessage( DESTINATION_COLUMN_ID, wParam, lParam, &ListViewWindowSelectionChangeFunction, &ListViewWindowDoubleClickFunction ) ) )
				{
					// Notify message was not handled from destination list view window

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

				} // End of notify message was not handled from destination list view window

			} // End of notify message is from destination list view window
			else
			{
				// Notify message is not from a list view window

				// Call default procedure
				lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

			} // End of notify message is not from a list view window

			// Break out of switch
			break;

		} // End of a notify message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			HMENU hMenu;

			// Load context menu
			hMenu = LoadMenu( NULL, MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show context menu
			TrackPopupMenu( GetSubMenu( hMenu, 0 ), ( TPM_LEFTALIGN | TPM_LEFTBUTTON ), LOWORD( lParam ), HIWORD( lParam ), 0, hWndMain, NULL );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message

			// Save file
			if( ComboBoxWindowSave( SOURCE_COLUMN_ID, TEMPLATE_FILE_NAME ) )
			{
				// Successfully saved file

				// Free control window memory
				ListViewWindowFreeMemory( SOURCE_COLUMN_ID );
				ListViewWindowFreeMemory( DESTINATION_COLUMN_ID );

				// Destroy main window
				DestroyWindow( hWndMain );

			} // End of successfully saved file
			else
			{
				// Unable to save file

				// Ensure that user is ok to close
				if( MessageBox( hWndMain, COMBO_BOX_WINDOW_UNABLE_TO_SAVE_WARNING_MESSAGE, WARNING_MESSAGE_CAPTION, ( MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING ) ) == IDYES )
				{
					// User is ok to close

					// Free control window memory
					ListViewWindowFreeMemory( SOURCE_COLUMN_ID );
					ListViewWindowFreeMemory( DESTINATION_COLUMN_ID );

					// Destroy main window
					DestroyWindow( hWndMain );

				} // End of user is ok to close

			} // End of unable to save file

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default window procedure
			lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWindowProcedure

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	MSG msg;

	WNDCLASSEX wcMain;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Clear window class structure
	ZeroMemory( &wcMain, sizeof( wcMain ) );

	// Initialise window class structure
	wcMain.cbSize			= sizeof( WNDCLASSEX );
	wcMain.lpfnWndProc		= MainWindowProcedure;
	wcMain.hInstance		= hInstance;
	wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
	wcMain.style			= MAIN_WINDOW_CLASS_STYLE;
	wcMain.hIcon			= MAIN_WINDOW_CLASS_ICON;
	wcMain.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
	wcMain.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
	wcMain.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
	wcMain.hIconSm			= MAIN_WINDOW_CLASS_ICON_SMALL;

	// Register main window class
	if( RegisterClassEx( &wcMain ) )
	{
		// Successfully registered main window class
		HWND hWndMain;

		// Create main window
		hWndMain = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TEXT, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

		// Ensure that main window was created
		if( hWndMain )
		{
			// Successfully created main window
			HMENU hMenuSystem;
			LPWSTR *lpszArgumentList;
			int nArgumentCount;

			// Get system menu
			hMenuSystem = GetSystemMenu( hWndMain, FALSE );

			// Add separator item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

			// Add about item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

			// Get argument list
			lpszArgumentList = CommandLineToArgvW( GetCommandLineW(), &nArgumentCount );

			// Ensure that argument list was got
			if( lpszArgumentList )
			{
				// Successfully got argument list
				int nWhichArgument;
				int nSizeNeeded;
				int nWideArgumentLength;

				// Allocate string memory
				LPTSTR lpszArgument = new char[ STRING_LENGTH + sizeof( char ) ];

				// Loop through arguments
				for( nWhichArgument = 1; nWhichArgument < nArgumentCount; nWhichArgument ++ )
				{
					// Get wide argument length
					nWideArgumentLength = lstrlenW( lpszArgumentList[ nWhichArgument ] );

					// Get size required for argument
					nSizeNeeded = WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ nWhichArgument ], nWideArgumentLength, NULL, 0, NULL, NULL );

					// Convert argument to ansi
					WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ nWhichArgument ], nWideArgumentLength, lpszArgument, nSizeNeeded, NULL, NULL );

					// Terminate argument
					lpszArgument[ nSizeNeeded ] = ( char )NULL;

					// Add argument to source combo box window
					ComboBoxWindowAddString( SOURCE_COLUMN_ID, lpszArgument, NULL );

				}; // End of loop through arguments

				// Free string memory
				delete [] lpszArgument;

			} // End of successfully got argument list

			// Show main window
			ShowWindow( hWndMain, nCmdShow );

			// Update main window
			UpdateWindow( hWndMain );

			// Populate combo box windows
			ComboBoxWindowPopulate( SOURCE_COLUMN_ID );
			ComboBoxWindowPopulate( DESTINATION_COLUMN_ID );

			// Message loop
			while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
			{
				// Translate message
				TranslateMessage( &msg );

				// Dispatch message
				DispatchMessage( &msg );

			}; // End of message loop

		} // End of successfully main created window
		else
		{
			// Unable to create main window

			// Display error message
			MessageBox( NULL, UNABLE_TO_CREATE_MAIN_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to create main window

	} // End of successfully registered main window class
	else
	{
		// Unable to register main window class

		// Display error message
		MessageBox( NULL, UNABLE_TO_REGISTER_MAIN_WINDOW_CLASS_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to register main window class

	return msg.wParam;

} // End of function WinMain
