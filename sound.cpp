#include "sound.h"

Mix_Music* loadMusic(const char* path) {
    Mix_Music* music = Mix_LoadMUS(path);
    if (!music) {
        printf("Không thể tải nhạc %s! Lỗi: %s\n", path, Mix_GetError());
    }
    return music;
}

Mix_Chunk* loadSound(const char* path) {
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (!sound) {
        printf("Không thể tải âm thanh %s! Lỗi: %s\n", path, Mix_GetError());
    }
    return sound;
}
