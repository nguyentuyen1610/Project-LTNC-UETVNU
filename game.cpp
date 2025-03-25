#include "game.h"


void initGame(SDL_Window** window, SDL_Renderer** renderer, Bird* bird, Pipe pipes[], SDL_Texture* birdTextures[], SDL_Texture** pipeTexture, SDL_Texture** backgroundTexture, TTF_Font** font, Mix_Music** bgMusic, Mix_Chunk** jumpSound) {
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

void closeGame(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* birdTextures[], SDL_Texture* pipeTexture, SDL_Texture* backgroundTexture, SDL_Texture* scoreTexture, TTF_Font* font, Mix_Music* bgMusic, Mix_Chunk* jumpSound) {
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(birdTextures[i]);
    }
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(scoreTexture);
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

void handleInput(SDL_Event* event, Bird* bird, Mix_Chunk* jumpSound, bool* gameOver) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_SPACE && !*gameOver) {
            bird->velocity = -JUMP_STRENGTH;
            Mix_PlayChannel(-1, jumpSound, 0);
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

    for (int i = 0; i < 2; i++) {
        pipes[i].x -= PIPE_SPEED;

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

void renderGame(SDL_Renderer* renderer, Bird* bird, Pipe pipes[], SDL_Texture* birdTextures[], SDL_Texture* pipeTexture, SDL_Texture* backgroundTexture, SDL_Texture** scoreTexture, TTF_Font* font, int score) {
    SDL_RenderClear(renderer);
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect);

    renderBird(renderer, bird, birdTextures);
    renderPipes(renderer, pipes, pipeTexture);

    // Display score
    char scoreText[32];
    sprintf(scoreText, "Score: %d", score);

    if (*scoreTexture) {
        SDL_DestroyTexture(*scoreTexture);
    }
    *scoreTexture = renderText(renderer, scoreText, font, {255, 255, 255, 255});

    if (*scoreTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(*scoreTexture, NULL, NULL, &textWidth, &textHeight);
        SDL_Rect scoreRect = {SCREEN_WIDTH / 2 - textWidth / 2, 20, textWidth, textHeight};
        SDL_RenderCopy(renderer, *scoreTexture, NULL, &scoreRect);
    }

    SDL_RenderPresent(renderer);
}
