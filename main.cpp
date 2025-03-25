#include "game.h"

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    Bird bird;
    Pipe pipes[2];
    SDL_Texture* birdTextures[4];
    SDL_Texture* pipeTexture = NULL;
    SDL_Texture* backgroundTexture = NULL;
    SDL_Texture* scoreTexture = NULL;
    TTF_Font* font = NULL;
    Mix_Music* bgMusic = NULL;
    Mix_Chunk* jumpSound = NULL;
    int score = 0;
    bool gameOver = false;

    initGame(&window, &renderer, &bird, pipes, birdTextures, &pipeTexture, &backgroundTexture, &font, &bgMusic, &jumpSound);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            handleInput(&event, &bird, jumpSound, &gameOver);
        }
        updateGame(&bird, pipes, &score, &gameOver);
        renderGame(renderer, &bird, pipes, birdTextures, pipeTexture, backgroundTexture, &scoreTexture, font, score);
        SDL_Delay(30);
    }

    closeGame(window, renderer, birdTextures, pipeTexture, backgroundTexture, scoreTexture, font, bgMusic, jumpSound);
    return 0;
}
