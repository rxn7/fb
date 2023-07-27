#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "bird.h"
#include "global.h"
#include "sfx.h"
#include "pipe.h"

SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
SDL_Texture *g_birdTexture = NULL;
SDL_Texture *g_pipeTexture = NULL;

static SDL_Texture  *scoreTextTexture;
static TTF_Font *font;
static Bird bird;
static Pipe pipes[PIPE_COUNT];
static bool shouldWindowClose = false;
static uint32_t score = 0;
static float scrollSpeed = PIPE_SCROLL_SPEED;

static bool init();
static bool init_window();
static bool init_renderer();
static bool init_ttf();
static void init_pipes();
static bool init_img();
static void clean_up();
static void game_over();
static void render_score_text();
static void handle_event(SDL_Event *event);
static void update_title(int32_t fps);
static void set_score(uint32_t score);
static void bird_pipe_collision_check(Bird *bird, Pipe *pipe);

int main(int argc, char *argv[]) {
	if(!init())
		return 1;

	set_score(0);

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
		render_score_text();

		scrollSpeed += dt * 5.0f;
		if(scrollSpeed >= PIPE_MAX_SCROLL_SPEED) scrollSpeed = PIPE_MAX_SCROLL_SPEED;

		Pipe *pipe;
		for(uint8_t i=0; i<PIPE_COUNT; ++i) {
			pipe = &pipes[i];

			pipe_update(pipe, scrollSpeed, dt);
			bird_pipe_collision_check(&bird, pipe);

			if(pipe_has_just_scored(pipe)) {
				set_score(score+1);
				sfx_play(SFX_SCORE);
			}

			pipe_render(pipe);
		}

		bird_update(&bird, dt);
		bird_render(&bird, g_renderer);

		if(bird_is_colliding_with_frame(&bird))
			game_over();

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

void bird_pipe_collision_check(Bird *bird, Pipe *pipe) {
    float halfBirdHeight = bird->destRect.h * 0.5f;
    float halfBirdWidth = bird->destRect.w * 0.5f;
    if((pipe->x < WINDOW_W * 0.5f + halfBirdWidth - BIRD_COLLISION_MARGIN && pipe->x + PIPE_W > WINDOW_W * 0.5f + BIRD_COLLISION_MARGIN) && (bird->position - halfBirdHeight + BIRD_COLLISION_MARGIN < pipe->gapPosition || bird->position + halfBirdHeight - BIRD_COLLISION_MARGIN > pipe->gapPosition + PIPE_GAP)) {
		game_over();
    }
}

static void game_over() {
	scrollSpeed = PIPE_SCROLL_SPEED;
	set_score(0);
	bird_die(&bird);
	init_pipes();
}

static void update_title(int32_t fps) {
	char buffer[25];
	sprintf(buffer, "Flappy Bird | FPS: %d", fps);
	SDL_SetWindowTitle(g_window, buffer);
}

static void render_score_text() {
	SDL_Rect rect = {
		.x = WINDOW_W * 0.5f,
		.y = WINDOW_H * 0.5f,
	};
	SDL_QueryTexture(scoreTextTexture, NULL, NULL, &rect.w, &rect.h);

	rect.x -= rect.w * 0.5f;
	rect.y -= rect.h * 0.5f;

	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderCopy(g_renderer, scoreTextTexture, NULL, &rect);
}

static void set_score(uint32_t newScore) {
	score = newScore;

	char buffer[5];
	sprintf(buffer, "%d", score);

	SDL_Surface *surface = TTF_RenderText_Solid(font, buffer, (SDL_Color){255,255,255,255});
	scoreTextTexture = SDL_CreateTextureFromSurface(g_renderer, surface);
	SDL_FreeSurface(surface);
}

static bool init() {
	srand(time(NULL));

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

	if(!init_ttf())
		return false;

	if(!sfx_init())
		return false;

	bird_init(&bird);
	init_pipes();

	return true;
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

	g_birdTexture = IMG_LoadTexture(g_renderer, "res/bird.png");
	if(!g_birdTexture) {
		SDL_Log("Failed to load bird texture\n");
		return false;
	}

	g_pipeTexture = IMG_LoadTexture(g_renderer, "res/pipe.png");
	if(!g_pipeTexture) {
		SDL_Log("Failed to load pipe texture\n");
		return false;
	}

	SDL_Surface *iconSurface = IMG_Load("res/icon.png");
	SDL_SetWindowIcon(g_window, iconSurface);
	SDL_FreeSurface(iconSurface);

	return true;
}

static bool init_ttf() {
	if(TTF_Init() != 0) {
		SDL_Log("Failed to init SDL_TTF: %s\n", TTF_GetError());
		return false;
	}

	font = TTF_OpenFont("res/font.ttf", 64);
	if(!font) {
		SDL_Log("Failed to open font: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

static void init_pipes() {
	pipe_init(&pipes[0], WINDOW_W);
	for(uint8_t i = 1; i < PIPE_COUNT; ++i)
		pipe_init(&pipes[i], pipes[i-1].x + GAP_BETWEEN_PIPES);
}

static void clean_up() {
	SDL_DestroyTexture(g_birdTexture);
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	SDL_DestroyTexture(scoreTextTexture);
	SDL_Quit();

	IMG_Quit();

	TTF_CloseFont(font);
	TTF_Quit();

	sfx_clean_up();
}
