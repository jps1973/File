// BrowseForFolder.cpp

#include "BrowseForFolder.h"

INT CALLBACK BrowseCallbackProc( HWND hWndBrowse, UINT uMessage, LPARAM, LPARAM )
{
	int nResult = 0;

	// Select message
	switch( uMessage )
	{
		case BFFM_INITIALIZED:
		{
			// A browse for folder initialised message

			// Allocate string memory
			LPTSTR lpszFolderPath = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get current folder
			if( GetCurrentDirectory( STRING_LENGTH, lpszFolderPath ) )
			{
				// Successfully got current folder

				// Select current folder
				SendMessage( hWndBrowse, BFFM_SETSELECTION, ( WPARAM )TRUE, ( LPARAM )lpszFolderPath );

			} // End of successfully got current folder

			// Free string memory
			delete [] lpszFolderPath;

			// Break out of switch
			break;

		} // End of a browse for folder initialised message

	}; // End of selection for message

	return nResult;

} // End of function BrowseCallbackProc

BOOL BrowseForFolder( LPCTSTR lpszTitle, LPTSTR lpszFolderPath, HWND hWndOwner )
{
	BOOL bResult = FALSE;

	BROWSEINFO bi;
	LPITEMIDLIST lpItemIdList;

	// Clear browse information structure
	ZeroMemory( &bi, sizeof( bi ) );

	// Initialise browse information structure
	bi.hwndOwner		= hWndOwner;
	bi.pidlRoot			= NULL;
	bi.pszDisplayName	= lpszFolderPath;
	bi.lpszTitle		= lpszTitle;
	bi.ulFlags			= 0;
	bi.lpfn				= BrowseCallbackProc;
	bi.lParam			= 0;
	bi.iImage			= -1;

	// Browse for folder
	lpItemIdList = SHBrowseForFolder( &bi );

	// Ensure that browse was successful
	if( lpItemIdList )
	{
		// Successfully browsed for folder

		// Get folder path
		if( SHGetPathFromIDList( lpItemIdList, lpszFolderPath ) )
		{
			// Successfully got folder path

			// Update return value
			bResult = TRUE;

		} // End of successfully got folder path

	} // End of successfully browsed for folder

	return bResult;

} // End of function BrowseForFolder
