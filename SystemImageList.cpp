// SystemImageList.cpp

#include "SystemImageList.h"

// Global variables
static HIMAGELIST g_hSystemImageList = NULL;

int SystemImageListGetItemIndex( LPCTSTR lpszItemPath )
{
	int nResult = 0;

	SHFILEINFO sfi;

	// Clear file information structure
	ZeroMemory( &sfi, sizeof( sfi ) );

	// Get item index
	if( SHGetFileInfo( lpszItemPath, 0, &sfi, sizeof( SHFILEINFO ), ( SHGFI_SYSICONINDEX | SHGFI_SMALLICON ) ) )
	{
		// Successfully got item index

		// Update return value
		nResult = sfi.iIcon;

	} // End of successfully got item index

	return nResult;

} // End of function SystemImageListGetItemIndex

BOOL SystemImageListInit()
{
	BOOL bResult = FALSE;

	// Ensure that system image list has not already been initialised
	if( g_hSystemImageList == NULL )
	{
		// System image list has not already been initialised
		SHFILEINFO sfi;

		// Clear file information structure
		ZeroMemory( &sfi, sizeof( sfi ) );

		// Get system image list
		g_hSystemImageList = ( HIMAGELIST)( SHGetFileInfo( "C:\\", 0, &sfi, sizeof(sfi), ( SHGFI_SYSICONINDEX | SHGFI_SMALLICON ) ) );

		// Ensure that system image list was got
		if( g_hSystemImageList )
		{
			// Successfully got system image list

			// Update return value
			bResult = TRUE;

		} // End of successfully got system image list

	} // End of system image list has not already been initialised

	return bResult;

} // End of function SystemImageListInit

HIMAGELIST SystemImageListSetListView( HWND hWndListView, int nWhichImageList )
{
	// Set list view image list
	return ( HIMAGELIST )SendMessage( hWndListView, LVM_SETIMAGELIST, ( WPARAM )nWhichImageList, ( LPARAM )g_hSystemImageList );

} // End of function SystemImageListSetListView
