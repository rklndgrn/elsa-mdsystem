//// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <array>

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
#define _maxParticles 20000 //ca 17x17x17 (x4 fcc)

class visual
{
private:
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

	Gui _theGui;

	std::array<_particle, _maxParticles> _particlesContainer;

	bool _atomsVisible = false;

	int _numberOfParticles;

	const GLfloat _gVertexBufferData[12] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	GLfloat* _gParticulePositionSizeData;

	GLFWwindow* window;

	GLubyte* _gParticuleColorData;
	
	GLuint _billboardVertexBuffer;
	GLuint _cameraRightWorldspaceID;
	GLuint _cameraUpWorldspaceID;
	GLuint _particlesColorBuffer;
	GLuint _particlesPositionBuffer;
	GLuint _programID;
	GLuint _texture;
	GLuint _textureID;
	GLuint _vertexArrayID;
	GLuint _viewProjMatrixID;

public:
	visual();
	visual(const int maxPart);

	~visual();

	std::array<_particle, _maxParticles> getParticlesContainer();

	GLfloat* getGParticulePositionSizeData();

	GLFWwindow* getWindow();

	GLubyte* getGParticuleColorData();

	GLuint getBillboardVertexBuffer();
	GLuint getCameraRightWorldspaceID();
	GLuint getCameraUpWorldspaceID();
	GLuint getParticlesColorBuffer();
	GLuint getParticlesPositionBuffer();
	GLuint getProgramID();
	GLuint getTexture();
	GLuint getTextureID();
	GLuint getVertexArrayID();
	GLuint getViewProjMatrix();

	Gui getGui();

	void initGLEW();
	void initGLFW();
	void initOpenGL();
	void initVisual();
	void mainLoopVisual(double***, int, int, double, int, int, int);
	void openWindow();
	void setAtomsVisible(bool visibility);
	void setNumberOfParticles(int);
	void setParticleCameraDist(int id, float dist);
	void setParticlePosition(int id, glm::vec3 pos);
	void setParticleColor(int id, glm::vec4 color);
	void setParticleSize(int id, float size);
	void sortParticles();
};