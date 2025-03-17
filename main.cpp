#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BIRD_SIZE 30
#define PIPE_WIDTH 60
#define PIPE_GAP 150
#define GRAVITY 1
#define JUMP_STRENGTH 12
#define PIPE_SPEED 9

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* birdTexture = NULL;
SDL_Texture* birdTextureDark = NULL;
SDL_Texture* pipeTexture = NULL;
SDL_Texture* backgroundTexture = NULL;
Mix_Music* bgMusic = NULL;
TTF_Font* font = NULL;
SDL_Color white = {255, 255, 255, 255};

typedef struct {
    int x, y;
    int velocity;
} Bird;

typedef struct {
    int x, height;
} Pipe;

Bird bird;
Pipe pipes[2];
int score = 0;
bool gameOver = false;

SDL_Texture* loadTexture(const char* path) {
    SDL_Texture* texture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        printf("Không thể tải ảnh %s! Lỗi: %s\n", path, IMG_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return texture;
}

SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    window = SDL_CreateWindow("Flappy Capy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    birdTexture = loadTexture("capy.png");
    birdTextureDark = loadTexture("capydark.png");
    pipeTexture = loadTexture("pipe.png");
    backgroundTexture = loadTexture("backg.png");

    font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        printf("Không thể tải font Arial! Lỗi: %s\n", TTF_GetError());
    }

    bgMusic = Mix_LoadMUS("sound.mp3");
    if (bgMusic) Mix_PlayMusic(bgMusic, -1);

    bird.x = SCREEN_WIDTH / 4;
    bird.y = SCREEN_HEIGHT / 2;
    bird.velocity = 0;

    srand(time(NULL));
    for (int i = 0; i < 2; i++) {
        pipes[i].x = SCREEN_WIDTH + i * (SCREEN_WIDTH / 2);
        pipes[i].height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
    }
}

void close() {
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(birdTextureDark);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(backgroundTexture);
    Mix_FreeMusic(bgMusic);
    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void handleInput(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_SPACE && !gameOver) {
            bird.velocity = -JUMP_STRENGTH;
        }
    }
}

void update() {
    if (gameOver) return;

    bird.velocity += GRAVITY;
    bird.y += bird.velocity;

    if (bird.y + BIRD_SIZE > SCREEN_HEIGHT) {
        gameOver = true;
        return;
    }

    for (int i = 0; i < 2; i++) {
        pipes[i].x -= PIPE_SPEED;

        if (pipes[i].x + PIPE_WIDTH < 0) {
            pipes[i].x = SCREEN_WIDTH;
            pipes[i].height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
            score++;
        }

        if (bird.x + BIRD_SIZE > pipes[i].x && bird.x < pipes[i].x + PIPE_WIDTH) {
            if (bird.y < pipes[i].height || bird.y + BIRD_SIZE > pipes[i].height + PIPE_GAP) {
                gameOver = true;
                return;
            }
        }
    }
}

void render() {
    SDL_RenderClear(renderer);

    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect);

    SDL_Texture* currentTexture = gameOver ? birdTextureDark : birdTexture;
    SDL_Rect birdRect = { bird.x, bird.y, BIRD_SIZE, BIRD_SIZE };
    SDL_RenderCopy(renderer, currentTexture, NULL, &birdRect);

    for (int i = 0; i < 2; i++) {
        SDL_Rect upperPipeRect = { pipes[i].x, pipes[i].height - SCREEN_HEIGHT, PIPE_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, pipeTexture, NULL, &upperPipeRect);

        SDL_Rect lowerPipeRect = { pipes[i].x, pipes[i].height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopyEx(renderer, pipeTexture, NULL, &lowerPipeRect, 180, NULL, SDL_FLIP_NONE);
    }

    char scoreText[10];
    sprintf(scoreText, "%d", score);
    SDL_Texture* scoreTexture = renderText(scoreText, font, white);
    SDL_Rect scoreRect = { SCREEN_WIDTH / 2 - 20, 20, 40, 30 };
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    SDL_DestroyTexture(scoreTexture);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    init();

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            handleInput(&event);
        }

        update();
        render();

        SDL_Delay(30);
    }

    close();
    return 0;
}
