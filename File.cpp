// File.cpp

#include "File.h"

void ListViewWindowDoubleClickFunction( LPCTSTR lpszFilePath )
{
	// Display file path
	MessageBox( NULL, lpszFilePath, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

} // End of function ListViewWindowDoubleClickFunction

void ListViewWindowSelectionChangedFunction( LPCTSTR lpszFilePath )
{
	// Show file path on status bar window
	StatusBarWindowSetText( lpszFilePath );

} // End of function ListViewWindowSelectionChangedFunction

BOOL TreeViewWindowAddTopLevelItem( BOOL bExpand )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszTopLevelFolderPath = new char[ STRING_LENGTH ];

	// Get selected item path
	if( !( TreeViewWindowGetItemPath( lpszTopLevelFolderPath ) ) )
	{
		// Unable to get selected item path

		// Get current folder path
		GetCurrentDirectory( STRING_LENGTH, lpszTopLevelFolderPath );

	} // End of unable to get selected item path

	// Select top-level folder
	if( SelectFolder( SELECT_TOP_LEVEL_FOLDER_TITLE, lpszTopLevelFolderPath ) )
	{
		// Successfully selected top-level folder
		HTREEITEM htiTopLevel;

		// Add top-level folder to tree view window
		htiTopLevel = TreeViewWindowAddTopLevelItem( lpszTopLevelFolderPath );

		// Ensure that top-level folder was added to tree view window
		if( htiTopLevel )
		{
			// Successfully added top-level folder to tree view window

			// See if top level folder needs to be expanded
			if( bExpand )
			{
				// Top level folder needs to be expanded

				// Expand top level folder
				TreeViewWindowExpandItem( htiTopLevel );

			} // End of top level folder needs to be expanded

		} // End of successfully added top-level folder to tree view window

	} // End of successfully selected top-level folder

	// Free string memory
	delete [] lpszTopLevelFolderPath;

	return bResult;

} // End of function TreeViewWindowAddTopLevelItem

void TreeViewWindowSelectionChangedFunction( LPCTSTR lpszItemPath )
{
	int nFileCount;

	// Allocate string memory
	LPTSTR lpszStatusMessage = new char[ STRING_LENGTH ];

	// Add files to list view window
	nFileCount = ListViewWindowAddFiles( lpszItemPath, ALL_FILES_FILTER );

	// Format status message
	wsprintf( lpszStatusMessage, POPULATE_STATUS_MESSAGE_FORMAT_STRING, lpszItemPath, nFileCount );

	// Show status message on status bar window
	StatusBarWindowSetText( lpszStatusMessage );

	// Free string memory
	delete [] lpszStatusMessage;

} // End of function TreeViewWindowSelectionChangedFunction

