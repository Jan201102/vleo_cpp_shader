#pragma once
#include <array>

// Define tetrahedron vertex positions using std::array instead of glm::vec3
static const std::array<float, 3> v0 = { 0.0f, -0.5f, 0.0f };  // Bottom left 
static const std::array<float, 3> v1 = { 0.0f, 0.5f, 0.0f };   // Bottom right
static const std::array<float, 3> v2 = { 0.0f, 0.0f, 0.5f };   // Top 
static const std::array<float, 3> v3 = { -0.5f, 0.0f, 0.0f };  // back

// Second tetrahedron vertices (shifted +0.5 along Z-axis)
static const std::array<float, 3> v0_2 = { 0.0f, -0.5f, 0.5f };  // Bottom left 
static const std::array<float, 3> v1_2 = { 0.0f, 0.5f, 0.5f };   // Bottom right
static const std::array<float, 3> v2_2 = { 0.0f, 0.0f, 1.0f };   // Top 
static const std::array<float, 3> v3_2 = { -0.5f, 0.0f, 0.5f };  // back

// Tetrahedron vertices without index buffer - 4 triangular faces
static float vertices[] = {
    // === First Tetrahedron ===
    // Face 1: bottom (v0, v1, v3)
    v0[0], v0[1], v0[2],
    v1[0], v1[1], v1[2],
    v3[0], v3[1], v3[2],

    // Face 2: right back (v1, v2, v3)
    v1[0], v1[1], v1[2],
    v2[0], v2[1], v2[2],
    v3[0], v3[1], v3[2],

    // Face 3: left back (v2, v0, v3)
    v2[0], v2[1], v2[2],
    v0[0], v0[1], v0[2],
    v3[0], v3[1], v3[2],

    // Face 4: front (v0, v2, v1)
    v0[0], v0[1], v0[2],
    v2[0], v2[1], v2[2],
    v1[0], v1[1], v1[2],

    // === Second Tetrahedron (shifted +0.5 along Z-axis) ===
    // Face 1: bottom (v0_2, v1_2, v3_2)
    v0_2[0], v0_2[1], v0_2[2],
    v1_2[0], v1_2[1], v1_2[2],
    v3_2[0], v3_2[1], v3_2[2],

    // Face 2: right back (v1_2, v2_2, v3_2)
    v1_2[0], v1_2[1], v1_2[2],
    v2_2[0], v2_2[1], v2_2[2],
    v3_2[0], v3_2[1], v3_2[2],

    // Face 3: left back (v2_2, v0_2, v3_2)
    v2_2[0], v2_2[1], v2_2[2],
    v0_2[0], v0_2[1], v0_2[2],
    v3_2[0], v3_2[1], v3_2[2],

    // Face 4: front (v0_2, v2_2, v1_2)
    v0_2[0], v0_2[1], v0_2[2],
    v2_2[0], v2_2[1], v2_2[2],
    v1_2[0], v1_2[1], v1_2[2]
};

static unsigned int triangleIDs[] = {
    // === First Tetrahedron ===
    // Face 1 bottom: Red
    1, 1, 1,

    // Face 2: Green
    2, 2, 2,

    // Face 3: Blue
    3, 3, 3,

    // Face 4: Yellow
    4, 4, 4,

    // === Second Tetrahedron ===
    // Face 1 bottom: Red
    5, 5, 5,

    // Face 2: Green
    6, 6, 6,

    // Face 3: Blue
    7, 7, 7,

    // Face 4: Yellow
    8, 8, 8
};

// Number of triangle ID entries
