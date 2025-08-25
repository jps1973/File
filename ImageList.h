// ImageList.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define TEMPLATE_WINDOW_CLASS_NAME												WC_LISTBOX

HIMAGELIST ImageListGetSystem();

int ImageListGetIndex( LPCTSTR lpszItemPath );
