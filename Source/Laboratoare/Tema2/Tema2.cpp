#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>

#include <Core/Engine.h>
#include "Platforms.h"
#include "Square.h"

using namespace std;

struct Positions {
	float x;
	float y;
	float z;
};

struct structForPlatforms {
	int type;
	struct Positions pos;
	int color;
	bool readyToLaunch;
	float scaleZ;

} platformsLeft[15], platformsMid[15], platformsRight[15];

float randMaxDistance(float min, float max) {
	float number;
	bool ret = false;
	number = rand() % 2;
	while (ret == false) {
		if (number >= min && number <= max) {
			ret = true;
			break;
		} 
		if (number < min) {
			number = number + 0.1;
		}
		if (number > max) {
			number = number - 0.1;
		}
	}
	return number;
}

void Tema2::check_the_color(int color) {
	if (color == 1) {
		end_red = true;
		col = 1;
	}
	else if (color == 2) {
		fuel -= 20;
		col = 1;
	}
	else if (color == 3) {
		pressed_w = 5000;
		col = 1;
	}
	else if (color == 4) {
		if (fuel < 100) {
			fuel += 20;
		}
		
		col = 1;
	}
}

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

inline glm::mat3 Translate(float translateX, float translateY)
{
	return glm::mat3(1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		translateX, translateY, 1.f);
}

inline glm::mat3 Scale(float scaleX, float scaleY)
{
	return glm::mat3(scaleX, 0.f, 0.f,
		0.f, scaleY, 0.f,
		0.f, 0.f, 1.f);
}

