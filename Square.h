// Square.h
#pragma once
#include <SDL.h>

class Square {
public:
    SDL_Color squareColor = { 255, 255, 255, 255 };
    int PositionX = 250.0f;
    int PositionY = 300.0f;

    float speed = 0.0f;

    float XScale = 100.0f;
    float YScale = 100.0f;

    float XPositionCollision = 0.0f;
    float YPositionCollision = 0.0f;

    float XScaleCollision = 100.0f;
    float YScaleCollision = 100.0f;
};