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
#define BIRD_SIZE 40
#define PIPE_WIDTH 65
#define PIPE_GAP 150
#define GRAVITY 1
#define JUMP_STRENGTH 10
#define PIPE_SPEED 12
#define FRAME_TIME 200 // 0.2 giây = 200 ms

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* birdTextures[4];
SDL_Texture* pipeTexture = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* scoreTexture = NULL;
Mix_Music* bgMusic = NULL;
Mix_Chunk* jumpSound = NULL;
TTF_Font* font = NULL;
SDL_Color white = {255, 255, 255, 255};

typedef struct {
    int x, y;
    int velocity;
    int frame;
    Uint32 lastFrameTime;
} Bird;

typedef struct {
    int x, height;
} Pipe;

Bird bird;
Pipe pipes[2];
int score = 0;
bool gameOver = false;

SDL_Texture* loadTexture(const char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        printf("Không thể tải ảnh %s! Lỗi: %s\n", path, IMG_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return texture;
}

SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Không thể tạo surface từ text! Lỗi: %s\n", TTF_GetError());
        return NULL;
    }
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

    birdTextures[0] = loadTexture("capy1.png");
    birdTextures[1] = loadTexture("capy2.png");
    birdTextures[2] = loadTexture("capy3.png");
    birdTextures[3] = loadTexture("capy4.png");
    pipeTexture = loadTexture("pipe.png");
    backgroundTexture = loadTexture("backg.png");

    font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        printf("Không thể tải font! Lỗi: %s\n", TTF_GetError());
    }

    bgMusic = Mix_LoadMUS("sound.mp3");
    if (bgMusic) Mix_PlayMusic(bgMusic, -1);

    jumpSound = Mix_LoadWAV("sound.wav");

    bird.x = SCREEN_WIDTH / 4;
    bird.y = SCREEN_HEIGHT / 2;
    bird.velocity = 0;
    bird.frame = 0;
    bird.lastFrameTime = SDL_GetTicks();

    srand(time(NULL));
    for (int i = 0; i < 2; i++) {
        pipes[i].x = SCREEN_WIDTH + i * (SCREEN_WIDTH / 2);
        pipes[i].height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
    }
}

void close() {
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

void handleInput(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_SPACE && !gameOver) {
            bird.velocity = -JUMP_STRENGTH;
            Mix_PlayChannel(-1, jumpSound, 0);
        }
    }
}

void update() {
    if (gameOver) return;

    bird.velocity += GRAVITY;
    bird.y += bird.velocity;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - bird.lastFrameTime >= FRAME_TIME) {
        bird.frame = (bird.frame + 1) % 4;
        bird.lastFrameTime = currentTime;
    }

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

        SDL_Rect birdHitbox = {bird.x + 5, bird.y + 5, BIRD_SIZE - 10, BIRD_SIZE - 10};
        SDL_Rect upperPipe = {pipes[i].x, 0, PIPE_WIDTH, pipes[i].height};
        SDL_Rect lowerPipe = {pipes[i].x, pipes[i].height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - (pipes[i].height + PIPE_GAP)};

        if (SDL_HasIntersection(&birdHitbox, &upperPipe) || SDL_HasIntersection(&birdHitbox, &lowerPipe)) {
            gameOver = true;
            return;
        }
    }
}

void render() {
    SDL_RenderClear(renderer);
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect);

    SDL_Rect birdRect = { bird.x, bird.y, BIRD_SIZE, BIRD_SIZE };
    SDL_RenderCopy(renderer, birdTextures[bird.frame], NULL, &birdRect);

    for (int i = 0; i < 2; i++) {
        SDL_Rect upperPipeRect = { pipes[i].x, pipes[i].height - SCREEN_HEIGHT, PIPE_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, pipeTexture, NULL, &upperPipeRect);

        SDL_Rect lowerPipeRect = { pipes[i].x, pipes[i].height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopyEx(renderer, pipeTexture, NULL, &lowerPipeRect, 180, NULL, SDL_FLIP_NONE);
    }

    // Hiển thị điểm số
    char scoreText[32];
    sprintf(scoreText, "Score: %d", score);

    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
    }
    scoreTexture = renderText(scoreText, font, white);

    if (scoreTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(scoreTexture, NULL, NULL, &textWidth, &textHeight);
        SDL_Rect scoreRect = {SCREEN_WIDTH / 2 - textWidth / 2, 20, textWidth, textHeight};
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    }

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
        SDL_Delay(50);
    }
    close();
    return 0;
}