void DoubleClickFunction( LPCTSTR lpszItemText )
{
	// Display item text
	MessageBox( NULL, lpszItemText, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

} // End of function DoubleClickFunction

void SelectionChangedFunction( LPCTSTR lpszItemText )
{
	// Show item text on status bar window
	StatusBarWindowSetText( lpszItemText );

} // End of function SelectionChangedFunction

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

LRESULT CALLBACK MainWndProc( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
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

			// Create tree view window
			if( TreeViewWindowCreate( hWndMain, hInstance ) )
			{
				// Successfully created tree view window

				// Get system image list
				ImageListGetSystem();

				// Set tree view window image list
				TreeViewWindowSetImageList( GetImageList() );

				// Set tree view window font
				TreeViewWindowSetFont( hFont );

				// Create list view window
				if( ListViewWindowCreate( hWndMain, hInstance ) )
				{
					// Successfully created list view window

					// Set list view window font
					ListViewWindowSetFont( hFont );

					// Create status bar window
					if( StatusBarWindowCreate( hWndMain, hInstance ) )
					{
						// Successfully created status bar window

						// Set status bar window font
						StatusBarWindowSetFont( hFont );

					} // End of successfully created status bar window

				} // End of successfully created list view window

			} // End of successfully created tree view window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatus;
			int nStatusWindowHeight;
			int nControlWindowHeight;
			int nListViewWindowWidth;
			int nListViewWindowLeft;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			StatusBarWindowSize();

			// Get status window size
			StatusBarWindowGetRect( &rcStatus );

			// Calculate window sizes
			nStatusWindowHeight		= ( rcStatus.bottom - rcStatus.top );
			nControlWindowHeight	= ( nClientHeight - nStatusWindowHeight );
			nListViewWindowWidth	= ( nClientWidth - ( TREE_VIEW_WINDOW_WIDTH + WINDOW_BORDER_WIDTH ) );

			// Calculate window positions
			nListViewWindowLeft = ( TREE_VIEW_WINDOW_WIDTH - WINDOW_BORDER_WIDTH );

			// Move control windows
			TreeViewWindowMove( 0, 0, TREE_VIEW_WINDOW_WIDTH, nControlWindowHeight );
			ListViewWindowMove( nListViewWindowLeft, 0, nListViewWindowWidth, nControlWindowHeight, TRUE );

			// Auto-size all list view window columns
			ListViewWindowAutoSizeAllColumns();

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on list view window
			ListViewWindowSetFocus();

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
			LPTSTR lpszFilePath = new char[ STRING_LENGTH ];

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

						// Add file path to list view window
						//ListViewWindowAddString( lpszFilePath );

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
				case IDM_FILE_NEW_TOP_LEVEL_FOLDER:
				{
					// A file new top-level folder command

					// Add top-level item to tree view window
					TreeViewWindowAddTopLevelItem( TRUE );

					// Break out of switch
					break;

				} // End of a file new top-level folder command
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy window
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

			// See if notify message is from tree view window
			if( IsTreeViewWindow( lpNmHdr->hwndFrom ) )
			{
				// Notify message is from tree view window

				// Handle notify message from tree view window
				if( !( TreeViewWindowHandleNotifyMessage( wParam, lParam, &TreeViewWindowSelectionChangedFunction ) ) )
				{
					// Notify message was not handled from tree view window

					// Call default window procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

				} // End of notify message was not handled from tree view window

			} // End of notify message is from tree view window
			else if( IsListViewWindow( lpNmHdr->hwndFrom ) )
			{
				// Notify message is from list view window

				// Handle notify message from list view window
				if( !( ListViewWindowHandleNotifyMessage( wParam, lParam, &ListViewWindowDoubleClickFunction, &ListViewWindowSelectionChangedFunction ) ) )
				{
					// Notify message was not handled from list view window

					// Call default window procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

				} // End of notify message was not handled from list view window

			} // End of notify message is from control window
			else
			{
				// Notify message is not from control window

				// Call default window procedure
				lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			} // End of notify message is not from control window

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

			// Save tree view window
			TreeViewWindowSave( FOLDERS_FILE_NAME );

			// Destroy main window
			DestroyWindow( hWndMain );

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
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWndProc

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	MSG msg;

	WNDCLASSEX wcMain;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Clear window class structure
	ZeroMemory( &wcMain, sizeof( wcMain ) );

	// Initialise main window class structure
	wcMain.cbSize			= sizeof( WNDCLASSEX );
	wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
	wcMain.lpfnWndProc		= MainWndProc;
	wcMain.hInstance		= hInstance;
	wcMain.style			= MAIN_WINDOW_CLASS_STYLE;
	wcMain.hIcon			= MAIN_WINDOW_CLASS_ICON;
	wcMain.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
	wcMain.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
	wcMain.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
	wcMain.hIconSm			= MAIN_WINDOW_CLASS_ICON;

	// Register main window class
	if( RegisterClassEx( &wcMain ) )
	{
		// Successfully registered main window class
		HWND hWndMain;

		// Create main window
		hWndMain = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TEXT, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

		// Ensure that main window was created
		if( hWndMain )
		{
			// Successfully created main window
			HMENU hMenuSystem;
			LPWSTR *lpszArgumentList;
			int nArgumentCount;

			// Get system menu
			hMenuSystem = GetSystemMenu( hWndMain, FALSE );

			// Add separator to system menu
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
				LPTSTR lpszArgument = new char[ STRING_LENGTH ];

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

					// Add argument to list view window
					//ListViewWindowAddString( lpszArgument );

				}; // End of loop through arguments

				// Free string memory
				delete [] lpszArgument;

			} // End of successfully got argument list

			// Show main window
			ShowWindow( hWndMain, nCmdShow );

			// Update main window
			UpdateWindow( hWndMain );

			// Load top-level folders
			if( TreeViewWindowLoad( FOLDERS_FILE_NAME ) == 0 )
			{
				// No top-level folders were loaded

				// Add top-level item to tree view window
				TreeViewWindowAddTopLevelItem( TRUE );

			} // End of no top-level folders were loaded

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
