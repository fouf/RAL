ras: resources

	gcc -g source/ral.c -l mingw32 -l SDLMain -l SDL -l comctl32 -o PvP -Wall -mwindows -mconsole resource/resource.o
resources:
	windres resource/resource.rc resource/resource.o
