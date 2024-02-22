cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra File.cpp ImageList.cpp SelectFolder.cpp ListBoxWindow.cpp StatusBarWindow.cpp TreeViewWindow.cpp Resource.o -o File.exe
