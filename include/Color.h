#pragma once

#include <iostream>
#include <cstdint> // Include for Uint8 definition

using Uint8 = uint8_t; // Type alias for unsigned 8-bit integer

struct Color {
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    Uint8 alpha;

    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
        : red((r > 255) ? 255 : ((r < 0) ? 0 : r)),
          green((g > 255) ? 255 : ((g < 0) ? 0 : g)),
          blue((b > 255) ? 255 : ((b < 0) ? 0 : b)),
          alpha((a > 255) ? 255 : ((a < 0) ? 0 : a)) {}

    Color operator*(float scalar) const {
        Uint8 r = static_cast<Uint8>(red * scalar);
        Uint8 g = static_cast<Uint8>(green * scalar);
        Uint8 b = static_cast<Uint8>(blue * scalar);
        Uint8 a = static_cast<Uint8>(alpha * scalar);
        return Color(r, g, b, a);
    }

    Color operator*(Color other) const {
        Uint8 r = static_cast<Uint8>((red / 255)    * (other.red / 255)     * 255);
        Uint8 g = static_cast<Uint8>((green / 255)  * (other.green / 255)   * 255);
        Uint8 b = static_cast<Uint8>((blue / 255)   * (other.blue / 255)    * 255);
        Uint8 a = static_cast<Uint8>((alpha / 255)  * (other.alpha / 255)   * 255);
        return Color(r, g, b, a);
    }

    bool operator==(const Color& other) const {
        return red == other.red && green == other.green && blue == other.blue && alpha == other.alpha;
    }

    bool operator!=(const Color& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Color& color) {
        os << "Color(" 
        << static_cast<int>(color.red) << ", "
        << static_cast<int>(color.green) << ", "
        << static_cast<int>(color.blue) << ", "
        << static_cast<int>(color.alpha) << ")";
        return os;
    }
};
