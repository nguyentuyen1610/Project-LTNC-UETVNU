#ifndef SOUND_H
#define SOUND_H

#include "head.h"

Mix_Music* loadMusic(const char* path);
Mix_Chunk* loadSound(const char* path);

#endif // SOUND_
