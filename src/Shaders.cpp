
#include "Shaders.h"

glm::vec3 vertexShader(const glm::vec3& vertex) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    // Return the transformed vertex as a vec3
    return vertex;
}

Color fragmentShader(const Fragment& fragment) {
    // Example: Assign a constant color to each fragment
    Color fragColor(255, 0, 0, 255); // Red color with full opacity

    // You can modify this function to implement more complex shading
    // based on the fragment's attributes (e.g., depth, interpolated normals, texture coordinates, etc.)

    return fragColor;
}