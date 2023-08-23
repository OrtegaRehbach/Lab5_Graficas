#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <vector>
#include "Color.h"
#include "Vertex.h"
#include "Face.h"
#include "Fragment.h"

void drawPoint(SDL_Renderer* renderer, float x_position, float y_position, const Color& color = Color(255, 255, 255));
std::vector<Fragment> drawLine(const glm::vec3& start, const glm::vec3& end, const Color& color = Color(255, 255, 255));
std::vector<Fragment> drawTriangle(const glm::vec3& pointA, const glm::vec3& pointB, const glm::vec3& pointC, const Color& color = Color(255, 255, 255));
std::vector<Fragment> drawTriangle(const std::vector<glm::vec3>& triangle, const Color& color = Color(255, 255, 255));
std::vector<glm::vec3> setupVertexArray(const std::vector<Vertex>& vertices, const std::vector<Face>& faces);
std::vector<std::vector<glm::vec3>> primitiveAssembly (const std::vector<glm::vec3>& transformedVertices);
std::vector<Fragment> rasterize(const std::vector<std::vector<glm::vec3>>& assembledVertices);