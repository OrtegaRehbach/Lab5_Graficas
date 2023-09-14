
#include "Shaders.h"

Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;

    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);

    // Transform the normal
    glm::vec3 transformedNormal = glm::mat3(uniforms.model) * vertex.normal;
    transformedNormal = glm::normalize(transformedNormal);
    glm::vec3 normal = transformedNormal;

    // std::cout << "Transformed Normal(" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
   
    // Return the transformed vertex
    return Vertex{
        glm::vec3(screenVertex),
        transformedNormal
        // vertex.normal
    };
}

Fragment fragmentShader(const Fragment& fragment) {
    // Example: Assign a constant color to each fragment
    glm::vec3 fragmentPosition(fragment.x, fragment.y, fragment.z);
    Color fragmentColor = Color(220, 220, 220);

    float intensity = fragment.intensity;
    
    Fragment shadedFragment = Fragment(fragmentPosition, fragmentColor * intensity);

    // You can modify this function to implement more complex shading
    // based on the fragment's attributes (e.g., depth, interpolated normals, texture coordinates, etc.)
    

    return shadedFragment;
}