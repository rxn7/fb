#pragma once

#include "global.h"
#include <SDL2/SDL_render.h>

#define PIPE_SCROLL_SPEED 50

#define PIPE_W 65
#define PIPE_H 373
#define PIPE_GAP WINDOW_H - PIPE_H

#define PIPE_COUNT 3
#define GAP_BETWEEN_PIPES 300

typedef struct Pipe {
    float gapPosition;
    float x;
} Pipe;

void pipe_init(Pipe *pipe, float x);
void pipe_restart(Pipe *pipe);
void pipe_update(Pipe *pipe, float dt);
void pipe_render(Pipe *pipe);