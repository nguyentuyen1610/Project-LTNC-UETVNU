#ifndef LIB_H
#define LIB_H

#include "head.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);
SDL_Texture* renderText(SDL_Renderer* renderer, const char* text, TTF_Font* font, SDL_Color color);

#endif // LIB_H
