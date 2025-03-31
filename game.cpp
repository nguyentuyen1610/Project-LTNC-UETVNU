#include "game.h"

void initGame(SDL_Window** window, SDL_Renderer** renderer,
               Bird* bird, Pipe pipes[], SDL_Texture* birdTextures[],
               SDL_Texture** pipeTexture, SDL_Texture** backgroundTexture,
                SDL_Texture** gameTexture, SDL_Texture** huongdanTexture,
                TTF_Font** font, Mix_Music** bgMusic, Mix_Chunk** jumpSound) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    *window = SDL_CreateWindow("Flappy Capy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    birdTextures[0] = loadTexture(*renderer, "capy1.png");
    birdTextures[1] = loadTexture(*renderer, "capy2.png");
    birdTextures[2] = loadTexture(*renderer, "capy3.png");
    birdTextures[3] = loadTexture(*renderer, "capy4.png");
    *pipeTexture = loadTexture(*renderer, "pipe.png");
    *backgroundTexture = loadTexture(*renderer, "backg.png");
    *gameTexture = loadTexture(*renderer, "game.png");
    *huongdanTexture = loadTexture(*renderer, "huongdan.png");

    *font = TTF_OpenFont("arial.ttf", 24);
    if (!*font) {
        printf("Không thể tải font! Lỗi: %s\n", TTF_GetError());
    }

    *bgMusic = loadMusic("sound.mp3");
    if (*bgMusic) Mix_PlayMusic(*bgMusic, -1);

    *jumpSound = loadSound("sound.wav");

    initBird(bird);
    initPipes(pipes);
}

void closeGame(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* birdTextures[],
               SDL_Texture* pipeTexture, SDL_Texture* backgroundTexture,
               SDL_Texture* scoreTexture, SDL_Texture* gameTexture,
                SDL_Texture* huongdanTexture, TTF_Font* font, Mix_Music* bgMusic, Mix_Chunk* jumpSound) {
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(birdTextures[i]);
    }
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(gameTexture);
    SDL_DestroyTexture(huongdanTexture);
    Mix_FreeMusic(bgMusic);
    Mix_FreeChunk(jumpSound);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void handleInput(SDL_Event* event, Bird* bird, Mix_Chunk* jumpSound, bool* gameOver, bool* paused, int* score, Pipe pipes[]) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_SPACE) {
            if (*gameOver) {
                *gameOver = false;
                *paused = true;
                bird->y = SCREEN_HEIGHT / 2;
                bird->velocity = 0;
                *score = 0; // Reset score
                initPipes(pipes); // Reset pipes
                 pipeSpeed = 12;
            } else if (*paused) {
                *paused = false;
            } else {
                bird->velocity = -JUMP_STRENGTH;
                Mix_PlayChannel(-1, jumpSound, 0);
            }
        }
    }
}

void updateGame(Bird* bird, Pipe pipes[], int* score, bool* gameOver) {
    if (*gameOver) return;

    updateBird(bird);
    if (bird->y + BIRD_SIZE > SCREEN_HEIGHT) {
        *gameOver = true;
        return;
    }
    if (*score > 10) {
        pipeSpeed = 15;
    }
    if (*score > 20) {
        pipeSpeed = 16;
    }

    for (int i = 0; i < 2; i++) {
        pipes[i].x -= pipeSpeed;

        if (pipes[i].x + PIPE_WIDTH < 0) {
            pipes[i].x = SCREEN_WIDTH;
            pipes[i].height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
            (*score)++;
        }

        SDL_Rect birdHitbox = {bird->x + 5, bird->y + 5, BIRD_SIZE - 10, BIRD_SIZE - 10};
        SDL_Rect upperPipe = {pipes[i].x, 0, PIPE_WIDTH, pipes[i].height};
        SDL_Rect lowerPipe = {pipes[i].x, pipes[i].height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - (pipes[i].height + PIPE_GAP)};

        if (SDL_HasIntersection(&birdHitbox, &upperPipe) || SDL_HasIntersection(&birdHitbox, &lowerPipe)) {
            *gameOver = true;
            return;
        }
    }
}

void renderGame(SDL_Renderer* renderer, Bird* bird, Pipe pipes[],
                SDL_Texture* birdTextures[], SDL_Texture* pipeTexture,
                SDL_Texture* backgroundTexture, SDL_Texture** scoreTexture,
                TTF_Font* font, int score, bool gameOver, int* highestScore, bool* paused) {
    SDL_RenderClear(renderer);
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect);

    renderBird(renderer, bird, birdTextures);
    renderPipes(renderer, pipes, pipeTexture);

    char scoreText[32];
    sprintf(scoreText, "Score: %d", score);

    if (*scoreTexture) {
        SDL_DestroyTexture(*scoreTexture);
    }
    *scoreTexture = renderText(renderer, scoreText, font, {0, 0, 255, 255});

    if (*scoreTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(*scoreTexture, NULL, NULL, &textWidth, &textHeight);
        SDL_Rect scoreRect = {SCREEN_WIDTH / 2 - textWidth / 2, 20, textWidth, textHeight};
        SDL_RenderCopy(renderer, *scoreTexture, NULL, &scoreRect);
    }

    static SDL_Texture* highestScoreTexture = NULL;
    static SDL_Texture* playAgainTexture = NULL;

    if (gameOver) {
        if (score > *highestScore) {
            *highestScore = score;
            if (highestScoreTexture) {
                SDL_DestroyTexture(highestScoreTexture);
            }
            char highestScoreText[32];
            sprintf(highestScoreText, "Highest Score: %d", *highestScore);
            highestScoreTexture = renderText(renderer, highestScoreText, font,{0, 0, 255, 255});
        }

        if (highestScoreTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(highestScoreTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect highestScoreRect = {SCREEN_WIDTH / 2 - textWidth / 2, 60, textWidth, textHeight};
            SDL_RenderCopy(renderer, highestScoreTexture, NULL, &highestScoreRect);
        }

        if (!playAgainTexture) {
            char playAgainText[] = "Press SPACE to play again";
            playAgainTexture = renderText(renderer, playAgainText, font,{255, 255, 255, 255});
        }

        if (playAgainTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(playAgainTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect playAgainRect = {SCREEN_WIDTH / 2 - textWidth / 2, 120, textWidth, textHeight};
            SDL_RenderCopy(renderer, playAgainTexture, NULL, &playAgainRect);
        }
    }

    SDL_RenderPresent(renderer);
}
