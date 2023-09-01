#include "RenderingUtils.h"

glm::vec3 L(400, 400, 200);

void drawPoint(SDL_Renderer* renderer, float x_position, float y_position, const Color& color) {
    // Get Screen dimensions for coordinate adjustment
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    SDL_GetRendererOutputSize(renderer, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, static_cast<int>(x_position), SCREEN_HEIGHT - static_cast<int>(y_position));
}

std::vector<Fragment> drawLine(const glm::vec3& start, const glm::vec3& end, const Color& color) {
    int x0 = static_cast<int>(start.x);
    int y0 = static_cast<int>(start.y);
    int x1 = static_cast<int>(end.x);
    int y1 = static_cast<int>(end.y);

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    std::vector<Fragment> lineFragments;

    while (true) {
        lineFragments.push_back(Fragment(x0, y0, 0.0f, color));

        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    return lineFragments;
}

std::vector<Fragment> drawTriangle(const glm::vec3& pointA, const glm::vec3& pointB, const glm::vec3& pointC, const Color& color) {
    std::vector<Fragment> triangleFragments;
    
    std::vector<Fragment> lineAB = drawLine(pointA, pointB, color);
    std::vector<Fragment> lineBC = drawLine(pointB, pointC, color);
    std::vector<Fragment> lineCA = drawLine(pointC, pointA, color);

    triangleFragments.insert(
        triangleFragments.end(),
        lineAB.begin(),
        lineAB.end()
    );

    triangleFragments.insert(
        triangleFragments.end(),
        lineBC.begin(),
        lineBC.end()
    );

    triangleFragments.insert(
        triangleFragments.end(),
        lineCA.begin(),
        lineCA.end()
    );

    return triangleFragments;
}

std::vector<Fragment> drawTriangle(const std::vector<Vertex>& triangle, const Color& color) {
    return drawTriangle(triangle[0].position, triangle[1].position, triangle[2].position, color);
}

std::vector<Fragment> getTriangleFragments(Vertex a, Vertex b, Vertex c) {
    glm::vec3 A = a.position;
    glm::vec3 B = b.position;
    glm::vec3 C = c.position;

    std::vector<Fragment> triangleFragments;

    // Build bounding box
    int minX = static_cast<int>( std::min(std::min(A.x, B.x), C.x) );
    int minY = static_cast<int>( std::min(std::min(A.y, B.y), C.y) );
    int maxX = static_cast<int>( std::max(std::max(A.x, B.x), C.x) );
    int maxY = static_cast<int>( std::max(std::max(A.y, B.y), C.y) );

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            glm::vec3 P(x, y, 0);
            glm::vec3 barCoords = barycentricCoordinates(P, A, B, C);
            if (isInsideTriangle(barCoords)) {
                // Interpolate z value
                float interpolatedZ = a.position.z * barCoords.x + b.position.z * barCoords.y + c.position.z * barCoords.z;
                P.z = interpolatedZ;

                // Calculate normal
                glm::vec3 normal = calculateTrinagleNormal(A, B, C);
                
                // Calculate light direction
                glm::vec3 lightDirection = glm::normalize(L - P);

                // Calculate intensity
                float intensity = glm::dot(normal, lightDirection);
                
                triangleFragments.push_back(Fragment(P, Color(), intensity));
            }
        }
    }
    
    return triangleFragments;
}

std::vector<glm::vec3> setupVertexArray(const std::vector<Vertex>& vertices, const std::vector<Face>& faces) {
    std::vector<glm::vec3> vertexArray;

    // For each face
    for (const auto& face : faces) {
        // std::cout << "Face: " << face.vertexIndices.at(0).at(0) << ", " << face.vertexIndices.at(0).at(1) << ", " << face.vertexIndices.at(0).at(2) << std::endl;
        // For each vertex in the face
        for (const auto& vertexIndexArray : face.vertexIndices) {
            // For each vertex index array in the 'vertexIndices' vector
            for (const auto& vertexIndex : vertexIndexArray) {
                // Get the vertex position from the input vertices array using the index from the face
                // std::cout << "Read vertex index: " << vertexIndex << std::endl;
                glm::vec3 vertexPosition = vertices[vertexIndex].position;

                // Add the vertex position to the vertex array
                vertexArray.push_back(vertexPosition);
            }
        }
    }

    return vertexArray;
}

