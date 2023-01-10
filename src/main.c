#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdbool.h>
#include <stdio.h>
#include "bird.h"
#include "global.h"
#include "audio.h"

SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
SDL_Texture *g_birdTexture = NULL;

static Bird bird;
static bool shouldWindowClose = false;

static bool init();
static bool init_window();
static bool init_renderer();
static bool init_img();
static void clean_up();
static void handle_event(SDL_Event *event);
static void update_title(int32_t fps);

int main(int argc, char *argv[]) {
	int32_t initStatus = init();
	if(initStatus != 0)
		return initStatus;

	SDL_Event event;
	uint64_t now = SDL_GetPerformanceCounter();
	uint64_t last = 0;
	float dt = 0.0f;
	int32_t fps = 0;
	while(!shouldWindowClose) {
		last = now;
		now = SDL_GetPerformanceCounter();
		dt = (float)(now - last) / (float)SDL_GetPerformanceFrequency();
		fps = (int)(1.0f / dt);

		while(SDL_PollEvent(&event))
			handle_event(&event);

		SDL_SetRenderDrawColor(g_renderer, 135, 206, 235, 255);
		SDL_RenderClear(g_renderer);

		update_title(fps);
		bird_update(&bird, dt);
		bird_render(&bird, g_renderer);

		SDL_RenderPresent(g_renderer);
	}

	clean_up();

	return 0;
}

static void handle_event(SDL_Event *event) {
	switch(event->type) {
		case SDL_QUIT:
			shouldWindowClose = true;
			break;

		case SDL_WINDOWEVENT:
			switch(event->window.type) {
				case SDL_WINDOWEVENT_CLOSE:
					shouldWindowClose = true;
					break;
			}
			break;

		case SDL_KEYDOWN:
			switch(event->key.keysym.sym) {
				case SDLK_ESCAPE:
					shouldWindowClose = true;
					break;

				case SDLK_SPACE:
					if(event->key.repeat == false)
						bird_jump(&bird);
					break;
			}

			break;

		case SDL_MOUSEBUTTONDOWN:
			switch(event->button.button) {
				case SDL_BUTTON_LEFT:
					bird_jump(&bird);
					break;
			}
			break;
	}
}

static void update_title(int32_t fps) {
	char title[25];
	sprintf(title, "Flappy Bird | FPS: %d", fps);
	SDL_SetWindowTitle(g_window, title);
}

static bool init() {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Failed to init SDL2: %s\n", SDL_GetError());
		return false;
	}
	SDL_Log("SDL2 initialized\n");

	if(!init_window())
		return false;

	if(!init_renderer())
		return false;

	if(!init_img())
		return false;

	audio_init();

	g_birdTexture = IMG_LoadTexture(g_renderer, "res/bird.png");
	if(!g_birdTexture) {
		SDL_Log("Failed to load bird texture\n");
		return false;
	}

	SDL_Surface *iconSurface = IMG_Load("res/icon.png");
	SDL_SetWindowIcon(g_window, iconSurface);
	SDL_FreeSurface(iconSurface);

	bird_init(&bird);

	return 0;
}

static bool init_window() {
	g_window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if(!g_window) {
		SDL_Log("Failed to create SDL2 window: %s\n", SDL_GetError());
		return false;
	}
	SDL_Log("SDL2 window created\n");
	
	// Be sure the window will never get resized.
	SDL_SetWindowResizable(g_window, SDL_FALSE);
	SDL_SetWindowMinimumSize(g_window, WINDOW_W, WINDOW_H);
	SDL_SetWindowMaximumSize(g_window, WINDOW_W, WINDOW_H);

	return true;
}

static bool init_renderer() {
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!g_renderer) {
		SDL_Log("Failed to init SDL2 renderer: %s\n", SDL_GetError());
		return false;
	}

	SDL_Log("SDL2 renderer initialized\n");
	return true;
}

static bool init_img() {
	if(!IMG_Init(IMG_INIT_PNG)) {
		SDL_Log("Failed to init SDL_Image: %s\n", IMG_GetError());
		return false;
	}

	SDL_Log("SDL2_image initialized\n");
	return true;
}

static void clean_up() {
	SDL_Log("Quitting...\n");

	SDL_DestroyTexture(g_birdTexture);
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	SDL_Quit();
	IMG_Quit();
}