void Tema2::Init()
{
	glm::vec3 corner = glm::vec3(0, 0, 0);
	//viteza initiala
	speed = 0.5;
	// coordonata de la care sunt generate initial platformele
	//platMax = 30;
	initialCoord = -30;
	outCoord = 5;
	// platforme
	scaleX = 1;
	scaleY = 0.1;
	scaleZ = 0;
	initialLeft = 0;
	initialRight = 0;
	initialMid = 0;
	// stabileste daca deformam sfera
	col = 0;
	//sfera
	sphereX = 0;
	sphereY = 0.34;
	sphereZ = 0.5;
	// jump stabileste daca sfera se afla pe o platforma
	jump = false;
	// terminare joc
	end_red = false;
	end_fall = false;
	// combustibil
	fuel = 100;
	// modul third sau first
	mode = 0;
	// pentru a bloca cateva secunde la viteza mare
	pressed_w = 0;
	sphere_scale_x = 0.5;
	sphere_scale_y = 0.5;
	sphere_scale_z = 0.5;
	// jocul nu se opreste imediat, ci dupa ce sfera cade
	contor = 70;

	camera = new Tema::Camera();
	camera->Set(glm::vec3(0, 1, 3.f), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

	for (int i = 0; i < 15; i++) {
		// setare platforme initaiale
		platformsLeft[i].type = -1;
		platformsLeft[i].pos.x = -2;
		platformsLeft[i].pos.y = 0;
		platformsMid[i].type = 0;
		platformsMid[i].pos.x = -0.5;
		platformsMid[i].pos.y = 0;
		platformsRight[i].type = 1;
		platformsRight[i].pos.x = 1;
		platformsRight[i].pos.y = 0;
		if (i == 0) {
			initialLeft = 0;
			initialRight = 0;
			initialMid = 0;
			platformsLeft[i].pos.z = initialLeft;
			platformsLeft[i].color = 0;
			platformsLeft[i].readyToLaunch = true;
			scaleZ = rand() % 4 + 1;
			platformsLeft[i].scaleZ = scaleZ;
			initialLeft = initialLeft - scaleZ;

			platformsMid[i].pos.z = initialMid;
			platformsMid[i].color = 0;
			platformsMid[i].readyToLaunch = true;
			scaleZ = rand() % 4 + 1;
			platformsMid[i].scaleZ = scaleZ;
			initialMid = initialMid - scaleZ;

			platformsRight[i].pos.z = initialRight;
			platformsRight[i].color = 0;
			platformsRight[i].readyToLaunch = true;
			scaleZ = rand() % 4 + 1;
			platformsRight[i].scaleZ = scaleZ;
			initialRight = initialRight - scaleZ;
		}
		else {
			initialLeft = initialLeft - randMaxDistance(0.3, 1.3);
			platformsLeft[i].pos.z = initialLeft;
			if (i % 2 != 0)
				platformsLeft[i].color = rand() % 5;
			else
				platformsLeft[i].color = 0;
			platformsLeft[i].readyToLaunch = true;
			scaleZ = rand() % 4 + 1;
			platformsLeft[i].scaleZ = scaleZ;
			initialLeft = initialLeft - scaleZ;

			initialMid = initialMid - randMaxDistance(0.3, 1.3);
			platformsMid[i].pos.z = initialMid;
			if (i % 2 != 0)
				platformsMid[i].color = rand() % 5;
			else
				platformsMid[i].color = 0;
			platformsMid[i].readyToLaunch = true;
			scaleZ = rand() % 4 + 1;
			platformsMid[i].scaleZ = scaleZ;
			initialMid = initialMid - scaleZ;

			initialRight = initialRight - randMaxDistance(0.3, 1.3);
			platformsRight[i].pos.z = initialRight;
			if (i % 2 != 0)
				platformsRight[i].color = rand() % 5;
			else
				platformsRight[i].color = 0;
			platformsRight[i].readyToLaunch = true;
			scaleZ = rand() % 4 + 1;
			platformsRight[i].scaleZ = scaleZ;
			initialRight = initialRight - scaleZ;
		}
	}
	
	//personaj
	{
		Mesh* sphere = new Mesh("sphere");
		sphere->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[sphere->GetMeshID()] = sphere;
	}
	

	//combustibil
	{
		Mesh* square = Square::CreateSquare("square", corner, 1, glm::vec3(1, 1, 1));
		meshes[square->GetMeshID()] = square;
	}

	{
		Mesh* fuel_square = Square::CreateSquare("fuel_square", corner, 1, glm::vec3(0.5f, 1.0f, 1.0f));
		meshes[fuel_square->GetMeshID()] = fuel_square;
	}
	
	

	// crearea platformelor
	Mesh* platformBlue = Platforms::CreatPlatform("platformBlue", corner, 1, glm::vec3(0.0, 0.0, 1.0));
	meshes[platformBlue->GetMeshID()] = platformBlue;
	Mesh* platformRed = Platforms::CreatPlatform("platformRed", corner, 1, glm::vec3(1.0, 0.0, 0.0));
	meshes[platformRed->GetMeshID()] = platformRed;
	Mesh* platformYellow = Platforms::CreatPlatform("platformYellow", corner, 1, glm::vec3(1.0, 1.0, 0.0));
	meshes[platformYellow->GetMeshID()] = platformYellow;
	Mesh* platformOrange = Platforms::CreatPlatform("platformOrange", corner, 1, glm::vec3(1.0, 0.5, 0.0));
	meshes[platformOrange->GetMeshID()] = platformOrange;
	Mesh* platformGreen = Platforms::CreatPlatform("platformGreen", corner, 1, glm::vec3(0.0, 1.0, 0.0));
	meshes[platformGreen->GetMeshID()] = platformGreen;
	Mesh* platformPurple = Platforms::CreatPlatform("platformPurple", corner, 1, glm::vec3(1.0, 0.0, 1.0));
	meshes[platformPurple->GetMeshID()] = platformPurple;

	
	{
		Shader* shaderPlatform = new Shader("ShaderPlatform");
		shaderPlatform->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shaderPlatform->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shaderPlatform->CreateAndLink();
		shaders[shaderPlatform->GetName()] = shaderPlatform;
	}

	{
		Shader* shaderSphere = new Shader("ShaderSphere");
		shaderSphere->AddShader("Source/Laboratoare/Tema2/SShader/SVertexShader.glsl", GL_VERTEX_SHADER);
		shaderSphere->AddShader("Source/Laboratoare/Tema2/SShader/SFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shaderSphere->CreateAndLink();
		shaders[shaderSphere->GetName()] = shaderSphere;
	}

}

void Tema2::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	int color = 0;

	fuel -= 0.06;

	if (mode % 2 != 0) {
		// pentru first mode setam camera la fiecare cadru
		camera->Set(glm::vec3(sphereX, sphereY, 0),  glm::vec3(sphereX, sphereY - 0.3, -1.7), glm::vec3(0, 1, 0));
	}
	
	if (end_red == true || fuel <= 0) {
		//jocul se inchide imediat
		exit(0);
	}
	else if (end_fall == true) {
		glm::mat4 modelMatrix = glm::mat4(1);
		if (contor > 0) {
			// asteptam cateva secunde
			// timp in care sfera se micsoreaza si cade
			sphere_scale_x -= 0.01;
			sphere_scale_y -= 0.01;
			sphere_scale_z -= 0.01;
			contor--;
			if (col == 1) {
				sphereY -= 0.1;
				modelMatrix = glm::translate(modelMatrix, glm::vec3(sphereX, sphereY, sphereZ));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(sphere_scale_x / 10, sphere_scale_y / 10, sphere_scale_z / 10));
			}
			else {
				sphereY -= 0.1;
				modelMatrix = glm::translate(modelMatrix, glm::vec3(sphereX, sphereY, sphereZ));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(sphere_scale_x, sphere_scale_y, sphere_scale_z));
			}
			if (mode % 2 == 0) {
				RenderSimpleSphere(meshes["sphere"], shaders["ShaderSphere"], modelMatrix);
			}
				
		}
		else {
			// dupa cateva secunde inchide jocul
			exit(0);
		}

			
	}
	else {
		if (jump == true) {
			// la fiecare saritura sfera se reface
			col = 0;
		}

		// desenare sfera
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(sphereX, sphereY, sphereZ));
		if (end_fall == false) {
			if (col == 1) {
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.05, 0.05));
			}
			else {
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
			}
		}

		if (mode % 2 == 0) {
			RenderSimpleSphere(meshes["sphere"], shaders["ShaderSphere"], modelMatrix);
		}


		// pentru combustbil
		glm::mat3 mmMatrix = glm::mat3(1);
		mmMatrix *= Translate(-2.3, 1.9);
		mmMatrix *= Scale(fuel / 50, 0.3);
		RenderMesh2D(meshes["fuel_square"], shaders["VertexColor"], mmMatrix);

		glm::mat3 mMatrix = glm::mat3(1);
		mMatrix *= Translate(-2.3, 1.9);
		mMatrix *= Scale(2.2, 0.3);
		RenderMesh2D(meshes["square"], shaders["VertexColor"], mMatrix);

		for (int i = 0; i < 15; i++) {
			if (platformsLeft[i].readyToLaunch == true) {
				//desenare platforme pe coloana stanga in fc de culoare
				//platformsLeft[i].pos.z = platformsLeft[i].pos.z + deltaTimeSeconds + up_speed;
				if (pressed_w == 0) {
					platformsLeft[i].pos.z += speed * deltaTimeSeconds;
				}
				else {
					platformsLeft[i].pos.z += 4 * deltaTimeSeconds;
					pressed_w--;
				}

				if (platformsLeft[i].color == 0) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsLeft[i].pos.x, platformsLeft[i].pos.y, platformsLeft[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsLeft[i].scaleZ));
					RenderSimpleMesh(meshes["platformBlue"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsLeft[i].color == 1) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsLeft[i].pos.x, platformsLeft[i].pos.y, platformsLeft[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsLeft[i].scaleZ));
					RenderSimpleMesh(meshes["platformRed"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsLeft[i].color == 2) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsLeft[i].pos.x, platformsLeft[i].pos.y, platformsLeft[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsLeft[i].scaleZ));
					RenderSimpleMesh(meshes["platformYellow"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsLeft[i].color == 3) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsLeft[i].pos.x, platformsLeft[i].pos.y, platformsLeft[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsLeft[i].scaleZ));
					RenderSimpleMesh(meshes["platformOrange"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsLeft[i].color == 4) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsLeft[i].pos.x, platformsLeft[i].pos.y, platformsLeft[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsLeft[i].scaleZ));
					RenderSimpleMesh(meshes["platformGreen"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsLeft[i].color == 5) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsLeft[i].pos.x, platformsLeft[i].pos.y, platformsLeft[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsLeft[i].scaleZ));
					RenderSimpleMesh(meshes["platformPurple"], shaders["ShaderPlatform"], modelMatrix);
				}
			}

			if (platformsMid[i].readyToLaunch == true) {
				//platformsMid[i].pos.z = platformsMid[i].pos.z + up_speed + deltaTimeSeconds;
				if (pressed_w == 0) {
					platformsMid[i].pos.z += speed * deltaTimeSeconds;
				}
				else {
					platformsMid[i].pos.z += 4 * deltaTimeSeconds;
				}

				//desenare platforme pe coloana mijloc in fc de culoare
				if (platformsMid[i].color == 0) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsMid[i].pos.x, platformsMid[i].pos.y, platformsMid[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsMid[i].scaleZ));
					RenderSimpleMesh(meshes["platformBlue"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsMid[i].color == 1) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsMid[i].pos.x, platformsMid[i].pos.y, platformsMid[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsMid[i].scaleZ));
					RenderSimpleMesh(meshes["platformRed"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsMid[i].color == 2) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsMid[i].pos.x, platformsMid[i].pos.y, platformsMid[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsMid[i].scaleZ));
					RenderSimpleMesh(meshes["platformYellow"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsMid[i].color == 3) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsMid[i].pos.x, platformsMid[i].pos.y, platformsMid[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsMid[i].scaleZ));
					RenderSimpleMesh(meshes["platformOrange"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsMid[i].color == 4) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsMid[i].pos.x, platformsMid[i].pos.y, platformsMid[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsMid[i].scaleZ));
					RenderSimpleMesh(meshes["platformGreen"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsMid[i].color == 5) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsMid[i].pos.x, platformsMid[i].pos.y, platformsMid[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsMid[i].scaleZ));
					RenderSimpleMesh(meshes["platformPurple"], shaders["ShaderPlatform"], modelMatrix);
				}
			}

			if (platformsRight[i].readyToLaunch == true) {
				//platformsRight[i].pos.z = platformsRight[i].pos.z + up_speed + deltaTimeSeconds;
				if (pressed_w == 0) {
					platformsRight[i].pos.z += speed * deltaTimeSeconds;
				}
				else {
					platformsRight[i].pos.z += 4 * deltaTimeSeconds;
				}

				//desenare platforme pe coloana dreapta in fc de culoare
				if (platformsRight[i].color == 0) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsRight[i].pos.x, platformsRight[i].pos.y, platformsRight[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsRight[i].scaleZ));
					RenderSimpleMesh(meshes["platformBlue"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsRight[i].color == 1) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsRight[i].pos.x, platformsRight[i].pos.y, platformsRight[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsRight[i].scaleZ));
					RenderSimpleMesh(meshes["platformRed"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsRight[i].color == 2) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsRight[i].pos.x, platformsRight[i].pos.y, platformsRight[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsRight[i].scaleZ));
					RenderSimpleMesh(meshes["platformYellow"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsRight[i].color == 3) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsRight[i].pos.x, platformsRight[i].pos.y, platformsRight[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsRight[i].scaleZ));
					RenderSimpleMesh(meshes["platformOrange"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsRight[i].color == 4) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsRight[i].pos.x, platformsRight[i].pos.y, platformsRight[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsRight[i].scaleZ));
					RenderSimpleMesh(meshes["platformGreen"], shaders["ShaderPlatform"], modelMatrix);
				}
				if (platformsRight[i].color == 5) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(platformsRight[i].pos.x, platformsRight[i].pos.y, platformsRight[i].pos.z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, platformsRight[i].scaleZ));
					RenderSimpleMesh(meshes["platformPurple"], shaders["ShaderPlatform"], modelMatrix);
				}
			}

			// setam pozitia pe z la pozitia initiala daca a trecut de observator
			if (platformsLeft[i].pos.z - platformsLeft[i].scaleZ > outCoord) {
				scaleZ = 0;
				platformsLeft[i].readyToLaunch = false;
				platformsLeft[i].pos.z = initialCoord;
				platformsLeft[i].scaleZ = 0;
			}

			if (platformsMid[i].pos.z - platformsMid[i].scaleZ > outCoord) {
				scaleZ = 0;
				platformsMid[i].readyToLaunch = false;
				platformsMid[i].pos.z = initialCoord;
				platformsMid[i].scaleZ = scaleZ;

			}

			if (platformsRight[i].pos.z - platformsRight[i].scaleZ > outCoord) {
				scaleZ = 0;
				platformsRight[i].readyToLaunch = false;
				platformsRight[i].pos.z = initialCoord;
				platformsRight[i].scaleZ = scaleZ;
			}

			// setam culoarea pentru platformele nou afisate
			if (platformsLeft[i].pos.z == initialCoord) {
				if (i != 0) {
					if (platformsLeft[i - 1].pos.z < platformsLeft[i].pos.z + platformsLeft[i - 1].scaleZ + randMaxDistance(0.3, 1)) {
						platformsLeft[i].readyToLaunch = false;
						platformsLeft[i].scaleZ = 0;
					}
					else {
						if (platformsLeft[i - 1].readyToLaunch == true) {
							platformsLeft[i].readyToLaunch = true;

							if (i % 2 != 0)
								color = rand() % 5;
							else
								color = 0;
							platformsLeft[i].color = color;
							scaleZ = rand() % 5 + 1;
							platformsLeft[i].scaleZ = scaleZ;
						}

					}
				}
				else {
					int randomDist = randMaxDistance(0.3, 1);
					if (platformsLeft[14].pos.z < platformsLeft[i].pos.z + platformsLeft[14].scaleZ + randomDist) {
						platformsLeft[i].readyToLaunch = false;
						platformsLeft[i].scaleZ = 0;
					}
					else {
						if (platformsLeft[14].readyToLaunch == true) {
							platformsLeft[i].readyToLaunch = true;
							if (i % 2 != 0)
								color = rand() % 5;
							else
								color = 0;
							platformsLeft[i].color = color;
							scaleZ = rand() % 5 + 1;
							platformsLeft[i].scaleZ = scaleZ;
						}
					}
				}
			}
			if (platformsMid[i].pos.z == initialCoord) {
				if (i > 0) {
					if (platformsMid[i - 1].pos.z < platformsMid[i].pos.z + platformsMid[i - 1].scaleZ + randMaxDistance(0.3, 1)) {
						platformsMid[i].readyToLaunch = false;
						platformsMid[i].scaleZ = 0;
					}
					else {
						if (platformsMid[i - 1].readyToLaunch == true) {
							platformsMid[i].readyToLaunch = true;
							if (i % 2 != 0)
								color = rand() % 5;
							else
								color = 0;
							platformsMid[i].color = color;
							scaleZ = rand() % 5 + 1;
							platformsMid[i].scaleZ = scaleZ;
						}

					}

				}
				else {
					if (platformsMid[14].pos.z < platformsMid[i].pos.z + platformsMid[14].scaleZ + randMaxDistance(0.3, 1)) {
						platformsMid[i].readyToLaunch = false;
						platformsMid[i].scaleZ = 0;
					}
					else {
						if (platformsMid[14].readyToLaunch == true) {
							platformsMid[i].readyToLaunch = true;
							if (i % 2 != 0)
								color = rand() % 5;
							else
								color = 0;
							platformsMid[i].color = color;
							scaleZ = rand() % 5 + 1;
							platformsMid[i].scaleZ = scaleZ;
						}

					}
				}

			}
			if (platformsRight[i].pos.z == initialCoord) {
				if (i > 0) {
					if (platformsRight[i - 1].pos.z < platformsRight[i].pos.z + platformsRight[i - 1].scaleZ + randMaxDistance(0.3, 1.3)) {
						platformsRight[i].readyToLaunch = false;
						platformsRight[i].scaleZ = 0;
					}
					else {
						if (platformsRight[i - 1].readyToLaunch == true) {
							platformsRight[i].readyToLaunch = true;
							if (i % 2 != 0)
								color = rand() % 5;
							else
								color = 0;
							platformsRight[i].color = color;
							scaleZ = rand() % 5 + 1;
							platformsRight[i].scaleZ = scaleZ;
						}

					}

				}
				else {
					if (platformsRight[14].pos.z < platformsRight[i].pos.z + platformsRight[14].scaleZ + randMaxDistance(0.3, 1.3)) {
						platformsRight[i].readyToLaunch = false;
						platformsRight[i].scaleZ = 0;
					}
					else {
						if (platformsRight[14].readyToLaunch == true) {
							platformsRight[i].readyToLaunch = true;
							if (i % 2 != 0)
								color = rand() % 5;
							else
								color = 0;
							platformsRight[i].color = color;
							scaleZ = rand() % 5 + 1;
							platformsRight[i].scaleZ = scaleZ;
						}

					}
				}

			}

			if (jump == false) {
				// pentru a verifica coloziunile pe fiecare coloana 
				// pentru a modifica culoarea in cazul in care sfera se afla pe platforma
				// caz separat pentru ultima platforma din vector
				if (sphereX < -0.5) {
					if (sphereX - 0.14 > -1 || sphereX < -2) {
						end_fall = true;
					}
					else {
						if (sphereZ <= platformsLeft[i].pos.z) {
							if (i != 14) {
								if (sphereZ < platformsLeft[i].pos.z - platformsLeft[i].scaleZ && sphereZ - 0.2 > platformsLeft[i + 1].pos.z && sphereY <= 0.35) {
									end_fall = true;
								}
								else {
									if (sphereZ >= platformsLeft[i].pos.z - platformsLeft[i].scaleZ) {
										check_the_color(platformsLeft[i].color);
										platformsLeft[i].readyToLaunch = true;
										platformsLeft[i].color = 5;
									}

								}
							}
							else {
								if (sphereZ < platformsLeft[14].pos.z - platformsLeft[14].scaleZ && sphereZ - 0.2 > platformsLeft[0].pos.z && sphereY <= 0.35) {
									end_fall = true;
								}
								else {
									if (sphereZ >= platformsLeft[14].pos.z - platformsLeft[14].scaleZ) {
										check_the_color(platformsLeft[14].color);
										platformsLeft[14].readyToLaunch = true;
										platformsLeft[14].color = 5;
									}
								}
							}
						}
					}
				}
				if (sphereX > 0.5) {
					if (sphereX + 0.14 < 1 || sphereX > 2) {
						end_fall = true;
					}
					else {
						if (sphereZ <= platformsRight[i].pos.z) {
							if (i != 14) {
								if (sphereZ < platformsRight[i].pos.z - platformsRight[i].scaleZ && sphereZ - 0.2 > platformsRight[i + 1].pos.z && sphereY <= 0.35) {
									end_fall = true;
								}
								else {
									if (sphereZ >= platformsRight[i].pos.z - platformsRight[i].scaleZ) {
										check_the_color(platformsRight[i].color);
										platformsRight[i].readyToLaunch = true;
										platformsRight[i].color = 5;
									}

								}
							}
							else {
								if (sphereZ < platformsRight[14].pos.z - platformsRight[14].scaleZ && sphereZ - 0.2 > platformsRight[0].pos.z && sphereY <= 0.35) {
									end_fall = true;
								}
								else {
									if (sphereZ >= platformsRight[14].pos.z - platformsRight[14].scaleZ) {
										check_the_color(platformsRight[14].color);
										platformsRight[14].readyToLaunch = true;
										platformsRight[14].color = 5;
									}
								}
							}
						}
					}
				}
				if (sphereX + 0.05 >= -0.5 && sphereX <= 0.5) {
					if (sphereZ <= platformsMid[i].pos.z) {
						if (i != 14) {
							if (sphereZ < platformsMid[i].pos.z - platformsMid[i].scaleZ && sphereZ - 0.2> platformsMid[i + 1].pos.z && sphereY <= 0.35) {
								end_fall = true;
							}
							else {
								if (sphereZ >= platformsMid[i].pos.z - platformsMid[i].scaleZ) {
									check_the_color(platformsMid[i].color);
									platformsMid[i].readyToLaunch = true;
									platformsMid[i].color = 5;
								}

							}
						}
						else {
							if (sphereZ < platformsMid[14].pos.z - platformsMid[14].scaleZ && sphereZ - 0.2 > platformsMid[0].pos.z && sphereY <= 0.35) {
								end_fall = true;
							}
							else {
								if (sphereZ >= platformsMid[14].pos.z - platformsMid[14].scaleZ) {
									check_the_color(platformsMid[14].color);
									platformsMid[14].readyToLaunch = true;
									platformsMid[14].color = 5;
								}
							}
						}
					}
				}
			}

		}

	}

	
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}


