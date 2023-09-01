#pragma once

#include <iostream>
#include "Color.h"

struct Fragment
{
    float x;
    float y;
    float z;
    Color color;
    float intensity;

    // Constructor
    Fragment(float _x, float _y, float _z = 0.0f, Color _color = Color(255, 255, 255), float _intensity = 1.0f) 
        : x(_x), y(_y), z(_z), color(_color), intensity(_intensity) {}

    Fragment(glm::vec3 position, Color _color = Color(255, 255, 255), float _intensity = 1.0f) 
        : x(position.x), y(position.y), z(position.z), color(_color), intensity(_intensity) {}

    // Overload the << operator for printing a Fragment
    friend std::ostream& operator<<(std::ostream& os, const Fragment& fragment) {
        os << "Fragment(" << fragment.x << ", " << fragment.y << ", " << fragment.z << ") " << fragment.color << " Intensity: " << fragment.intensity;
        return os;
    }
};
