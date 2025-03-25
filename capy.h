#ifndef CAPY_H
#define CAPY_H

#include "head.h"

#define BIRD_SIZE 40
#define GRAVITY 1
#define JUMP_STRENGTH 10
#define FRAME_TIME 200

typedef struct {
    int x, y;
    int velocity;
    int frame;
    Uint32 lastFrameTime;
} Bird;

void initBird(Bird* bird);
void updateBird(Bird* bird);
void renderBird(SDL_Renderer* renderer, Bird* bird, SDL_Texture* birdTextures[]);

#endif // CAPY_H
