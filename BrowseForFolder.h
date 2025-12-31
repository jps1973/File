// BrowseForFolder.h

#pragma once

#include <windows.h>
#include <shlobj.h>

#include "Ascii.h"
#include "Common.h"

#define BROWSE_FOR_FOLDER_TITLE													"Select Folder"

BOOL BrowseForFolder( LPCTSTR lpszTitle, LPTSTR lpszFolderPath, HWND hWndParent = NULL );
