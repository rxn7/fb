#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

#define BIRD_SPRITE_PARTS 4
#define BIRD_SCALE 2.5f
#define BIRD_GRAVITY 150.f
#define BIRD_MAX_VELOCITY 300.0f
#define BIRD_JUMP_VELOCITY -150.0f

typedef struct Bird {
    float position;
    float velocity;
    int32_t angle;
    SDL_Rect srcRect;
    SDL_FRect destRect;
} Bird;

void bird_init(Bird *bird);
void bird_restart(Bird *bird);
void bird_update(Bird *bird, float dt);
void bird_render(Bird *bird, SDL_Renderer *renderer);
void bird_die(Bird *bird);
void bird_jump(Bird *bird);