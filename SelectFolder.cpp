// SelectFolder.cpp

#include "SelectFolder.h"

static int CALLBACK BrowseCallbackProc( HWND hWnd, UINT uMsg, LPARAM, LPARAM lpData )
{
	int nResult = 0;

	// Select message
	switch( uMsg )
	{
		case BFFM_INITIALIZED:
		{
			// An initialised message

			// Select folder path
			SendMessage( hWnd, BFFM_SETSELECTION, ( WPARAM )TRUE, ( LPARAM )lpData );

		} // End of an initialised message

	}; // End of selection for message

	return nResult;

} // End of function BrowseCallbackProc

BOOL SelectFolder( LPCTSTR lpszTitle, LPTSTR lpszFolderPath )
{
	BOOL bResult = FALSE;

	BROWSEINFO bi;
	LPITEMIDLIST pidl;

	// Clear browse information structure
	ZeroMemory( &bi, sizeof( bi ) );

	// Initialise browse information structure
	bi.lpszTitle	= lpszTitle;
	bi.ulFlags		= ( BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE );
	bi.lpfn			= BrowseCallbackProc;
	bi.lParam		= ( LPARAM )lpszFolderPath;

	// Browse for folder
	pidl = SHBrowseForFolder ( &bi );

	// Ensure that folder was selected
	if( pidl != 0 )
	{
		// Successfully selected folder
		IMalloc *lpIMalloc = 0;

		// Get folder path
		SHGetPathFromIDList ( pidl, lpszFolderPath );

		// Get memory
		if( SUCCEEDED( SHGetMalloc( &lpIMalloc ) ) )
		{
			// Successfully got memory

			// Free memory
			lpIMalloc->Free( pidl );

			// Release memory
			lpIMalloc->Release();

		} // End of successfully got memory

		// Update return value
		bResult = TRUE;

	} // End of successfully selected folder

	return bResult;

} // End of function SelectFolder
/*
std::string BrowseFolder(std::string saved_path)
{
TCHAR path[MAX_PATH];

const char * path_param = saved_path.c_str();

BROWSEINFO bi = { 0 };
bi.lpszTitle  = ("Browse for folder...");
bi.ulFlags    = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
bi.lpfn       = BrowseCallbackProc;
bi.lParam     = (LPARAM) path_param;

LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );

if ( pidl != 0 )
{
//get the name of the folder and put it in path
SHGetPathFromIDList ( pidl, path );

//free memory used
IMalloc * imalloc = 0;
if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
{
imalloc->Free ( pidl );
imalloc->Release ( );
}

return path;
}

return "";
}*/