#pragma once

#include <iostream>
#include "Color.h"

struct Fragment
{
    float x;
    float y;
    Color color;

    // Constructor
    Fragment(float _x, float _y, Color _color = Color(255, 255, 255)) 
        : x(_x), y(_y), color(_color) {}

    // Overload the << operator for printing a Fragment
    friend std::ostream& operator<<(std::ostream& os, const Fragment& fragment) {
        os << "Fragment(" << fragment.x << ", " << fragment.y << ") " << fragment.color;
        return os;
    }
};
