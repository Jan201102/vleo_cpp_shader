#include <iostream>
#include <thread>
#include <chrono>

//geometry data
#include "res/geometries/tetraeder.h"

#include "BinaryShader.h"

int main(void)
{
	float windX = 0.0f;
	float windY = 0.0f;
	float windZ = 1.0f;
	size_t const lenTriangleIDs = sizeof(triangleIDs) / sizeof(unsigned int);
	bool visibleIDs[numTriangles] = { false };
	size_t lenVertices = sizeof(vertices) / sizeof(float);
	while (true) {
		std::cout << "Rendering with wind direction: (" << windX << ", " << windY << ", " << windZ << ")" << std::endl;
		int status =  BinaryRenderer(vertices, lenVertices, triangleIDs, lenTriangleIDs, visibleIDs, numTriangles, windX, windY, windZ);

		std::cout << "Visible Triangle IDs:" << std::endl;
		for (unsigned int i = 0; i < numTriangles; ++i) {
			if (visibleIDs[i]) {
				std::cout << "Triangle ID " << i+1  << " is visible." << std::endl;
			}
		}
		std::cout << "----------------------------------------" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));

	}
}