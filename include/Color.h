#pragma once

#include <iostream>
#include <cstdint> // Include for Uint8 definition

using Uint8 = uint8_t; // Type alias for unsigned 8-bit integer

struct Color {
    Uint8 red;
    Uint8 green;
    Uint8 blue;

    Color(Uint8 r, Uint8 g, Uint8 b)
        : red((r > 255) ? 255 : ((r < 0) ? 0 : r)),
          green((g > 255) ? 255 : ((g < 0) ? 0 : g)),
          blue((b > 255) ? 255 : ((b < 0) ? 0 : b)) {}

    Color operator*(float scalar) const {
        Uint8 r = static_cast<Uint8>(red * scalar);
        Uint8 g = static_cast<Uint8>(green * scalar);
        Uint8 b = static_cast<Uint8>(blue * scalar);
        return Color(r, g, b);
    }

    bool operator==(const Color& other) const {
        return red == other.red && green == other.green && blue == other.blue;
    }

    bool operator!=(const Color& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Color& color) {
        os << "Color(" << static_cast<int>(color.red) << ", "
        << static_cast<int>(color.green) << ", "
        << static_cast<int>(color.blue) << ")";
        return os;
    }
};
