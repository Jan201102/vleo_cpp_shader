#include "OpenGLUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "glm/glm.hpp"


MeshData LoadOBJ(const std::string& filepath)
{
    std::cout << "Loading OBJ file: " << filepath << std::endl;

    MeshData meshData;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cout << "ERROR: Could not open OBJ file: " << filepath << std::endl;
        return meshData;
    }

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_texCoords;

    // Temporäre Struktur für Faces
    struct Face {
        unsigned int v1, v2, v3;  // Vertex-Indizes
        unsigned int triangleID;   // Triangle-ID für dieses Face
    };

    std::vector<Face> faces;
    unsigned int currentTriangleID = 1;  // Start bei 1 (0 = Hintergrund)

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            // Vertex-Position parsen
            float x, y, z;
            iss >> x >> y >> z;
            temp_vertices.push_back(glm::vec3(x, y, z));

        }
        else if (prefix == "vn") {
            // Vertex-Normal parsen (optional, für später)
            float nx, ny, nz;
            iss >> nx >> ny >> nz;
            temp_normals.push_back(glm::vec3(nx, ny, nz));

        }
        else if (prefix == "vt") {
            // Texture-Koordinaten parsen (optional)
            float u, v;
            iss >> u >> v;
            temp_texCoords.push_back(glm::vec2(u, v));

        }
        else if (prefix == "f") {
            // Face parsen
            std::string vertex1, vertex2, vertex3;
            iss >> vertex1 >> vertex2 >> vertex3;

            // Face-Format: v/vt/vn oder v//vn oder nur v
            auto parseVertexIndex = [](const std::string& vertexStr) -> unsigned int {
                size_t slashPos = vertexStr.find('/');
                if (slashPos != std::string::npos) {
                    return std::stoi(vertexStr.substr(0, slashPos)) - 1;  // OBJ-Indizes sind 1-basiert
                }
                else {
                    return std::stoi(vertexStr) - 1;
                }
                };

            Face face;
            face.v1 = parseVertexIndex(vertex1);
            face.v2 = parseVertexIndex(vertex2);
            face.v3 = parseVertexIndex(vertex3);
            face.triangleID = currentTriangleID++;

            faces.push_back(face);

        }
        else if (prefix == "o" || prefix == "g") {
            // Objekt- oder Gruppen-Name (optional, für Material-/ID-Zuordnung)
            std::string name;
            iss >> name;
            std::cout << "Found object/group: " << name << std::endl;
        }
    }

    file.close();

    // Daten für OpenGL aufbereiten (ohne Index-Buffer)
    meshData.vertices.clear();
    meshData.triangleIDs.clear();

    for (const Face& face : faces) {
        // Vertex 1
        if (face.v1 < temp_vertices.size()) {
            const glm::vec3& v1 = temp_vertices[face.v1];
            meshData.vertices.push_back(v1.x);
            meshData.vertices.push_back(v1.y);
            meshData.vertices.push_back(v1.z);
            meshData.triangleIDs.push_back(face.triangleID);
        }

        // Vertex 2
        if (face.v2 < temp_vertices.size()) {
            const glm::vec3& v2 = temp_vertices[face.v2];
            meshData.vertices.push_back(v2.x);
            meshData.vertices.push_back(v2.y);
            meshData.vertices.push_back(v2.z);
            meshData.triangleIDs.push_back(face.triangleID);
        }

        // Vertex 3
        if (face.v3 < temp_vertices.size()) {
            const glm::vec3& v3 = temp_vertices[face.v3];
            meshData.vertices.push_back(v3.x);
            meshData.vertices.push_back(v3.y);
            meshData.vertices.push_back(v3.z);
            meshData.triangleIDs.push_back(face.triangleID);
        }
    }

    meshData.triangleCount = faces.size();
    meshData.vertexCount = meshData.vertices.size() / 3;

    std::cout << "OBJ loaded successfully!" << std::endl;
    std::cout << "  Vertices: " << temp_vertices.size() << std::endl;
    std::cout << "  Faces: " << faces.size() << std::endl;
    std::cout << "  Output vertices: " << meshData.vertexCount << std::endl;
    std::cout << "  Output triangles: " << meshData.triangleCount << std::endl;

    return meshData;
}