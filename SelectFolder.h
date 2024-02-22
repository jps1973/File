// SelectFolder.h

#pragma once

#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>

#include "Ascii.h"
#include "Common.h"

BOOL SelectFolder( LPCTSTR lpszTitle, LPTSTR lpszFolderPath );
