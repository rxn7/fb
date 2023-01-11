CC := gcc
OBJ_DIR += obj/linux
BIN_DIR := bin/linux
LDFLAGS += $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf) -lm

include make/shared.mk

all: create_dirs copy_res $(OBJ) $(OUT)