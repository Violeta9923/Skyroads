#include "Platforms.h"
#include <Core/Engine.h>


Mesh* Platforms::CreatPlatform(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices{
		VertexFormat(corner + glm::vec3(0, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, 0, -length), color),
		VertexFormat(corner + glm::vec3(length, 0, -length), color),
		VertexFormat(corner + glm::vec3(0, length, -length), color),
		VertexFormat(corner + glm::vec3(length, length, -length), color),

	};

	std::vector<unsigned short> indices{
		0, 1, 2,
		1, 3, 2,
		2, 3, 7,
		2, 6, 7,
		0, 2, 4,
		2, 6, 4,
		0, 1, 4,
		1, 5, 4,
		4, 5, 7,
		4, 6, 7,
		1, 3, 7,
		1, 5, 7,
	};


	unsigned int VAO = 0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);


	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));


	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	glBindVertexArray(0);

	CheckOpenGLError();

	Mesh* platform = new Mesh(name);
	platform->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	platform->vertices = vertices;
	platform->indices = indices;
	
	return platform;


}