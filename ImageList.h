// ImageList.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

HIMAGELIST GetImageList();

int ImageListGetIconIndex( LPCTSTR lpszFilePath );

BOOL ImageListGetSystem();
