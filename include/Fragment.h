#pragma once

#include <iostream>

struct Fragment
{
    float x;
    float y;

    // Overload the << operator for printing a Fragment
    friend std::ostream& operator<<(std::ostream& os, const Fragment& fragment) {
        os << "Fragment(" << fragment.x << ", " << fragment.y << ")";
        return os;
    }
};
