#include "pipe.h"
#include "lib.h"

int pipeSpeed = 12; // Giá trị mặc định ban đầu

void initPipes(Pipe pipes[]) {
    srand(time(NULL));
    for (int i = 0; i < 2; i++) {
        pipes[i].x = SCREEN_WIDTH + i * (SCREEN_WIDTH / 2);
        pipes[i].height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
    }
}

void updatePipes(Pipe pipes[], int* score) {
    // Cập nhật tốc độ khi điểm đạt 15
    if (*score >= 15) {
        pipeSpeed = 15; // Tăng tốc độ lên 15
    }

    for (int i = 0; i < 2; i++) {
        pipes[i].x -= pipeSpeed;

        if (pipes[i].x + PIPE_WIDTH < 0) {
            pipes[i].x = SCREEN_WIDTH;
            pipes[i].height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
            (*score)++;
        }
    }
}



void renderPipes(SDL_Renderer* renderer, Pipe pipes[], SDL_Texture* pipeTexture) {
    for (int i = 0; i < 2; i++) {
        SDL_Rect upperPipeRect = { pipes[i].x, pipes[i].height - SCREEN_HEIGHT, PIPE_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, pipeTexture, NULL, &upperPipeRect);

        SDL_Rect lowerPipeRect = { pipes[i].x, pipes[i].height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopyEx(renderer, pipeTexture, NULL, &lowerPipeRect, 180, NULL, SDL_FLIP_NONE);
    }
}
