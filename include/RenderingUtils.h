#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp" // glm::lookAt()
#include <vector>
#include "Color.h"
#include "Vertex.h"
#include "Face.h"
#include "Fragment.h"
#include "Camera.h"

void drawPoint(SDL_Renderer* renderer, float x_position, float y_position, const Color& color = Color(255, 255, 255));
std::vector<Fragment> drawLine(const glm::vec3& start, const glm::vec3& end, const Color& color = Color(255, 255, 255));
std::vector<Fragment> drawTriangle(const glm::vec3& pointA, const glm::vec3& pointB, const glm::vec3& pointC, const Color& color = Color(255, 255, 255));
std::vector<Fragment> drawTriangle(const std::vector<glm::vec3>& triangle, const Color& color = Color(255, 255, 255));
std::vector<glm::vec3> setupVertexArray(const std::vector<Vertex>& vertices, const std::vector<Face>& faces);
std::vector<std::vector<Vertex>> primitiveAssembly (const std::vector<Vertex>& transformedVertices);
std::vector<Fragment> rasterize(const std::vector<std::vector<glm::vec3>>& assembledVertices);

glm::mat4 createModelMatrix(float scale);
glm::mat4 createViewMatrix();
glm::mat4 createProjectionMatrix(int SCREEN_WIDTH, int SCREEN_HEIGHT);
glm::mat4 createViewportMatrix(int SCREEN_WIDTH, int SCREEN_HEIGHT);