#pragma once
#include <SDL2/SDL_render.h>
#include <stdint.h>

#define WINDOW_W 640
#define WINDOW_H 480

extern SDL_Renderer *g_renderer;
extern SDL_Window *g_window;
extern SDL_Texture *g_birdTexture;
extern SDL_Texture *g_pipeTexture;