#ifndef GAME_H
#define GAME_H

#include "capy.h"
#include "pipe.h"
#include "lib.h"
#include "sound.h"

void initGame(SDL_Window** window, SDL_Renderer** renderer, Bird* bird, Pipe pipes[], SDL_Texture* birdTextures[], SDL_Texture** pipeTexture, SDL_Texture** backgroundTexture, TTF_Font** font, Mix_Music** bgMusic, Mix_Chunk** jumpSound);
void closeGame(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* birdTextures[], SDL_Texture* pipeTexture, SDL_Texture* backgroundTexture, SDL_Texture* scoreTexture, TTF_Font* font, Mix_Music* bgMusic, Mix_Chunk* jumpSound);
void handleInput(SDL_Event* event, Bird* bird, Mix_Chunk* jumpSound, bool* gameOver);
void updateGame(Bird* bird, Pipe pipes[], int* score, bool* gameOver);
void renderGame(SDL_Renderer* renderer, Bird* bird, Pipe pipes[], SDL_Texture* birdTextures[], SDL_Texture* pipeTexture, SDL_Texture* backgroundTexture, SDL_Texture** scoreTexture, TTF_Font* font, int score);

#endif // GAME_H
