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
    SDL_Texture* gameTexture = NULL;
    SDL_Texture* huongdanTexture = NULL;
    TTF_Font* font = NULL;
    Mix_Music* bgMusic = NULL;
    Mix_Chunk* jumpSound = NULL;
    int score = 0;
    int highestScore = 0;
    bool gameOver = false;
    bool paused = true;

    initGame(&window, &renderer, &bird, pipes, birdTextures, &pipeTexture,
             &backgroundTexture, &gameTexture, &huongdanTexture, &font, &bgMusic, &jumpSound);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_Rect gameRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    SDL_RenderCopy(renderer, gameTexture, NULL, &gameRect);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_Rect huongdanRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    SDL_RenderCopy(renderer, huongdanTexture, NULL, &huongdanRect);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            handleInput(&event, &bird, jumpSound, &gameOver, &paused, &score, pipes);
        }

        if (!paused) {
            updateGame(&bird, pipes, &score, &gameOver);
        }

        renderGame(renderer, &bird, pipes, birdTextures, pipeTexture,
                   backgroundTexture, &scoreTexture, font, score, gameOver, &highestScore, &paused);

        SDL_Delay(30);
    }

    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = NULL;
    }

    closeGame(window, renderer, birdTextures, pipeTexture, backgroundTexture,
              gameTexture, huongdanTexture, font, bgMusic, jumpSound);

    return 0;
}
