#include "ui.h"

// 定义全局变量
SDL_Window *window = NULL;     // 窗口
SDL_Renderer *renderer = NULL; // 渲染器
SDL_Texture *texture = NULL;   // 纹理

TTF_Font *font = NULL;             // 字体
SDL_Renderer *fontRenderer = NULL; // 字体渲染器
SDL_Texture *fontTexture = NULL;   // 字体纹理
SDL_Surface *fontSurface = NULL;   // 字体表面

SDL_Renderer *bgRenderer = NULL; // 背景渲染器
SDL_Texture *bgTexture = NULL;   // 背景纹理
SDL_Surface *bgSurface = NULL;   // 背景表面

// 检查鼠标点击是否在按钮区域内
// 这个函数不会直接调用，不用放在头文件里
int isButtonClicked(Button *button, int x, int y)
{
    return (x >= button->rect.x && x < button->rect.x + button->rect.w &&
            y >= button->rect.y && y < button->rect.y + button->rect.h);
}

int GUI_Init()
{
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "SDL初始化失败: %s", SDL_GetError());
        return -1;
    }

    // 初始化SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "SDL_image初始化失败: %s", IMG_GetError());
        return -1;
    }

    // 初始化SDL_ttf
    if (TTF_Init() < 0)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf初始化失败: %s", TTF_GetError());
        return -1;
    }

    // 创建窗口
    window = SDL_CreateWindow("快递管理系统", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "窗口创建失败: %s", SDL_GetError());
        return -1;
    }

    // 创建所有渲染器
    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "渲染器创建失败: %s", SDL_GetError());
        return -1;
    }

    // 创建字体渲染器
    fontRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (fontRenderer == NULL)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "字体渲染器创建失败: %s", SDL_GetError());
        return -1;
    }

    // 创建背景渲染器
    bgRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (bgRenderer == NULL)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "背景渲染器创建失败: %s", SDL_GetError());
        return -1;
    }

    // 加载字体
    font = TTF_OpenFont("C:/Windows/Fonts/simhei.ttf", 24);
    if (font == NULL)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "字体加载失败: %s", TTF_GetError());
        return -1;
    }

    return 0;
}

void backgroundInit()
{
    // 加载背景图片，返回一个SDL_Surface
    bgSurface = IMG_Load("../images/bg.png");
    if (bgSurface == NULL)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "背景图片加载失败: %s", IMG_GetError());
        return -1;
    }

    // 创建背景纹理
    bgTexture = SDL_CreateTextureFromSurface(bgRenderer, bgSurface); // 接受图片的surface，返回纹理
    if (bgTexture == NULL)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "背景纹理创建失败: %s", SDL_GetError());
        return -1;
    }

    // 显示文字

    // 复制背景纹理到背景渲染器
    SDL_RenderCopy(bgRenderer, bgTexture, NULL, NULL);

    // 更新屏幕显示
    SDL_RenderPresent(bgRenderer);

    // 事件监听与响应
    SDL_Event event;
    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_MOUSEBUTTONDOWN: // 鼠标按下事件
            {
                if (event.button.button == SDL_BUTTON_LEFT) // 左键按下
                {
                    // 进入下一页
                }
                break;
            }

            default:
                break;
            }
        }
    }
}

void GUI_Destroy()
{
    // 关闭字体
    TTF_CloseFont(font);

    // 销毁渲染器
    SDL_DestroyRenderer(renderer);
    SDL_DestroyRenderer(fontRenderer);
    SDL_DestroyRenderer(bgRenderer);

    // 销毁纹理
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(fontTexture);
    SDL_DestroyTexture(bgTexture);

    // 销毁表面
    SDL_FreeSurface(bgSurface);

    // 销毁窗口
    SDL_DestroyWindow(window);

    // 退出SDL_ttf
    TTF_Quit();

    // 退出SDL_image
    IMG_Quit();

    // 退出SDL
    SDL_Quit();
}

void GUI_Update(Button *buttons, int numButtons)
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
                exit(0);
                break;
            case SDL_MOUSEBUTTONDOWN: // 鼠标按下事件
            {
                if (event.button.button == SDL_BUTTON_LEFT) // 左键按下
                {
                    int x = event.button.x;              // 鼠标点击的x坐标
                    int y = event.button.y;              // 鼠标点击的y坐标
                    for (int i = 0; i < numButtons; i++) // 遍历此时屏幕上的所有按钮，直到找到被点击的按钮，调用其回调函数
                    {
                        if (isButtonClicked(&buttons[i], x, y))
                        {
                            buttons[i].callback();
                        }
                    }
                }
                break;
            }

            default:
                break;
            }
        }
    }
}

void GUI_Draw(Button *buttons, int numButtons)
{
    // 清屏
    SDL_RenderClear(renderer);

    // 设置渲染颜色
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    // 填充整个屏幕为指定颜色
    SDL_Rect screenRect = {0, 0, W, H};
    SDL_RenderFillRect(renderer, &screenRect);

    // 绘制欢迎信息
    drawText("欢迎使用快递管理系统！", 800, 100);

    // 绘制按钮
    for (int i = 0; i < numButtons; i++)
    {
        drawButton(&buttons[i]);
    }

    // 呈现渲染结果
    SDL_RenderPresent(renderer);
}

void drawText(const char *text, int x, int y) // x, y是展示文字的位置
{
    // 定义文本颜色--黑色
    SDL_Color color = {0, 0, 0, 255};

    // 渲染文本到表面
    fontSurface = TTF_RenderUTF8_Blended(font, text, color);
    if (fontSurface == NULL)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "文本渲染失败: %s", TTF_GetError());
        return -1;
    }

    // 创建文本纹理
    fontTexture = SDL_CreateTextureFromSurface(fontRenderer, fontSurface);
    if (fontTexture == NULL)
    {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "文本纹理创建失败: %s", SDL_GetError());
        return -1;
    }

    // 设置清除颜色
    SDL_SetRenderDrawColor(fontRenderer, 255, 255, 255, 255);
    // 清除渲染器目标缓冲区
    SDL_RenderClear(fontRenderer);

    // 复制文本纹理到文本渲染器
    SDL_RenderCopy(fontRenderer, fontTexture, NULL, &(SDL_Rect){x, y, fontSurface->w, fontSurface->h}); // 这里是强转

    // 更新屏幕显示
    SDL_RenderPresent(fontRenderer);

    // 
}

void drawButton(Button *button)
{
    SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
    SDL_RenderFillRect(renderer, &button->rect);
    drawText(button->text, button->rect.x + 150, button->rect.y + 10);
}