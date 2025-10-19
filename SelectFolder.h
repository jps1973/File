// SelectFolder.h

#pragma once

#include <windows.h>
#include <shlobj.h>

#include "Ascii.h"
#include "Common.h"

#define SELECT_FOLDER_DEFAULT_TITLE												( LPTSTR )"Select Folder"

BOOL SelectFolder( LPTSTR lpszFolderPath, LPTSTR lpszTitle = SELECT_FOLDER_DEFAULT_TITLE );
