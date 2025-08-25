// ImageList.cpp

#include "ImageList.h"

HIMAGELIST ImageListGetSystem()
{
	HIMAGELIST hImageList = NULL;

	SHFILEINFO sfi;

	// Get system image list
	hImageList = ( HIMAGELIST )SHGetFileInfo( NULL, 0, &sfi, sizeof( SHFILEINFO ), ( SHGFI_SYSICONINDEX | SHGFI_SMALLICON ) );

	return hImageList;

} // End of function ImageListGetSystem

int ImageListGetIndex( LPCTSTR lpszItemPath )
{
	int nResult = -1;

	SHFILEINFO sfi;

	// Get image list index
	if( SHGetFileInfo( lpszItemPath, 0, &sfi, sizeof( SHFILEINFO ), ( SHGFI_SYSICONINDEX | SHGFI_SMALLICON ) ) )
	{
		// Successfully got image list index

		// Update return value
		nResult = sfi.iIcon;

	} // End of successfully got image list index

	return nResult;

} // End of function ImageListGetIndex
