prog: main.c level.c level.h
	gcc main.c level.c level.h -o prog $(sdl2-config --cflags --libs) -lSDL2_image -g3
