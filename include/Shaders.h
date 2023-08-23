#pragma once

#include <glm/glm.hpp>
#include "Uniform.h"
#include "Color.h"
#include "Fragment.h"

// glm::vec3 vertexShader(const glm::vec3& vertex, const Uniforms& uniforms);
glm::vec3 vertexShader(const glm::vec3& vertex);
Color fragmentShader(const Fragment& fragment);