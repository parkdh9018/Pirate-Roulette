/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>
#include "math.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include "Geometry/TriangularSurface/StaticTriangularSurface.h"
#include "GL2/GL2_Object.h"
#include "GL2/GL2_World.h"
#include "GL2/GL2_Light.h"
#include "GL2/GL2_Material.h"
#include "GL2/GL_VIEW.h"
#include "BMPImage.h"

#define SWORD_MAX 20

int width_window = 640;
int height_window = 480;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

GL2_World gl_world;
bool swordOk[SWORD_MAX] = { false };
int move_flag = 0;
int sword_num = 0;
bool IsGameOver = false;
bool isGameStart = false;

void TextureMapping(StaticTriangularSurface &surface)
{
	surface.vertex_uv_.init(surface.vertex_positions_.num_elements_);
	for (int i = 0; i < surface.vertex_positions_.num_elements_; ++i)
	{
		glm::vec2 ray_xy(surface.vertex_positions_[i].x_, surface.vertex_positions_[i].z_);
		glm::vec2 x_axis(1.0f, 0.0f);
		float theta = acos(dot(glm::normalize(ray_xy), x_axis));

		if (ray_xy.y < 0.0) theta *= -1;

		surface.vertex_uv_[i].x_ = theta / 3.141592f + 0.5f;
		surface.vertex_uv_[i].y_ = surface.vertex_positions_[i].y_ + 0.5f;
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	gl_world.camera_.UpdateDolly(yoffset);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (!isGameStart) {
		isGameStart = true;
		gl_world.camera_.cameraPos = glm::vec3(0.0f, 1.5f, 2.0f);
		gl_world.camera_.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		gl_world.camera_.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		return;
	}
	if (IsGameOver)
		return;

	if (key == GLFW_KEY_A)
	{
		if (!move_flag) {
			move_flag = -1;

			if (sword_num == 0)
				sword_num = SWORD_MAX - 1;
			else
				sword_num -= 1;
		}
	}
	if (key == GLFW_KEY_D)
	{
		if (!move_flag) {
			move_flag = 1;
			sword_num = (sword_num + 1) % SWORD_MAX;
		}

	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if(!swordOk[sword_num] && !move_flag)
			swordOk[sword_num] = true;
	}

}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(isGameStart)
		gl_world.camera_.ProcessMouseMotion(xpos, ypos);
}

