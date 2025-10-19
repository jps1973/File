// SelectFolder.cpp

#include "SelectFolder.h"

// Global variables
LPTSTR g_lpszInitialFolderPath;

int CALLBACK BrowseCallbackProcedure( HWND hWndParent, UINT uMessage, LPARAM, LPARAM )
{
	int nResult = 0;

	// Select message
	switch( uMessage )
	{
		case BFFM_INITIALIZED:
		{
			// An initialised message

			// Select initial folder
			SendMessage( hWndParent, BFFM_SETSELECTION, ( WPARAM )TRUE, ( LPARAM )g_lpszInitialFolderPath );

			// Break out of switch
			break;

		} // End of an initialised message

	}; // End of selection for message

	return nResult;

} // End of function BrowseCallbackProcedure

BOOL SelectFolder( LPTSTR lpszFolderPath, LPTSTR lpszTitle )
{
	BOOL bResult = FALSE;

	BROWSEINFO browseInformation;
	LPITEMIDLIST lpItemIdList;

	// Allocate string memory
	g_lpszInitialFolderPath = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear browse information structure
	ZeroMemory( &browseInformation, sizeof( browseInformation ) );

	// Initialise browse information structure
	browseInformation.hwndOwner			= NULL;
	browseInformation.pidlRoot			= NULL;
	browseInformation.pszDisplayName	= lpszFolderPath;
	browseInformation.lpszTitle			= lpszTitle;
	browseInformation.ulFlags			= ( BIF_RETURNONLYFSDIRS | BIF_USENEWUI );
	browseInformation.lParam			= ( LPARAM )NULL;
	browseInformation.iImage			= 0;
	browseInformation.lpfn				= BrowseCallbackProcedure;

	// Set initial folder path
	lstrcpy( g_lpszInitialFolderPath, lpszFolderPath );

	// Browse for folder
	lpItemIdList = SHBrowseForFolder( &browseInformation );

	// Ensure that folder was selected
	if( lpItemIdList )
	{
		// Successfully selected folder

		// Get path from item id list
		bResult = SHGetPathFromIDList( lpItemIdList, lpszFolderPath );

	} // End of successfully selected folder

	// Free string memory
	delete [] g_lpszInitialFolderPath;

	return bResult;

} // End of function SelectFolder
