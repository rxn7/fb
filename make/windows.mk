CC := x86_64-w64-mingw32-gcc
OBJ_DIR += obj/windows
BIN_DIR := bin/windows
CFLAGS := $(shell x86_64-w64-mingw32-sdl2-config --cflags)
LDFLAGS := $(shell x86_64-w64-mingw32-sdl2-config --libs) -lSDL2_image

include make/shared.mk

all: create_dirs copy_dlls copy_res $(OBJ) $(OUT)

copy_dlls:
	@echo -e "\e[32mCopying dlls...\e[0m" 
	@cp /usr/x86_64-w64-mingw32/bin/SDL2*.dll $(BIN_DIR)
	@cp /usr/x86_64-w64-mingw32/bin/libpng*.dll $(BIN_DIR)