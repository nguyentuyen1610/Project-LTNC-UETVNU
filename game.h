#ifndef GAME_H
#define GAME_H

#include "capy.h"
#include "pipe.h"
#include "lib.h"
#include "sound.h"

void initGame(SDL_Window** window, SDL_Renderer** renderer,
              Bird* bird, Pipe pipes[], SDL_Texture* birdTextures[],
               SDL_Texture** pipeTexture, SDL_Texture** backgroundTexture,
               SDL_Texture** gameTexture, SDL_Texture** huongdanTexture,
                TTF_Font** font, Mix_Music** bgMusic, Mix_Chunk** jumpSound);
void closeGame(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* birdTextures[],
               SDL_Texture* pipeTexture, SDL_Texture* backgroundTexture,
               SDL_Texture* scoreTexture, SDL_Texture* gameTexture,
                SDL_Texture* huongdanTexture, TTF_Font* font, Mix_Music* bgMusic, Mix_Chunk* jumpSound);
void handleInput(SDL_Event* event, Bird* bird, Mix_Chunk* jumpSound,
                  bool* gameOver, bool* paused, int* score, Pipe pipes[]);
void updateGame(Bird* bird, Pipe pipes[], int* score, bool* gameOver);
void renderGame(SDL_Renderer* renderer, Bird* bird, Pipe pipes[],
                SDL_Texture* birdTextures[], SDL_Texture* pipeTexture,
                 SDL_Texture* backgroundTexture, SDL_Texture** scoreTexture,
                  TTF_Font* font, int score, bool gameOver, int* highestScore, bool* paused);

#endif
