// File.cpp

#include "File.h"

#include <windows.h>

// Global variables
static LPTSTR g_lpszFoldersFilePath;

void ComboBoxWindowSelectionChangeFunction( LPCTSTR lpszItemText )
{
	// Set current folder
	if( SetCurrentDirectory( lpszItemText ) )
	{
		// Successfully set current folder
		int nItemCount;

		// Allocate string memory
		LPTSTR lpszStatusMessage = new char[ STRING_LENGTH + sizeof( char ) ];

		// Populate list view window
		nItemCount = ListViewWindowPopulate();

		// Format status message
		wsprintf( lpszStatusMessage, LIST_VIEW_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING, lpszItemText, nItemCount );

		// Show status message in status bar window
		StatusBarWindowSetText( lpszStatusMessage );

		// Free string memory
		delete [] lpszStatusMessage;

	} // End of successfully set current folder

} // End of function ComboBoxWindowSelectionChangeFunction

BOOL Populate( LPCTSTR lpszFolderPath )
{
	BOOL bResult = FALSE;

	int nFolderPosition;

	// Add folder to combo box window
	nFolderPosition = ComboBoxWindowAddString( lpszFolderPath );

	// Ensure that folder was added to combo box window
	if( nFolderPosition >= 0 )
	{
		// Successfully added folder to combo box window

		// Select folder on combo box window
		if( ComboBoxWindowSelectItem( nFolderPosition, &ComboBoxWindowSelectionChangeFunction ) != CB_ERR )
		{
			// Successfully selected folder on combo box window

			// Update return value
			bResult = TRUE;

		} // End of successfully selected folder on combo box window

	} // End of successfully added folder to combo box window

	return bResult;

} // End of function Populate

