#include "capy.h"
#include "lib.h"
void initBird(Bird* bird) {
    bird->x = SCREEN_WIDTH / 4;
    bird->y = SCREEN_HEIGHT / 2;
    bird->velocity = 0;
    bird->frame = 0;
    bird->lastFrameTime = SDL_GetTicks();
}

void updateBird(Bird* bird) {
    bird->velocity += GRAVITY;
    bird->y += bird->velocity;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - bird->lastFrameTime >= FRAME_TIME) {
        bird->frame = (bird->frame + 1) % 4;
        bird->lastFrameTime = currentTime;
    }
}

void renderBird(SDL_Renderer* renderer, Bird* bird, SDL_Texture* birdTextures[]) {
    SDL_Rect birdRect = { bird->x, bird->y, BIRD_SIZE, BIRD_SIZE };
    SDL_RenderCopy(renderer, birdTextures[bird->frame], NULL, &birdRect);
}
