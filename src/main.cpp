#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include "ObjLoader.h"
#include "Vertex.h"
#include "Face.h"
#include "Uniform.h"
#include "RenderingUtils.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;


bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Render Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    return true;
}

void clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void render() {
    // 1. Vertex Shader
    // 2. Primitive Assembly
    // 3. Rasterization
    // 4. Fragment Shader
}

std::vector<glm::vec3> transformVertexArray(const std::vector<glm::vec3>& vertexArray, float scale, const int offset_x = SCREEN_WIDTH / 2, const int offset_y = SCREEN_HEIGHT / 2) {
    std::vector<glm::vec3> transformedArray;

    for (const glm::vec3& vertex : vertexArray) {
        // Apply scaling and translation to each vertex
        glm::vec3 transformedVertex = vertex * scale;
        transformedVertex.x += offset_x;
        transformedVertex.y += offset_y;
        transformedArray.push_back(transformedVertex);
    }

    return transformedArray;
}

void renderOBJ(std::vector<glm::vec3>& vertexArray) {
    for (int i = 0; i < vertexArray.size() - 1; i += 3) {
        glm::vec3 pointA = vertexArray[i];
        glm::vec3 pointB = vertexArray[i + 1];
        glm::vec3 pointC = vertexArray[i + 2];

        drawTriangle(renderer, pointA, pointB, pointC, Color(255, 255, 255));
        // drawPoint(renderer, pointA);
        // drawPoint(renderer, pointB);
        // drawPoint(renderer, pointC);
    }    
}

void printVertexArray(const std::vector<glm::vec3>& vertexArray) {
    std::cout << "Vertex Array Contents:" << std::endl;
    for (const glm::vec3& vertex : vertexArray) {
        std::cout << "Vertex: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
    }
}

int main() {
    // Initialize SDL
    if (!init()) { return 1; }
    
    // Read from .obj file and store the vertices/faces
    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    bool loadModel = false;
    if (loadModel) {
        if (!ObjLoader::LoadObj("../Lab3_Ship.obj", vertices, faces)) {
            SDL_Log("Failed to load .obj file.");
            return 1;
        } else {
            SDL_Log("Loaded .obj file successfully.");
        }
    }

    // std::vector<glm::vec3> vertexArray = setupVertexArray(vertices, faces);
    // std::vector<glm::vec3> transformedVertexArray = transformVertexArray(vertexArray, 30.0f);

    // printVertexArray(transformedVertexArray);

    std::vector<glm::vec3> vertices = {
        {300.0f, 200.0f, 0.0f},
        {400.0f, 400.0f, 0.0f},
        {500.0f, 200.0f, 0.0f}
    };

    Uniforms uniforms;

    glm::mat4 model = glm::mat4(1);
    glm::mat4 view = glm::mat4(1);
    glm::mat4 projection = glm::mat4(1);

    uniforms.model = model;
    uniforms.model = view;
    uniforms.model = projection;

    // Render loop
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear the buffer
        clear();

        // Call render() function
        render();
        // renderOBJ(transformedVertexArray);
        
        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
