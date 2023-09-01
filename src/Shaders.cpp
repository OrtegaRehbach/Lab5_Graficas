
#include "Shaders.h"

Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;


    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);
    
    // Return the transformed vertex as a vec3
    return Vertex{
        glm::vec3(screenVertex),
        vertex.color
    };
}

Vertex vertexShader(const Vertex& vertex) {
    return vertex;
}

Fragment fragmentShader(const Fragment& fragment) {
    // Example: Assign a constant color to each fragment
    glm::vec3 fragmentPosition(fragment.x, fragment.y, fragment.z);
    Color fragmentColor(128, 128, 128);

    Fragment shadedFragment = Fragment(fragmentPosition, fragmentColor);

    // You can modify this function to implement more complex shading
    // based on the fragment's attributes (e.g., depth, interpolated normals, texture coordinates, etc.)
    

    return shadedFragment;
}