#pragma once

#include <glm/glm.hpp>
#include "Uniform.h"
#include "Color.h"
#include "Fragment.h"
#include "Vertex.h"

Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms);
Vertex vertexShader(const Vertex& vertex);
Color fragmentShader(const Fragment& fragment);