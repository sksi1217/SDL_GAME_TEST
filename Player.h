// Player.h
#pragma once
#include <SDL.h>

class Player {
public:
    SDL_Color playerColor = { 255, 255, 255, 255 };
    int PositionX = 0;
    int PositionY = 0;

    int speed = 5.0f;

    int XScale = 100;
    int YScale = 100.0f;

    int XPositionCollision = 0.0f;
    int YPositionCollision = 50;

    int XScaleCollision = 100.0f;
    int YScaleCollision = 10;
};