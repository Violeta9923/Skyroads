#pragma once
#include <Component/SimpleScene.h>
#include "Labcamera.h"

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;
	

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
	void RenderSimpleSphere(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
	void check_the_color(int color);


	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	Tema::Camera* camera;
	glm::mat4 projectionMatrix;
	float initialCoord;
	float outCoord;
	float up_speed;
	float speed;
	int platMax;
	float scaleX;
	float scaleY;
	float scaleZ;
	float initialLeft;
	float initialRight;
	float initialMid;
	int col;
	float sphereX;
	float sphereY;
	float sphereZ;
	bool jump;
	bool end_red;
	bool end_fall;
	bool pressed;
	float fuel;
	int mode;
	float pressed_w;
	float sphere_scale_x;
	float sphere_scale_y;
	float sphere_scale_z;
	float contor;
};
