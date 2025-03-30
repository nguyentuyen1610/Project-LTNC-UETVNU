#include "sound.h"

Mix_Music* loadMusic(const char* path) {
    Mix_Music* music = Mix_LoadMUS(path);
    if (!music) {
        printf("loi am thanh", path, Mix_GetError());
    }
    return music;
}

Mix_Chunk* loadSound(const char* path) {
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (!sound) {
        printf("loi am thanh", path, Mix_GetError());
    }
    return sound;
}
