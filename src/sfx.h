#pragma once

#include <stdint.h>
#include <stdbool.h>

enum {
    SFX_FLAP = 0,
    SFX_SCORE,
    SFX_COUNT,
};

bool sfx_init();
void sfx_play(uint8_t sfxId);
void sfx_clean_up();