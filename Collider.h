// Collider.h
#pragma once

#include <SDL.h>

class Collider {
public:
    float x, y, width, height;

    // �����������
    Collider(float x = 0.0f, float y = 0.0f, float width = 100.0f, float height = 100.0f)
        : x(x), y(y), width(width), height(height) {
    }

    // ������� ��� �������� ������������ ����� ����� ������������
    bool CheckCollision(const Collider& other) const {
        return (
            x < other.x + other.width &&
            x + width > other.x &&
            y < other.y + other.height &&
            y + height > other.y
            );
    }
};