std::vector<std::vector<Vertex>> primitiveAssembly (const std::vector<Vertex>& transformedVertices) {
    // Group your vertices in groups of 3
    std::vector<std::vector<Vertex>> assembledVertices;

    for (int i = 0; i < transformedVertices.size() - 1; i += 3) {
        std::vector<Vertex> vertexGroup;
        vertexGroup.push_back(transformedVertices[i]);
        vertexGroup.push_back(transformedVertices[i + 1]);
        vertexGroup.push_back(transformedVertices[i + 2]);

        assembledVertices.push_back(vertexGroup);
    }
    
    // Return your assembled vertices
    return assembledVertices;
}

std::vector<Fragment> rasterize(const std::vector<std::vector<glm::vec3>>& assembledVertices) {
    std::vector<Fragment> fragments;

    for (const std::vector<glm::vec3>& triangleVertices : assembledVertices) {
        std::vector<Fragment> triangleFragments = drawTriangle(triangleVertices[0], triangleVertices[1], triangleVertices[2]);
        fragments.insert(fragments.end(), triangleFragments.begin(), triangleFragments.end());
    }

    return fragments;
}

glm::mat4 createModelMatrix(const glm::vec3& scaleVector, const glm::vec3& translationVector, const float rotationAngleRadians, const glm::vec3& rotationAxis) {
    glm::mat4 translation = glm::translate(glm::mat4(1), translationVector);
    glm::mat4 scale = glm::scale(glm::mat4(1), scaleVector);
    glm::mat4 rotation = glm::rotate(glm::mat4(1), rotationAngleRadians, rotationAxis);

    glm::mat4 modelMatrix = glm::mat4(1) * translation * scale * rotation;
    
    return modelMatrix;
}

glm::mat4 createViewMatrix() {
    // glm::mat4 viewMatrix = glm::lookAt(camera.targetPosition, camera.cameraPosition, camera.upVector);
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    return viewMatrix;
}

glm::mat4 createProjectionMatrix(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
  float fovInDegrees = 45.0f;
  float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
  float nearClip = 0.1f;
  float farClip = 100.0f;

  return glm::perspective(glm::radians(fovInDegrees), aspectRatio, nearClip, farClip);
}

glm::mat4 createViewportMatrix(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    glm::mat4 viewport = glm::mat4(1.0f);

    // Scale
    viewport = glm::scale(viewport, glm::vec3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.5f));

    // Translate
    viewport = glm::translate(viewport, glm::vec3(1.0f, 1.0f, 0.5f));

    return viewport;
}

glm::vec3 barycentricCoordinates(const glm::vec3& P, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C) {
    float w =   ((B.y - C.y)*(P.x - C.x) + (C.x - B.x)*(P.y - C.y)) / 
                ((B.y - C.y)*(A.x - C.x) + (C.x - B.x)*(A.y - C.y));

    float u =   ((C.y - A.y)*(P.x - C.x) + (A.x - C.x)*(P.y - C.y)) / 
                ((B.y - C.y)*(A.x - C.x) + (C.x - B.x)*(A.y - C.y));

    float v = 1.0f - u - w;

    return glm::vec3(w, u, v);
}

bool isInsideTriangle(const glm::vec3& barycentricCoordinates) {
    return (
        barycentricCoordinates.x <= 1 && barycentricCoordinates.x >= 0 &&
        barycentricCoordinates.y <= 1 && barycentricCoordinates.y >= 0 &&
        barycentricCoordinates.z <= 1 && barycentricCoordinates.z >= 0
    );
}

glm::vec3 findTriangleCentroid(Vertex a, Vertex b, Vertex c) {
    glm::vec3 A = a.position;
    glm::vec3 B = b.position;
    glm::vec3 C = c.position;

    glm::vec3 centroid = glm::vec3( (A.x + B.x + C.x) / 3,  (A.y + B.y + C.y) / 3, (A.z + B.z + C.z) / 3);

    return centroid;
}

glm::vec3 calculateTrinagleNormal(glm::vec3 A,glm::vec3 B, glm::vec3 C) {
    glm::vec3 edge1 = B - A;
    glm::vec3 edge2 = C - A;
    glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
    // glm::vec3 normal = glm::cross(edge1, edge2);

    return normal;
}

