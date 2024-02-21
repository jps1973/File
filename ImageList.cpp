// ImageList.cpp

#include "ImageList.h"

// Global variables
HIMAGELIST g_hImageList;

HIMAGELIST GetImageList()
{
	return g_hImageList;

} // End of function GetImageList

int ImageListGetIconIndex( LPCTSTR lpszFilePath )
{
	SHFILEINFO shfi;

	// Get icon index
	SHGetFileInfo( (LPCTSTR)lpszFilePath, 0,&shfi, sizeof(SHFILEINFO), ( SHGFI_SYSICONINDEX | SHGFI_SMALLICON ) );

	return shfi.iIcon;

} // End of function ImageListGetIconIndex

BOOL ImageListGetSystem()
{
	BOOL bResult = FALSE;

	SHFILEINFO shfi; 

	// Get system image list
	g_hImageList = ( HIMAGELIST )SHGetFileInfo( "C:\\", 0, &shfi, sizeof( SHFILEINFO ), ( SHGFI_SYSICONINDEX | SHGFI_SMALLICON ) );

	// Ensure that system image list was got
	if( g_hImageList )
	{
		// Successfully got system image list

		// Update result
		bResult = TRUE;

	} // End of successfully got system image list

	return bResult;

} // End of function ImageListGetSystem