int main(void)
{
	GLFWwindow *window = nullptr;

	/* Initialize the library */
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_SAMPLES, 32);

	// window resolution
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width_window = mode->width * 0.8f;
	height_window = mode->height * 0.8f;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width_window, height_window, "Pirate Roullet", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	// callbacks here
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glClearColor(1, 1, 1, 1); // while background

	printf("%s\n", glGetString(GL_VERSION));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);


	// in modern opengl, we initialize 
	const float zNear = 0.001, zFar = 100.0, fov = 45.0;			// UI
	gl_world.camera_.Resize(width, height, fov, zNear, zFar);
	gl_world.initShaders();

	//	glEnable(GL_MULTISAMPLE);

	StaticTriangularSurface surface;
	surface.vertex_positions_.init(4);
	surface.vertex_positions_[0] = TV(-1.0, -1.0, 0.0);
	surface.vertex_positions_[1] = TV(1.0, -1.0, 0.0);
	surface.vertex_positions_[2] = TV(1.0, 1.0, 0.0);
	surface.vertex_positions_[3] = TV(-1.0, 1.0, 0.0);
	surface.vertex_normals_.init(4);
	surface.vertex_normals_[0] = TV(0.0, 0.0, 1.0);
	surface.vertex_normals_[1] = TV(0.0, 0.0, 1.0);
	surface.vertex_normals_[2] = TV(0.0, 0.0, 1.0);
	surface.vertex_normals_[3] = TV(0.0, 0.0, 1.0);

	// texture coordinate
	surface.vertex_uv_.init(4);
	surface.vertex_uv_[0] = TV2(0.0, 0.0);
	surface.vertex_uv_[1] = TV2(1.0, 0.0);
	surface.vertex_uv_[2] = TV2(1.0, 1.0);
	surface.vertex_uv_[3] = TV2(0.0, 1.0);

	surface.triangles_.init(2);
	surface.triangles_[0] = TV_INT(0, 1, 2);
	surface.triangles_[1] = TV_INT(0, 2, 3);

	TextureMapping(surface);
	surface.scale(120.0f);
	GL2_Object sea;
	sea.translate(glm::vec3(0.0f, -15.0f, 0.0f));
	sea.rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	sea.initPhongSurfaceWithTexture(surface);

	StaticTriangularSurface surface2;
	surface2.readObj("barrel.obj", true, true);
	surface2.scale(1.4f);
	TextureMapping(surface2);
	GL2_Object barrel;
	barrel.initPhongSurfaceWithTexture(surface2);

	StaticTriangularSurface surface3;
	surface3.readObj("Pirate_Ship_v1.obj", true, true);
	TextureMapping(surface3);
	surface3.scale(35.0f);
	GL2_Object ship;
	ship.translate(glm::vec3(-0.2f, 4.0f, 0.0f));
	ship.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ship.initPhongSurfaceWithTexture(surface3);


	StaticTriangularSurface surface4;
	surface4.readObj("hat.obj", true, true);
	surface4.scale(0.5f);
	surface4.translate(glm::vec3(0.0f, 1.2f, 0.12f));
	surface4.rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	GL2_Object hat;
	hat.mat_.setRuby();
	hat.initPhongSurface(surface4);

	StaticTriangularSurface surface5;
	surface5.readObj("person.obj", true, true);
	surface5.scale(2.0f);
	surface5.translate(glm::vec3(0.0f, 0.25f, 0.1f));
	surface5.rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	GL2_Object person;
	person.mat_.setPerl();
	person.initPhongSurface(surface5);

	StaticTriangularSurface surface6;
	surface6.readObj("game_over.obj", true, true);
	TextureMapping(surface6);
	surface6.scale(4.0f);
	surface6.translate(glm::vec3(0.0f,2.0f, 0.0f));
	GL2_Object game_over;
	game_over.initPhongSurfaceWithTexture(surface6);

	StaticTriangularSurface surface7;
	surface7.readObj("pirate_roulette.obj", true, true);
	TextureMapping(surface7);
	surface7.scale(3.0f);
	surface7.rotate(glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	surface7.translate(glm::vec3(-13.5f, 8.5f, 15.0f));
	GL2_Object pirate_roulette;
	pirate_roulette.initPhongSurfaceWithTexture(surface7);

	StaticTriangularSurface surface8;
	surface8.readObj("anykey.obj", true, true);
	TextureMapping(surface8);
	surface8.scale(2.0f);
	surface8.rotate(glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	surface8.translate(glm::vec3(-13.5f, 7.3f, 15.0f));
	GL2_Object anykey;
	anykey.initPhongSurfaceWithTexture(surface8);

	StaticTriangularSurface surface9;
	surface9.readObj("pirate1.obj", true, true);
	surface9.scale(2.5f);
	GL2_Object pirate1;
	pirate1.translate(glm::vec3(2.4f, 0.0f, -1.5f));
	pirate1.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	pirate1.mat_.setCopper();
	pirate1.initPhongSurface(surface9);

	StaticTriangularSurface surface10;
	surface10.readObj("pirate2.obj", true, true);
	surface10.rotate(glm::radians( -90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	surface10.rotate(glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	surface10.scale(2.5f);
	GL2_Object pirate2;
	pirate2.translate(glm::vec3(-4.7f, 1.8f, -2.2f));
	pirate2.mat_.setCopper();
	pirate2.initPhongSurface(surface10);

	srand(time(NULL));
	StaticTriangularSurface surface_sw[SWORD_MAX];
	GL2_Object sword[SWORD_MAX];
	for (int i=0; i < SWORD_MAX; i++) {

		surface_sw[i].readObj("sword.obj", true, true);

		if (i % 2 == 0)
			surface_sw[i].translate(glm::vec3(0.0f, 0.2f, 1.2f));
		else
			surface_sw[i].translate(glm::vec3(0.0f, -0.2f, 1.2f));

		surface_sw[i].rotate(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		int rand_num = rand() % 8;
		switch (rand_num) {
			case 0:
				sword[i].mat_.setBrass();
				break;
			case 1:
				sword[i].mat_.setCopper();
				break;
			case 2:
				sword[i].mat_.setEmerald();
				break;
			case 3:
				sword[i].mat_.setGold();
				break;
			case 4:
				sword[i].mat_.setCyan();
				break;
			case 5:
				sword[i].mat_.setRuby();
				break;
			case 6:
				sword[i].mat_.setYellowpastic();
				break;
			case 7:
				sword[i].mat_.setGreenpastic();
				break;
		}
		sword[i].initPhongSurface(surface_sw[i]);
	}


	// depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_COLOR_MATERIAL);

	glLoadIdentity();

	GL2_Light light;

	GLuint textures[5];
	glGenTextures(5, textures);


	// read texture from file
	int texture_width = 0;
	int texture_height = 0;
	unsigned char *rgb_array = nullptr;
	readBMP24("wooden.bmp", &texture_width, &texture_height, &rgb_array);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_array);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	readBMP24("dark_steel.bmp", &texture_width, &texture_height, &rgb_array);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_array);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures[2]);

	readBMP24("wood_floor.bmp", &texture_width, &texture_height, &rgb_array);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_array);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	
	readBMP24("water_texture.bmp", &texture_width, &texture_height, &rgb_array);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_array);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



	int match_num = rand() % SWORD_MAX;
	match_num = 2;
	std::cout << "match num : " << match_num << std::endl;


	/* Loop until the user closes the window */
	ParticleSystem ps;
	bool once = true;


	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		gl_world.camera_.ContinueRotation();
		gl_world.camera_.viewUpdate();

		glm::mat4 m = glm::mat4(1.0f);
		glm::mat4 vp = gl_world.camera_.GetWorldViewMatrix();

		glm::mat4 mvp = vp * m;

		for (int i = 0; i < SWORD_MAX; i++)
			sword[i].updatePhongSurface(surface_sw[i]);
		
		for (int i = 0; i < surface.vertex_positions_.num_elements_; ++i)
			surface.vertex_uv_[i].x_ += 0.0001f;
		for (int i = 0; i < surface7.vertex_positions_.num_elements_; ++i)
			surface7.vertex_uv_[i].y_ += 0.0001f;

		sea.updatePhongSurfaceWithTexture(surface);
		pirate_roulette.updatePhongSurfaceWithTexture(surface7);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(gl_world.shaders_.program_id_);
		gl_world.shaders_.sendUniform(mvp, "mvp");



		if (move_flag)
		{
			float speed = 0.1f;
			barrel.updateRotate(speed,move_flag);
			person.updateRotate(speed, move_flag);
			hat.updateRotate(speed, move_flag);
			
			for (int i = 0; i < SWORD_MAX; i++) {
				if (swordOk[i]) {
					sword[i].updateRotate(speed, move_flag);
				}
			}
		}

		ps.advanceOneTimeStep(0.01);


		glUniform1i(glGetUniformLocation(gl_world.shaders_.program_id_, "wood"), 0);
		glUniform1i(glGetUniformLocation(gl_world.shaders_.program_id_, "dark_steel"), 1);
		glUniform1i(glGetUniformLocation(gl_world.shaders_.program_id_, "wood_floor"), 2);
		glUniform1i(glGetUniformLocation(gl_world.shaders_.program_id_, "water"), 3);


		gl_world.shaders_.sendUniform(0.0f, "location");
		barrel.applyLighting(light);
		barrel.drawWithShaderWithTexture(gl_world.shaders_);

		gl_world.shaders_.sendUniform(1.0f, "location");
		ship.applyLighting(light);
		ship.drawWithShaderWithTexture(gl_world.shaders_);

		gl_world.shaders_.sendUniform(2.0f, "location");
		hat.applyLighting(light);
		hat.drawWithShader(gl_world.shaders_);
		person.applyLighting(light);
		person.drawWithShader(gl_world.shaders_);
		pirate1.applyLighting(light);
		pirate1.drawWithShader(gl_world.shaders_);
		pirate2.applyLighting(light);
		pirate2.drawWithShader(gl_world.shaders_);

		for (int i = 0; i < SWORD_MAX; i++) {
			if (swordOk[i]) {
				if (match_num == i && once) {
					IsGameOver = true;
					ps.initiate(i);
					once = false;
				}
				sword[i].applyLighting(light);
				sword[i].updateTranslate(surface_sw[i], 0.01);
				sword[i].drawWithShader(gl_world.shaders_);
			}
		}

		gl_world.shaders_.sendUniform(3.0f, "location");
		sea.applyLighting(light);
		sea.drawWithShaderWithTexture(gl_world.shaders_);

		gl_world.shaders_.sendUniform(4.0f, "location");
		if (!isGameStart) {
			pirate_roulette.applyLighting(light);
			pirate_roulette.drawWithShaderWithTexture(gl_world.shaders_);

			anykey.applyLighting(light);
			anykey.drawWithShaderWithTexture(gl_world.shaders_);
		}

		if (IsGameOver) {
			game_over.applyLighting(light);
			game_over.drawWithShaderWithTexture(gl_world.shaders_);
		}



		glUseProgram(0);

		// Old-style rendering

		glLoadMatrixf(&mvp[0][0]);


		// draw particles
		glDisable(GL_LIGHTING);

		glPointSize(5.0f);
		glBegin(GL_POINTS);

		
		float height;
		for (int p = 0; p < ps.particles_.size(); ++p)
		{

			if (match_num % 2 == 0)
				height = 0.2;
			else
				height = -0.2;

			if (ps.particles_[p].pos_.y_ < height) {
				const TV3 color1 = TV3(1.0f, 0.0f, 0.0f), color2 = TV3(0.0, 0.0, 1.0f);
				const float alpha = ps.particles_[p].vel_.getMagnitude() * 0.5f;
				const TV3 blenced_color = alpha * color1 + (1.0f - alpha) * color2;

				glColor3fv(blenced_color.values_);
				glVertex3fv(ps.particles_[p].pos_.values_);
			}
		}
	
		glEnd();


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	glfwTerminate();

	return 0;
}

