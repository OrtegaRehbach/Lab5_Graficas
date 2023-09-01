#pragma once

#include <iostream>
#include "Color.h"

struct Fragment
{
    float x;
    float y;
    float z;
    Color color;

    // Constructor
    Fragment(float _x, float _y, float _z = 0.0f, Color _color = Color(255, 255, 255)) 
        : x(_x), y(_y), z(_z), color(_color) {}

    Fragment(glm::vec3 position, Color _color = Color(255, 255, 255)) 
        : x(position.x), y(position.y), z(position.z), color(_color) {}

    // Overload the << operator for printing a Fragment
    friend std::ostream& operator<<(std::ostream& os, const Fragment& fragment) {
        os << "Fragment(" << fragment.x << ", " << fragment.y << ", " << fragment.z << ") " << fragment.color;
        return os;
    }
};
