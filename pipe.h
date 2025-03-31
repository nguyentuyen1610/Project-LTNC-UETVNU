#ifndef PIPE_H
#define PIPE_H
#include "head.h"

#define PIPE_WIDTH 65
#define PIPE_GAP 150

extern int pipeSpeed;

typedef struct {
    int x, height;
} Pipe;

void initPipes(Pipe pipes[]);
void updatePipes(Pipe pipes[], int* score);
void renderPipes(SDL_Renderer* renderer, Pipe pipes[], SDL_Texture* pipeTexture);

#endif // PIPE_H
