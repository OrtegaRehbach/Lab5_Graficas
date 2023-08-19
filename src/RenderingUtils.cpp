#include "RenderingUtils.h"

void drawPoint(SDL_Renderer* renderer, const glm::vec3& position, const Color& color) {
    // Get Screen dimensions for coordinate adjustment
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    SDL_GetRendererOutputSize(renderer, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, static_cast<int>(position.x), SCREEN_HEIGHT - static_cast<int>(position.y));
}

void drawLine(SDL_Renderer* renderer, const glm::vec3& start, const glm::vec3& end, const Color& color) {
    int x0 = static_cast<int>(start.x);
    int y0 = static_cast<int>(start.y);
    int x1 = static_cast<int>(end.x);
    int y1 = static_cast<int>(end.y);

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        drawPoint(renderer, glm::vec3(x0, y0, 0.0f), color);

        if (x0 == x1 && y0 == y1)
            break;

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
}

void drawTriangle(SDL_Renderer* renderer, const glm::vec3& pointA, const glm::vec3& pointB, const glm::vec3& pointC, const Color& color) {
    drawLine(renderer, pointA, pointB, color);
    drawLine(renderer, pointB, pointC, color);
    drawLine(renderer, pointC, pointA, color);
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
