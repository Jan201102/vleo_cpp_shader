#pragma once
#include "glm/glm.hpp"
glm::vec3 v0(0.0f, -0.5f, 0.0f);  // Bottom left 
glm::vec3 v1(0.0f, 0.5f, 0.0f);  // Bottom right
glm::vec3 v2(0.0f, 0.0f, 0.5f);  // Top 
glm::vec3 v3(-0.5f, 0.0f, 0.0f);   // back

// Tetrahedron vertices without index buffer - 4 triangular faces
float vertices[] = {
    // Face 1: bottom (v0, v1, v3)
    v0.x, v0.y, v0.z,
    v1.x, v1.y, v1.z,
    v3.x, v3.y, v3.z,

    // Face 2: right back (v1, v2, v3)
    v1.x, v1.y, v1.z,
    v2.x, v2.y, v2.z,
    v3.x, v3.y, v3.z,

    // Face 3: left back (v2, v0, v3)
    v2.x, v2.y, v2.z,
    v0.x, v0.y, v0.z,
    v3.x, v3.y, v3.z,

    // Face 4: front (v0, v2, v1)
    v0.x, v0.y, v0.z,
    v2.x, v2.y, v2.z,
    v1.x, v1.y, v1.z
};
unsigned int triangleIDs[] = {
    // Face 1 bottom: Red
    1, 1, 1,

    // Face 2: Green
    2,2,2,

    // Face 3: Blue
    3,3,3,

    // Face 4: Yellow
    4,4,4
};
const unsigned int numTriangleIDs = sizeof(triangleIDs) / sizeof(unsigned int) * 3;