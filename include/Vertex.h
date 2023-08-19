#pragma once

#include <iostream>
#include <glm/glm.hpp> // Include the GLM library
#include "Color.h"      // Include the Color struct

struct Vertex {
    glm::vec3 position; // Using glm::vec3 for x, y, z coordinates
    Color color;        // Color member

    // Constructor with a default color
    Vertex(const glm::vec3& pos, const Color& col = Color(255, 255, 255))
        : position(pos), color(col) {}

    // Overload the << operator for printing a Vertex
    friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex) {
        os << "Vertex(" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z
        << "), Color(" << static_cast<int>(vertex.color.red) << ", "
        << static_cast<int>(vertex.color.green) << ", "
        << static_cast<int>(vertex.color.blue) << ")";
        return os;
    }
};