#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Platforms
{

	Mesh* CreatPlatform(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
}