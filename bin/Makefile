checks = -Wall -pedantic-errors
libs = -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
no_console = -Wl,-subsystem,windows

default:
	g++ -std=c++17 -O2 -c ../src/*.cpp -I ../include $(checks)
	g++ ./*.o -o ObjectiveBeach $(libs) $(no_console)
