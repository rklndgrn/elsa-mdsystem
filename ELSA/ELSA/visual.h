//// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>

//Include classes
#include "Gui.h"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <GLFW\glfw3.h>

#include "shader.hpp"
#include "texture.hpp"
#include "controls.hpp"
#include "objloader.hpp"

#pragma once
#define _maxParticles 27

class visual
{
public:
	visual();
	visual(const int maxPart);
	~visual() = default;

	struct _particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a; // Color
		float size;
		float cameradistance;

		bool operator<(const _particle& that) const {
			// Sort in reverse order : far particles drawn first.
			return this->cameradistance > that.cameradistance;
		}
	};

	std::array<_particle, _maxParticles> getParticlesContainer();
	void setParticleCameraDist(int id, float dist);
	void setParticlePosition(int id, glm::vec3 pos);
	void setParticleColor(int id, glm::vec4 color);
	void setParticleSize(int id, float size);
	int initGLFW();
	void openWindow();
	void sortParticles();
	void initVisual();
	void initGLEW();
	void initOpenGL();

	void mainLoopVisual(double*** pos);

	void mainLoopVisual();

	GLuint getTexture();
	GLuint getBillboardVertexBuffer();
	GLuint getParticlesPositionBuffer();
	GLuint getParticlesColorBuffer();

	void setAtomsVisible(bool visibility);

	GLFWwindow* getWindow();

	GLuint getVertexArrayID();
	GLuint getProgramID();
	GLuint getCameraRightWorldspaceID();
	GLuint getCameraUpWorldspaceID();
	GLuint getViewProjMatrix();
	GLuint getTextureID();

	GLfloat* getGParticulePositionSizeData();
	GLubyte* getGParticuleColorData();

	Gui getGui();
	//void handleGui();

private:
	Gui _theGui;
	//int _maxParticles;
	bool _atomsVisible = false;
	GLFWwindow * window;
	GLuint _texture;
	const GLfloat _gVertexBufferData[12] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};
	GLuint _billboardVertexBuffer;
	GLuint _particlesPositionBuffer;
	GLuint _particlesColorBuffer;
	GLuint _vertexArrayID;
	GLuint _programID;
	// Vertex shader
	GLuint _cameraRightWorldspaceID;
	GLuint _cameraUpWorldspaceID;
	GLuint _viewProjMatrixID;

	// fragment shader
	GLuint _textureID;

	GLfloat* _gParticulePositionSizeData;
	GLubyte* _gParticuleColorData;

	std::array<_particle, _maxParticles> _particlesContainer;
};