void Tema2::RenderSimpleSphere(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// get shader location for uniform mat4 "Model"
	int locationM = glGetUniformLocation(shader->GetProgramID(), "Model");

	// set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(locationM, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// get shader location for uniform mat4 "View"
	int locationV = glGetUniformLocation(shader->GetProgramID(), "View");

	// set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(locationV, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// get shader location for uniform mat4 "Projection"
	int locationP = glGetUniformLocation(shader->GetProgramID(), "Projection");

	// set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(locationP, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// trimitem parametru pentru a stabili daca deformam sfera
	int location = glGetUniformLocation(shader->program, "col");
	glUniform1i(location, col);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;
	glUseProgram(shader->program);

	// get shader location for uniform mat4 "Model"
	int locationM = glGetUniformLocation(shader->GetProgramID(), "Model");
	 
	// set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(locationM, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// get shader location for uniform mat4 "View"
	int locationV = glGetUniformLocation(shader->GetProgramID(), "View");

	// set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(locationV, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// get shader location for uniform mat4 "Projection"
	int locationP = glGetUniformLocation(shader->GetProgramID(), "Projection");

	// set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(locationP, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_S) && pressed_w == 0) {
		if (speed > 0) {
			speed -= deltaTime;
		}
		
	}
	
	if (window->KeyHold(GLFW_KEY_W) && pressed_w == 0) {
		if (speed < 20) {
			speed += deltaTime;
		}
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		sphereX -= deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		sphereX += deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_SPACE) && pressed == false) {
		jump = true;
		if (sphereY < 3) {
			sphereY += deltaTime * 3;
		}
		else {
			// sfera coboara singura daca a atins inaltimea maxima
			pressed = true;
		}
	}
	else {
		if (sphereY > 0.35) {
			sphereY -= deltaTime * 3;
		}
		else {  
			jump = false;
			pressed = false;
		}
	}
	
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_C)
	{
		mode++;
		if (mode % 2 == 0) {
			camera = new Tema::Camera();
			camera->Set(glm::vec3(0, 1, 3.f), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		}
		else {
			camera = new Tema::Camera();
			camera->Set(glm::vec3(0, 1.05, 0.2f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
		}
	}
	
	
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_SPACE)
	{
		pressed = true;
	}
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
