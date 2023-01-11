#include "audio.h"
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_mixer.h>

static Mix_Chunk *flapSoundChunk;
static Mix_Music *music;

void audio_init() {
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) != 0)
        SDL_Log("Failed to open audio: %s\n", Mix_GetError());

    music = Mix_LoadMUS("res/music.mp3");
    if(!music)
        SDL_Log("Failed to load music file: %s\n", Mix_GetError());

    flapSoundChunk = Mix_LoadWAV("res/flap.wav");
    if(!flapSoundChunk)
        SDL_Log("Failed to load flap sound effect: %s\n", Mix_GetError());

    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
}

void audio_clean_up() {
    Mix_FreeMusic(music);
    Mix_FreeChunk(flapSoundChunk);
    Mix_CloseAudio();
    Mix_Quit();
}

void audio_play_flap() {
    Mix_PlayChannel(-1, flapSoundChunk, 0);
}