#include "sfx.h"
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_mixer.h>

static const char *sfxPaths[SFX_COUNT] = {
    "res/flap.wav",
    "res/score.wav"
};
static Mix_Chunk *chunks[SFX_COUNT];
static Mix_Music *music;

bool sfx_init() {
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) != 0) {
        SDL_Log("Failed to open audio: %s\n", Mix_GetError());
        return false;
    }

    music = Mix_LoadMUS("res/music.mp3");
    if(!music) {
        SDL_Log("Failed to load music file: %s\n", Mix_GetError());
        return false;
    }

    // Load all sound effects
    for(uint8_t i=0; i<SFX_COUNT; ++i) {
        chunks[i]  = Mix_LoadWAV(sfxPaths[i]);
        if(!chunks[i]) {
            SDL_Log("Failed to load %s sound effect: %s\n", sfxPaths[i], Mix_GetError());
            return false;
        }
    }

    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);

    return true;
}

void sfx_clean_up() {
    Mix_FreeMusic(music);

    for(uint8_t i=0; i<SFX_COUNT; ++i)
        Mix_FreeChunk(chunks[i]);

    Mix_CloseAudio();
    Mix_Quit();
}

void sfx_play(uint8_t sfxId) {
    if(sfxId < 0 || sfxId >= SFX_COUNT) {
        SDL_Log("Failed to play sound of id: %d, it's out of range (0-%d)\n", sfxId, SFX_COUNT);
        return;
    }

    Mix_PlayChannel(-1, chunks[sfxId], 0);
}