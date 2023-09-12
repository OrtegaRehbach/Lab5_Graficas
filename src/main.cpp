#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <array>
#include <sstream>
#include "ObjLoader.h"
#include "Vertex.h"
#include "Face.h"
#include "Uniform.h"
#include "RenderingUtils.h"
#include "Shaders.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

std::array<std::array<float, SCREEN_WIDTH>, SCREEN_HEIGHT> zbuffer;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Uniforms uniforms;


bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Render Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    return true;
}

void clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Fill the z-buffer
    for (auto &row : zbuffer) {
        std::fill(row.begin(), row.end(), 99999.0f);
    }
}

bool isInsideScreen(Fragment fragment) {
    return (
        fragment.x > 0 && fragment.x < SCREEN_WIDTH &&
        fragment.y > 0 && fragment.y < SCREEN_HEIGHT
    );
}

void point(Fragment fragment) {
    if (isInsideScreen(fragment) && fragment.z < zbuffer[fragment.y][fragment.x]) {
        // Draw the fragment on screen
        drawPoint(renderer, fragment.x, fragment.y, fragment.color);
        // Update the zbuffer for value for this position
        zbuffer[fragment.y][fragment.x] = fragment.z;
    }
}

void render(std::vector<glm::vec3> vertexBufferObject) {
    // 1. Vertex Shader
    std::vector<Vertex> transformedVertices;
    for (int i = 0; i < vertexBufferObject.size(); i += 2) {
        Vertex vertex = Vertex(vertexBufferObject[i], vertexBufferObject[i + 1]);
        // std::cout << vertex << std::endl;
        Vertex transformedVertex = vertexShader(vertex, uniforms);
        transformedVertices.push_back(transformedVertex);
        // std::cout << transformedVertex << std::endl;
    }
    // exit(1);
    // 2. Primitive Assembly
    std::vector<std::vector<Vertex>> triangles = primitiveAssembly(transformedVertices);

    // 3. Rasterization
    std::vector<Fragment> fragments;
    for (std::vector<Vertex> triangle : triangles) {
        // std::vector<Fragment> rasterizedTriangle = drawTriangle(triangle[0].position, triangle[1].position, triangle[2].position, triangle[0].color);
        std::vector<Fragment> rasterizedTriangle = getTriangleFragments(triangle[0], triangle[1], triangle[2], SCREEN_WIDTH, SCREEN_HEIGHT);

        fragments.insert(
            fragments.end(),
            rasterizedTriangle.begin(),
            rasterizedTriangle.end()
        );
    }
    
    // 4. Fragment Shader
    for (Fragment fragment : fragments) {
        // std::cout << "Before fragmentShader()" << std::endl;
        Fragment transformedFragment = fragmentShader(fragment);
        // std::cout << "After fragmentShader()" << std::endl;
        // drawPoint(renderer, transformedFragment.x, transformedFragment.y, transformedFragment.color);
        point(transformedFragment);
    }
}

int main() {
    // Initialize SDL
    if (!init()) { return 1; }
    
    // Read from .obj file and store the vertices/faces
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;

    bool loadModel = true;
    if (loadModel) {
        loadOBJ("../models/Lab3_Ship.obj", vertices, normals, faces);
    }

    Camera camera = {glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)};
    const float cameraMovementSpeed = 0.1f;
    const float horizontalRotationSpeed = 0.02f;

    std::vector<glm::vec3> VBO = setupVertexBufferObject(vertices, normals, faces);

    float rotation = 0.0f;
    Uint32 frameStart, frameTime;

    // Render loop
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            // Camera movement
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w) {
                    // "S" key was pressed
                    camera.MoveForward(cameraMovementSpeed);
                }
                else if (event.key.keysym.sym == SDLK_s) {
                    // "W" key was pressed
                    camera.MoveBackward(cameraMovementSpeed);
                }
                else if (event.key.keysym.sym == SDLK_a) {
                    // Rotate the camera left (orbit)
                    camera.Rotate(horizontalRotationSpeed);
                }
                else if (event.key.keysym.sym == SDLK_d) {
                    // Rotate the camera right (orbit)
                    camera.Rotate(-horizontalRotationSpeed);
                }
            }
        }
        

        // Clear the buffer
        clear();

        // Calculate matrixes for rendering
        uniforms.model = createModelMatrix(glm::vec3(0.8), glm::vec3(0, 0, 0), rotation += 0.02f);
        // uniforms.model = createModelMatrix(glm::vec3(0.8), glm::vec3(0, 0, 0), 0.0f);
        uniforms.view = createViewMatrix(camera);
        uniforms.projection = createProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);
        uniforms.viewport = createViewportMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);

        // Call render() function
        render(VBO);
        
        // Present the framebuffer to the screen
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;

        // Calculate frames per second and update window title
        if (frameTime > 0) {
            std::ostringstream titleStream;
            titleStream << "FPS: " << static_cast<int>(1000.0 / frameTime);  // Milliseconds to seconds
            SDL_SetWindowTitle(window, titleStream.str().c_str());
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
