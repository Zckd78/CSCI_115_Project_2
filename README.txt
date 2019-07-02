README -=- README -=- README -=- README -=- README -=- README -=- README

======================================================
	The Forbidden Cave
	Project 2 for CSCI 115
	Written by Zachary Scott
======================================================

======================================================
	HOW TO COMPILE:
	
	Either run these commands in a command prompt, or use the included
	COMPILE.bat file. 
	
		g++.exe -Wall -std=c++11 -g -g -ICommon\inc -Iinclude -Iinclude -c "main.cpp" -o obj\Debug\main.cpp.o
		g++.exe -Wall -std=c++11 -g -g -ICommon\inc -Iinclude -Iinclude -c "src\Enemy.cpp" -o obj\Debug\src\Enemy.cpp.o
		g++.exe -Wall -std=c++11 -g -g -ICommon\inc -Iinclude -Iinclude -c "src\MapController.cpp" -o obj\Debug\src\MapController.cpp.o
		g++.exe -Wall -std=c++11 -g -g -ICommon\inc -Iinclude -Iinclude -c "src\Maze.cpp" -o obj\Debug\src\Maze.cpp.o
		g++.exe -Wall -std=c++11 -g -g -ICommon\inc -Iinclude -Iinclude -c "src\Player.cpp" -o obj\Debug\src\Player.cpp.o
		g++.exe -Wall -std=c++11 -g -g -ICommon\inc -Iinclude -Iinclude -c "src\Structures.cpp" -o obj\Debug\src\Structures.cpp.o
		g++.exe -Wall -std=c++11 -g -g -ICommon\inc -Iinclude -Iinclude -c "src\Timer.cpp" -o obj\Debug\src\Timer.cpp.o
		g++.exe -LCommon\inc -Linclude -o Project02.exe obj\Debug\main.cpp.o obj\Debug\src\Enemy.cpp.o obj\Debug\src\MapController.cpp.o obj\Debug\src\Maze.cpp.o obj\Debug\src\Player.cpp.o obj\Debug\src\Structures.cpp.o obj\Debug\src\Timer.cpp.o  -static  Common\lib\libSOIL.a -lopengl32 -lglu32 -lwinmm -lgdi32 Common\lib\glut32.lib Common\lib\irrKlang.lib Common\lib\libirrKlang.a
======================================================

======================================================
	NEED HELP? QUESTIONS?

	If you have any questions, please contact me at:
		zachary.chunscott@gmail.com
======================================================

README -=- README -=- README -=- README -=- README -=- README -=- README
