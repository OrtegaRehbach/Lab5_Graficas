#include "ObjLoader.h"
#include <fstream>
#include <sstream>

bool ObjLoader::LoadObj(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<Face>& faces) {
    std::ifstream objFile(filePath);
    if (!objFile.is_open()) {
        return false;
    }

    vertices.clear();
    faces.clear();

    std::string line;
    int lineNumber = 0;
    while (std::getline(objFile, line)) {
        ++lineNumber;
        std::istringstream iss(line);
        char type;
        iss >> type;

        if (type == 'v') {
            glm::vec3 pos;
            if (iss.str() != "") {
                iss >> pos.x >> pos.y >> pos.z;
                // std::cout << "Read vertex: " << "(" << pos.x << ", " << pos.y << ", " << pos.z  << ")" << std::endl;
                vertices.push_back(Vertex(pos));
            }
            
            
        } else if (type == 'f') {
            Face face;
            std::vector<int> vertexIndices; // Store vertex indices of the face

            while (iss) {
                int vertexIndex;
                iss >> vertexIndex;
                if (iss.fail()) {
                    break;
                }
                // std::cout << "Read index: " << vertexIndex << std::endl;
                vertexIndices.push_back(vertexIndex - 1); // Convert to 0-based indexing
                
                // Skip texture and normal indices
                iss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
                // iss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
            }

            // Build Face to store in 'faces' vector
            face.vertexIndices.push_back({vertexIndices[0], vertexIndices[1], vertexIndices[2]});
            faces.push_back(face);

            // std::cout << "Loaded indexes:\n";
            // for (const int vertexIndex : vertexIndices) {
            //     std::cout << "(" << vertexIndex << ")" << std::endl;
            // }
        }
    }

    // Print the loaded vertices and faces (optional)
    // std::cout << "Loaded vertices:\n";
    // for (const Vertex& vertex : vertices) {
    //     std::cout << vertex << "\n";
    // }    

    // std::cout << "\nLoaded faces:\n";
    // for (const Face& face : faces) {
    //     for (const auto& vertexIndices : face.vertexIndices) {
    //         std::cout << "(" << vertexIndices[0] << ", " << vertexIndices[1] << ", " << vertexIndices[2] << ") ";
    //     }
    //     std::cout << "\n";
    // }

    return true;
}
