#pragma once
#include <string>
#include <vector>

struct MeshData
{
    std::vector<float> vertices;        // Vertex-Positionen (x, y, z, x, y, z, ...)
    std::vector<unsigned int> triangleIDs;  // Triangle-IDs für jedes Vertex
    unsigned int triangleCount;         // Anzahl der Dreiecke
    unsigned int vertexCount;          // Anzahl der Vertices
};

MeshData LoadOBJ(const std::string& filepath);