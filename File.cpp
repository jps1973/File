// File.cpp

#include "File.h"

void FolderTreeViewWindowDoubleClickFunction( LPCTSTR lpszItemText )
{
	// Display item text
	MessageBox( NULL, lpszItemText, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

} // End of function FolderTreeViewWindowDoubleClickFunction

void FolderTreeViewWindowSelectionChangedFunction( LPCTSTR lpszItemText )
{
	// Show item text on status bar window
	StatusBarWindowSetText( lpszItemText );

} // End of function FolderTreeViewWindowSelectionChangedFunction

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= NULL;
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

			// Get instance
			hInstance = GetModuleHandle( NULL );

			// Create folder tree view window
			if( FolderTreeViewWindowCreate( hWndMain, hInstance ) )
			{
				// Successfully created folder tree view window
				HFONT hFont;

				// Get font
				hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

				// Set folder tree view window font
				FolderTreeViewWindowSetFont( hFont );

				// Create status bar window
				if( StatusBarWindowCreate( hWndMain, hInstance ) )
				{
					// Successfully created status bar window

					// Set status bar window font
					StatusBarWindowSetFont( hFont );

				} // End of successfully created status bar window

			} // End of successfully created folder tree view window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatusBar;
			int nStatusBarWindowHeight;
			int nFolderTreeViewWindowHeight;

			// Get client size
			nClientWidth	= LOWORD( lParam );
			nClientHeight	= HIWORD( lParam );

			// Size status bar window
			StatusBarWindowSize();

			// Get status bar window size
			StatusBarWindowGetRect( &rcStatusBar );

			// Calculate window sizes
			nStatusBarWindowHeight	= ( rcStatusBar.bottom - rcStatusBar.top );
			nFolderTreeViewWindowHeight	= ( nClientHeight - nStatusBarWindowHeight );

			// Move control windows
			FolderTreeViewWindowMove( 0, 0, nClientWidth, nFolderTreeViewWindowHeight, TRUE );

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on folder tree view window
			FolderTreeViewWindowSetFocus();

			// Break out of switch
			break;

		} // End of an activate message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO *lpMinMaxInfo = ( MINMAXINFO * )lParam;

			// Update minimum size
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
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy main window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				default:
				{
					// Default command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

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

					// Call default window procedure
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

			// Get notify message information
			lpNmHdr = ( LPNMHDR )lParam;

			// See if notify message is from folder tree view window
			if( IsFolderTreeViewWindow( lpNmHdr->hwndFrom ) )
			{
				// Notify message is from folder tree view window

				// Handle notify message from folder tree view window
				if( !( FolderTreeViewWindowHandleNotifyMessage( wParam, lParam, &FolderTreeViewWindowSelectionChangedFunction, &FolderTreeViewWindowDoubleClickFunction ) ) )
				{
					// Notify message was not handled from folder tree view window

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

				} // End of notify message was not handled from folder tree view window

			} // End of notify message is from folder tree view window
			else
			{
				// Notify message is not from folder tree view window

				// Call default procedure
				lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			} // End of notify message is not from folder tree view window

			// Break out of switch
			break;

		} // End of a notify message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			HMENU hMenuContext;

			// Load context menu
			hMenuContext = LoadMenu( NULL, MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show context menu
			TrackPopupMenu( GetSubMenu( hMenuContext, 0 ), ( TPM_LEFTALIGN | TPM_LEFTBUTTON ), LOWORD( lParam ), HIWORD( lParam ), 0, hWndMain, NULL );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message

			// Save folder tree window
			if( FolderTreeViewWindowSave( FOLDERS_FILE_NAME ) )
			{
				// Successfully saved folder tree window

				// Destroy main window
				DestroyWindow( hWndMain );

			} // End of successfully saved folder tree window
			else
			{
				// Unable to save folder tree window
				
				// Ensure that user is ok to continue
				if( MessageBox( hWndMain, FOLDER_TREE_VIEW_WINDOW_UNABLE_TO_SAVE_FOLDERS_WARNING_MESSAGE, WARNING_MESSAGE_CAPTION, ( MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2 ) ) == IDYES )
				{
					// User is ok to continue

					// Destroy main window
					DestroyWindow( hWndMain );

				} // End of user is ok to continue

			} // End of unable to save folder tree window

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

			// Call default procedure
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWindowProcedure

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,  LPSTR, int nCmdShow )
{
	MSG msg;

	WNDCLASSEX wcMain;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Clear main window class structure
	ZeroMemory( &wcMain, sizeof( wcMain ) );

	// Initialise main window class structure
	wcMain.cbSize			= sizeof( WNDCLASSEX );
	wcMain.style			= MAIN_WINDOW_CLASS_STYLE;
	wcMain.lpfnWndProc		= MainWindowProcedure;
	wcMain.hInstance		= hInstance;
	wcMain.hIcon			= MAIN_WINDOW_CLASS_ICON;
	wcMain.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
	wcMain.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
	wcMain.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
	wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
	wcMain.hIconSm			= MAIN_WINDOW_CLASS_ICON;

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

			// Get system menu
			hMenuSystem = GetSystemMenu( hWndMain, FALSE );

			// Add separator to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

			// Add about item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

			// Show main window
			ShowWindow( hWndMain, nCmdShow );

			// Update main window
			UpdateWindow( hWndMain );

			// Load folders into folder tree view window
			if( FolderTreeViewWindowLoad( FOLDERS_FILE_NAME ) == 0 )
			{
				// Unable to load folders into folder tree view window

				// Allocate string memory
				LPTSTR lpszFolderPath = new char[ STRING_LENGTH ];

				// Get current folder path
				if( GetCurrentDirectory( STRING_LENGTH, lpszFolderPath ) )
				{
					// Successfully got current folder path
					HTREEITEM htiFolder;

					// Add folder path to folder tree view window
					htiFolder = FolderTreeViewWindowInsertItem( lpszFolderPath );

					// Ensure that folder path was added to folder tree view window
					if( htiFolder )
					{
						// Successfully added folder path to folder tree view window

						// Add dummy sub-item under folder path
						FolderTreeViewWindowInsertItem( FOLDER_TREE_VIEW_WINDOW_DUMMY_ITEM_TEXT, htiFolder, TVI_SORT );

					} // End of successfully added folder path to folder tree view window

				} // End of successfully got current folder path

				// Free string memory
				delete [] lpszFolderPath;

			} // End of unable to load folders into folder tree view window

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

	return msg.wParam;

} // End of function WinMain