BOOL ListViewWindowDoubleClickFunction( LPCTSTR lpszItemPath )
{
	BOOL bResult = FALSE;

	// See if item is a folder
	if( GetFileAttributes( lpszItemPath ) & FILE_ATTRIBUTE_DIRECTORY )
	{
		// Item is a folder

		// Populate to folder
		bResult = Populate( lpszItemPath );

	} // End of item is a folder
	else
	{
		// Item is a file

		// Open item
		if( ( INT_PTR )ShellExecute( NULL, SHELL_EXECUTE_OPEN_COMMAND, lpszItemPath, NULL, NULL, SW_SHOWDEFAULT ) > SHELL_EXECUTE_MAXIMUM_FAILURE_RETURN_VALUE )
		{
			// Successfully opened item

			// Update return value
			bResult = TRUE;

		} // End of successfully opened item

	} // End of item is a file

	return bResult;

} // End of function ListViewWindowDoubleClickFunction

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= NULL; // Note that this must be null to use standard icons
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= MAIN_WINDOW_CLASS_ICON_NAME;

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMessage )
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

			// Create combo box window
			if( ComboBoxWindowCreate( hWndMain, hInstance, hFont ) )
			{
				// Successfully created combo box window

				// Create list view window
				if( ListViewWindowCreate( hWndMain, hInstance, hFont ) )
				{
					// Successfully created list view window

					// Create status bar window
					if( StatusBarWindowCreate( hWndMain, hInstance, hFont ) )
					{
						// Successfully created status bar window
					} // End of successfully created status bar window

				} // End of successfully created status bar window

			} // End of successfully created list view window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcComboBox;
			RECT rcStatusBar;
			int nComboBoxWindowHeight;
			int nStatusBarWindowHeight;
			int nListViewWindowHeight;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			StatusBarWindowSize();

			// Move combo box window
			ComboBoxWindowMove( 0, 0, nClientWidth, nClientHeight );

			// Get window rects
			ComboBoxWindowGetRect( &rcComboBox );
			StatusBarWindowGetRect( &rcStatusBar );

			// Calculate window sizes
			nComboBoxWindowHeight	= ( rcComboBox.bottom - rcComboBox.top );
			nStatusBarWindowHeight	= ( rcStatusBar.bottom - rcStatusBar.top );
			nListViewWindowHeight	= ( nClientHeight - ( nComboBoxWindowHeight + nStatusBarWindowHeight ) );

			// Move list view window
			ListViewWindowMove( 0, nComboBoxWindowHeight, nClientWidth, nListViewWindowHeight );

			// Break out of switch
			break;

		} // End of a size message
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
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case IDM_FILE_SELECT_FOLDER:
				{
					// A file select folder command

					// Allocate string memory
					LPTSTR lpszFolderPath = new char[ STRING_LENGTH + sizeof( char ) ];

					// Browse for folder
					if( BrowseForFolder( BROWSE_FOR_FOLDER_TITLE, lpszFolderPath, hWndMain ) )
					{
						// Successfully browsed for folder

						// Populate to folder
						Populate( lpszFolderPath );

					} // End of successfully browsed for folder

					// Free string memory
					delete [] lpszFolderPath;

					// Break out of switch
					break;

				} // End of a file select folder command
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

					// See if command message is from combo box window
					if( IsComboBoxWindow( ( HWND )lParam ) )
					{
						// Command message is from combo box window

						// Handle command message from combo box window
						if( !( ComboBoxWindowHandleCommandMessage( wParam, lParam, ComboBoxWindowSelectionChangeFunction ) ) )
						{
							// Command message was not handled from combo box window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

						} // End of command message was not handled from combo box window

					} // End of command message is from combo box window
					else
					{
						// Command message is not from a control window

						// Call default procedure
						lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					} // End of command message is not from a control window

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
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

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

			// See if notify message is from list view window
			if( IsListViewWindow( lpNmHdr->hwndFrom ) )
			{
				// Notify message is from list view window

				// Handle notify message from list view window
				if( !( ListViewWindowHandleNotifyMessage( wParam, lParam, &ListViewWindowDoubleClickFunction, &StatusBarWindowSetText ) ) )
				{
					// Notify message was not handled from list view window

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

				} // End of notify message was not handled from list view window

			} // End of notify message is from list view window
			else
			{
				// Notify message is not from list view window

				// Call default procedure
				lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			} // End of notify message is not from list view window

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

			// Save combo box window
			if( ComboBoxWindowSave( g_lpszFoldersFilePath ) )
			{
				// Successfully saved combo box window

				// Destroy main window
				DestroyWindow( hWndMain );

			} // End of successfully saved combo box window
			else
			{
				// Unable to save combo box window

				// Ensure that user is ok to continue
				if( MessageBox( hWndMain, UNABLE_TO_SAVE_FOLDERS_WARNING_MESSAGE, WARNING_MESSAGE_CAPTION, ( MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING ) ) == IDYES )
				{
					// User is ok to continue

					// Destroy main window
					DestroyWindow( hWndMain );

				} // End of user is ok to continue

			} // End of unable to save combo box window

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

			// Call default handler
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWindowProcedure

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int nCmdShow )
{
	MSG msg;

	WNDCLASSEX wcMain;

	// Allocate global memory
	g_lpszFoldersFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

	// Get current folder into folders file path
	GetCurrentDirectory( STRING_LENGTH, g_lpszFoldersFilePath );

	// Ensure that folders file path ends with a back-slash
	if( g_lpszFoldersFilePath[ lstrlen( g_lpszFoldersFilePath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
	{
		// Folders file path does not end with a back-slash

		// Append a back-slash onto folders file path
		lstrcat( g_lpszFoldersFilePath, ASCII_BACK_SLASH_STRING );

	} // End of folders file path does not end with a back-slash

	// Append folders file name onto folders file path
	lstrcat( g_lpszFoldersFilePath, FOLDERS_FILE_NAME );

	// Note that we need to store the folders file path like this as the current folder will change

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Clear main window class structure
	ZeroMemory( &wcMain, sizeof( wcMain ) );

	// Initialise main window class structure
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
		hWndMain = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TEXT, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,  NULL, NULL, hInstance, NULL );

		// Ensure that main window was created
		if( hWndMain )
		{
			// Successfully created main window
			HMENU hMenuSystem;

			// Get system menu
			hMenuSystem = GetSystemMenu( hWndMain, FALSE );

			// Add separator item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

			// Add about item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

			// Show main window
			ShowWindow( hWndMain, nCmdShow );

			// Update main window
			UpdateWindow( hWndMain );

			// Load folders into combo box window
			if( !( ComboBoxWindowLoad( g_lpszFoldersFilePath ) ) )
			{
				// Unable to load folders into combo box window

				// Allocate string memory
				LPTSTR lpszCurrentFolderPath = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get current folder path
				GetCurrentDirectory( STRING_LENGTH, lpszCurrentFolderPath );

				// Add current folder path to combo box window
				ComboBoxWindowAddString( lpszCurrentFolderPath );

				// Free string memory
				delete [] lpszCurrentFolderPath;

			} // End of unable to load folders into combo box window

			// Select first item on combo box window
			ComboBoxWindowSelectItem( 0, &ComboBoxWindowSelectionChangeFunction );

			// Main message loop
			while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
			{
				// Translate message
				TranslateMessage( &msg );

				// Dispatch message
				DispatchMessage( &msg );

			}; // End of main message loop

		} // End of successfully created main window
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

	// Free global memory
	delete [] g_lpszFoldersFilePath;

	return msg.wParam;

} // End of function WinMain
