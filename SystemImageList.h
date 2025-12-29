// SystemImageList.h

#pragma once

#include <windows.h>

#include "Ascii.h"
#include "Common.h"

int SystemImageListGetItemIndex( LPCTSTR lpszItemPath );

BOOL SystemImageListInit();

HIMAGELIST SystemImageListSetListView( HWND hWndListView, int nWhichImageList );
