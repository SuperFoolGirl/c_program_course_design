#pragma once
#include "common.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define W 1920
#define H 1080
#undef main

// 声明GUI全局变量
// 只要包含了ui.h，就可以直接使用这两个变量
extern SDL_Window *window;
extern SDL_Renderer *renderer;

// 按钮结构体
typedef struct Button
{
    SDL_Rect rect;
    char *text;
    int (*callback)(void);
} Button;

int GUI_Init();

void GUI_Destroy();

void GUI_Update(Button *buttons, int numButtons);

void GUI_Draw(Button *buttons, int numButtons);

void drawText(const char *text, int x, int y);

void drawButton(Button *button);