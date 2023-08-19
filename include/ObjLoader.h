#pragma once

#include <vector>
#include <string>
#include "Vertex.h"
#include "Face.h"

class ObjLoader {
public:
    static bool LoadObj(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<Face>& faces);
};
