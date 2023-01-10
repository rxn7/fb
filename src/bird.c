#include <SDL2/SDL_render.h>
#include <stdio.h>
#include "global.h"
#include "bird.h"

void bird_init(Bird *bird) {
    bird->srcRect = (SDL_Rect) {
        .x = 0,
        .y = 0
    };

    SDL_QueryTexture(g_birdTexture, NULL, NULL , &bird->srcRect.w, &bird->srcRect.h);
    bird->srcRect.w /= BIRD_SPRITE_PARTS;

    bird->destRect.w = bird->srcRect.w * BIRD_SCALE;
    bird->destRect.h = bird->srcRect.h * BIRD_SCALE;

    bird_restart(bird);
}

void bird_restart(Bird *bird) {
    bird->position = (WINDOW_H - bird->destRect.h) * 0.5f;
    bird->rotation = 0;
    bird->velocity = 0;
}

void bird_update(Bird *bird, float dt) {
    bird->velocity += BIRD_GRAVITY * dt;
    if(bird->velocity > BIRD_MAX_VELOCITY)
        bird->velocity = BIRD_MAX_VELOCITY;

    bird->position += bird->velocity * dt;

    float halfHeight = bird->destRect.w * 0.5f;
    if(bird->position <= halfHeight || bird->position >= WINDOW_H - halfHeight)
        bird_die(bird);
}

void bird_render(Bird *bird, SDL_Renderer *renderer) {
    bird->destRect.x = (float)(WINDOW_W - bird->destRect.h) * 0.5f;
    bird->destRect.y = bird->position - bird->destRect.h * 0.5f;

    SDL_RenderCopy(renderer, g_birdTexture, &bird->srcRect, &bird->destRect);
}

void bird_die(Bird *bird) {
    printf("Game Over...\n");
    bird_restart(bird);
}
