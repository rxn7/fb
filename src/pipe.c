#include "pipe.h"
#include "global.h"
#include "mathHelper.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

void pipe_init(Pipe *pipe, float x) {
    pipe->x = x;
    pipe_restart(pipe);
}

void pipe_restart(Pipe *pipe) {
    pipe->scored = false;
    pipe->gapPosition = rand_range(10, PIPE_H-10);
}

void pipe_update(Pipe *pipe, float dt) {
    pipe->x -= dt * PIPE_SCROLL_SPEED;

    if(pipe->x + PIPE_W <= 0) {
        pipe_init(pipe, pipe->x + PIPE_COUNT * GAP_BETWEEN_PIPES);
    }
}

void pipe_render(Pipe *pipe) {
    SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);

    SDL_FRect topPipeRect = (SDL_FRect){
        .h = PIPE_H,
        .w = PIPE_W,
        .x = pipe->x,
        .y = pipe->gapPosition - PIPE_H,
    };

    SDL_FRect bottomPipeRect = (SDL_FRect){
        .w = PIPE_W,
        .h = PIPE_H,
        .x = pipe->x,
        .y = WINDOW_H - PIPE_H + pipe->gapPosition,
    };

    SDL_RenderCopyExF(g_renderer, g_pipeTexture, NULL, &topPipeRect, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyExF(g_renderer, g_pipeTexture, NULL, &bottomPipeRect, 0, NULL, SDL_FLIP_VERTICAL);
}

bool pipe_has_just_scored(Pipe *pipe) {
    if(pipe->scored)
        return false;

    if(pipe->x + PIPE_W * 0.5f <= WINDOW_W * 0.5f)
        pipe->scored = true;

    return pipe->scored;
}