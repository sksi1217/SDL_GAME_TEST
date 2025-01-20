#include <SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "Player.h"
#include "Square.h"
#include "Camera.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Player player;
Camera camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
std::vector<Square> squares;
std::vector<bool> keys(256, false);

// Структура для представления коллайдера
struct Collider {
    int x, y, w, h;
};

// Функция проверки столкновений
bool checkCollision(const Collider& a, const Collider& b) {
    return a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y;
}

void _playerMove(SDL_Event& event, bool& running, std::vector<Square>& squares) {
    Collider playerCollider = {
        player.PositionX + player.XPositionCollision,
        player.PositionY + player.YPositionCollision,
        player.XScaleCollision,
        player.YScaleCollision
    };


    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        else if (event.type == SDL_KEYDOWN) keys[event.key.keysym.sym] = true;
        else if (event.type == SDL_KEYUP) keys[event.key.keysym.sym] = false;
    }

    float moveX = 0.0f;
    float moveY = 0.0f;

    if (keys[SDLK_w]) moveY -= 1.0f;
    if (keys[SDLK_s]) moveY += 1.0f;
    if (keys[SDLK_a]) moveX -= 1.0f;
    if (keys[SDLK_d]) moveX += 1.0f;

    float length = std::sqrt(moveX * moveX + moveY * moveY);
    if (length > 0) {
        moveX /= length;
        moveY /= length;
    }

    float nextX = player.PositionX + moveX * player.speed;
    float nextY = player.PositionY + moveY * player.speed;

    // Проверка на столкновения
    Collider nextPlayerCollider = { 
        static_cast<int>(nextX) + player.XPositionCollision,
        static_cast<int>(nextY) + player.YPositionCollision,
        player.XScaleCollision, 
        player.YScaleCollision 
    };
    bool collision = false;
    for (auto& square : squares) {
        Collider squareCollider = { square.PositionX, square.PositionY, square.XScaleCollision, square.YScaleCollision };
        if (checkCollision(nextPlayerCollider, squareCollider)) {
            collision = true;
            break;
        }
    }


    if (!collision) {
        player.PositionX = nextX;
        player.PositionY = nextY;
    }
}
void UpdateCamera() {
    // Камера будет следовать за игроком
    camera.x = player.PositionX - camera.w / 2;
    camera.y = player.PositionY - camera.h / 2;
}
void Render(SDL_Renderer* renderer) {
    // Очистка экрана
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Отрисовка коллайдера игрока
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Желтый, полупрозрачный
    SDL_Rect playerColliderRect = {
            static_cast<int>(player.PositionX + player.XPositionCollision - camera.x),
            static_cast<int>(player.PositionY + player.YPositionCollision - camera.y),
            player.XScaleCollision,
            player.YScaleCollision
    };
    SDL_RenderFillRect(renderer, &playerColliderRect);

    for (const auto& square : squares) {
        SDL_Rect squareRect = { square.PositionX - static_cast<int>(camera.x), square.PositionY - static_cast<int>(camera.y), square.XScale, square.YScale };
        SDL_SetRenderDrawColor(renderer, square.squareColor.r, square.squareColor.g, square.squareColor.b, square.squareColor.a);
        SDL_RenderFillRect(renderer, &squareRect);
    }

    

    /* Отрисовка игрока
    SDL_SetRenderDrawColor(renderer, player.playerColor.r, player.playerColor.g, player.playerColor.b, player.playerColor.a);
    SDL_Rect playerRect = { static_cast<int>(player.PositionX - camera.x), static_cast<int>(player.PositionY - camera.y), player.XScale, player.YScale };
    SDL_RenderFillRect(renderer, &playerRect);

    */


    // Обновление экрана
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Initialization error SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("My window in SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Error create window!: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Error create renderer!: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    

    squares.push_back({
        {255, 0, 0, 255},
        100,
        0,
        0,
        100,
        100,
        100,
        100 
        }
    ); // Красный квадрат
    squares.push_back({ {0, 255, 0, 255}, 350, 150, 0, 100, 100, 100, 100 });     // Зеленый квадрат


    while (running) {
        std::cout << player.PositionX << std::endl;
        std::cout << player.PositionY << std::endl;

        _playerMove(event, running, squares);
        UpdateCamera();
        Render(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}