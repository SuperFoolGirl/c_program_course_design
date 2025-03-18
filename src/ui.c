#include "ui.h"

// 定义全局变量
SDL_Window *window = NULL;     // 窗口
SDL_Renderer *renderer = NULL; // 渲染器

int GUI_Init()
{
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "SDL初始化失败: %s", SDL_GetError());
        return -1;
    }

    // 创建窗口
    window = SDL_CreateWindow("快递管理系统", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "窗口创建失败: %s", SDL_GetError());
        return -1;
    }

    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "渲染器创建失败: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

void GUI_Destroy()
{
    // 销毁渲染器
    SDL_DestroyRenderer(renderer);

    // 销毁窗口
    SDL_DestroyWindow(window);

    // 退出SDL
    SDL_Quit();
}

void GUI_Update()
{
    // 事件监听与响应
    SDL_Event event;
    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return;

            default:
                break;
            }
        }
    }
}

void GUI_Draw()
{
    
}