#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <vector>
#include "Color.h"
#include "Vertex.h"
#include "Face.h"

void drawPoint(SDL_Renderer* renderer, const glm::vec3& position, const Color& color = Color(255, 255, 255));
void drawLine(SDL_Renderer* renderer, const glm::vec3& start, const glm::vec3& end, const Color& color = Color(255, 255, 255));
void drawTriangle(SDL_Renderer* renderer, const glm::vec3& pointA, const glm::vec3& pointB, const glm::vec3& pointC, const Color& color = Color(255, 255, 255));
std::vector<glm::vec3> setupVertexArray(const std::vector<Vertex>& vertices, const std::vector<Face>& faces);