cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra ^
 ^
 Tab.cpp ^
 ^
 ControlWindow.cpp ^
 StatusBarWindow.cpp ^
 TabControlWindow.cpp ^
 ^
 Resource.o ^
 ^
 -o Tab.exe
