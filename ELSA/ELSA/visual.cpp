#include "visual.h"
//#define _maxParticles 27

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

visual::visual()//:_maxParticles(0)
{
	//_theGui.setupGui(getWindow());
	initVisual();
}

visual::visual(const int maxPart)//:_maxParticles(maxPart)
{
	//_theGui.setupGui(getWindow());
	initVisual();
}


std::array<visual::_particle, _maxParticles> visual::getParticlesContainer()
{
	return _particlesContainer;
}

void visual::setParticleCameraDist(int id, float dist)
{
	_particlesContainer[id].cameradistance = dist;
}

void visual::setParticlePosition(int id, glm::vec3 pos)
{
	_particlesContainer[id].pos = pos;
}

void visual::setParticleColor(int id, glm::vec4 color)
{
	_particlesContainer[id].r = static_cast<unsigned char>(color.x);
	_particlesContainer[id].g = static_cast<unsigned char>(color.y);
	_particlesContainer[id].b = static_cast<unsigned char>(color.z);
	_particlesContainer[id].a = static_cast<unsigned char>(color.w);
}

void visual::setParticleSize(int id, float size)
{
	_particlesContainer[id].size = size;
}

int visual::initGLFW()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void visual::openWindow()
{
	window = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height, "ELSA MD-Simulation", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		//return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	//return 0;
}



void visual::sortParticles()
{
	std::sort(&_particlesContainer[0], &_particlesContainer[_maxParticles]); 
}

void visual::initVisual()
{
	initGLFW();
	openWindow();
	initGLEW();
	initOpenGL();


}

void visual::initGLEW()
{
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		//return -1;
	}

}

void visual::initOpenGL()
{
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, glfwGetVideoMode(glfwGetPrimaryMonitor())->width / 2, glfwGetVideoMode(glfwGetPrimaryMonitor())->height / 2);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &_vertexArrayID);
	glBindVertexArray(_vertexArrayID);

	// Create and compile our GLSL program from the shaders
	_programID = LoadShaders("Particle.vertexshader", "Particle.fragmentshader");

	// Vertex shader
	_cameraRightWorldspaceID = glGetUniformLocation(_programID, "CameraRight_worldspace");
	_cameraUpWorldspaceID = glGetUniformLocation(_programID, "CameraUp_worldspace");
	_viewProjMatrixID = glGetUniformLocation(_programID, "VP");

	// fragment shader
	_textureID = glGetUniformLocation(_programID, "myTextureSampler");

	_gParticulePositionSizeData = new GLfloat[_maxParticles * 4];
	_gParticuleColorData = new GLubyte[_maxParticles * 4];

	for (int i = 0; i < _maxParticles; i++) {
		//ParticlesContainer[i].life = -1.0f;
		setParticleCameraDist(i, -1.0f);
	}

	_texture = loadDDS("particle.DDS");

	glGenBuffers(1, &_billboardVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _billboardVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_gVertexBufferData), _gVertexBufferData, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &_particlesPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _particlesPositionBuffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, _maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &_particlesColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _particlesColorBuffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, _maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

	for (int i = 0; i < _maxParticles; i++) {
		int particleIndex = i;

		//setParticlePosition(particleIndex, glm::vec3(0, 0, -20.0f));
		setParticleColor(particleIndex, glm::vec4(50, 30, 255, 255));
		setParticleSize(particleIndex, 0.1f);

		visual::_particle& p = getParticlesContainer()[i]; // shortcut
	}
}

GLuint visual::getTexture()
{
	return _texture;
}

GLuint visual::getBillboardVertexBuffer()
{
	return _billboardVertexBuffer;
}

GLuint visual::getParticlesPositionBuffer()
{
	return _particlesPositionBuffer;
}

GLuint visual::getParticlesColorBuffer()
{
	return _particlesColorBuffer;
}

void visual::setAtomsVisible(bool visibility)
{
	_atomsVisible = visibility;
}

GLFWwindow * visual::getWindow()
{
	return window;
}

GLuint visual::getVertexArrayID()
{
	return _vertexArrayID;
}

GLuint visual::getProgramID()
{
	return _programID;
}

GLuint visual::getCameraRightWorldspaceID()
{
	return _cameraRightWorldspaceID;
}

GLuint visual::getCameraUpWorldspaceID()
{
	return _cameraUpWorldspaceID;
}

GLuint visual::getViewProjMatrix()
{
	return _viewProjMatrixID;
}

GLuint visual::getTextureID()
{
	return _textureID;
}

GLfloat* visual::getGParticulePositionSizeData()
{
	return _gParticulePositionSizeData;
}

GLubyte* visual::getGParticuleColorData()
{
	return _gParticuleColorData;
}

Gui visual::getGui()
{
	return _theGui;
}

void visual::mainLoopVisual(double*** pos, int time, int maxTime)
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (glfwGetKey(getWindow(), GLFW_KEY_M) == GLFW_PRESS)
	{
		glfwSetCursorPos(getWindow(),
			glfwGetVideoMode(glfwGetPrimaryMonitor())->width / 2,
			glfwGetVideoMode(glfwGetPrimaryMonitor())->height / 2);
		glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		computeMatricesFromInputs(getWindow());
	}
	else
	{
		glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();

	// We will need the camera's position in order to sort the particles
	// w.r.t the camera's distance.
	// There should be a getCameraPosition() function in common/controls.cpp, 
	// but this works too.
	glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);

	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;


	// Simulate all particles
	int ParticlesCount = 0;
	float positionsF[_maxParticles][3];
	float latticeConstant = 408.53e-12;
	if (pos != NULL && time < (int)floor(maxTime/5))
	{
		for (int i = 0; i < _maxParticles; i++)
		{
			positionsF[i][0] = static_cast<float>(pos[time*5][i][0]) / latticeConstant;
			positionsF[i][1] = static_cast<float>(pos[time*5][i][1]) / latticeConstant;
			positionsF[i][2] = static_cast<float>(pos[time*5][i][2]) / latticeConstant;
		}

		for (int i = 0; i < _maxParticles; i++) {
			//_particlesContainer[i].speed = glm::vec3(0.001,0.001,0.001);
			//_particlesContainer[i].pos += _particlesContainer[i].speed;
			_particlesContainer[i].pos = glm::vec3(positionsF[i][0], positionsF[i][1], positionsF[i][2]);

			//_particlesContainer[i].pos = glm::vec3(0, 1, 2);
			//printf("Pos x: %e y: %e z: %e\n", pos[0][i][0], pos[0][i][1], pos[0][i][2]);
			//printf("PPos x: %e y: %e z: %e\n", _particlesContainer[i].pos.x, _particlesContainer[i].pos.y, _particlesContainer[i].pos.z);

			_particlesContainer[i].cameradistance = glm::length(_particlesContainer[i].pos - CameraPosition);

			// Fill the GPU buffer
			if (positionsF[i][0] > 0.1 && positionsF[i][0] < 4.9 && positionsF[i][1] > 0.1 && positionsF[i][1] < 4.9 &&positionsF[i][2] > 0.1 && positionsF[i][2] < 4.9)
			{
				_gParticulePositionSizeData[4 * ParticlesCount + 0] = _particlesContainer[i].pos.x;
				_gParticulePositionSizeData[4 * ParticlesCount + 1] = _particlesContainer[i].pos.y;
				_gParticulePositionSizeData[4 * ParticlesCount + 2] = _particlesContainer[i].pos.z;

				_gParticulePositionSizeData[4 * ParticlesCount + 3] = _particlesContainer[i].size;


				_gParticuleColorData[4 * ParticlesCount + 0] = _particlesContainer[i].r;
				_gParticuleColorData[4 * ParticlesCount + 1] = _particlesContainer[i].g;
				_gParticuleColorData[4 * ParticlesCount + 2] = _particlesContainer[i].b;
				_gParticuleColorData[4 * ParticlesCount + 3] = _particlesContainer[i].a;

				ParticlesCount++;
			}


		}

		sortParticles();
	}
	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but this is outside the scope of this tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming


	glBindBuffer(GL_ARRAY_BUFFER, getParticlesPositionBuffer());
	glBufferData(GL_ARRAY_BUFFER, _maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, _gParticulePositionSizeData);

	glBindBuffer(GL_ARRAY_BUFFER, getParticlesColorBuffer());
	glBufferData(GL_ARRAY_BUFFER, _maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, _gParticuleColorData);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader
	glUseProgram(_programID);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getTexture());
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(_textureID, 0);

	// Same as the billboards tutorial
	glUniform3f(_cameraRightWorldspaceID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(_cameraUpWorldspaceID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniformMatrix4fv(_viewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, getBillboardVertexBuffer());
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, getParticlesPositionBuffer());
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, getParticlesColorBuffer());
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

								 // Draw the particules !
								 // This draws many times a small triangle_strip (which looks like a quad).
								 // This is equivalent to :
								 // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
								 // but faster.
	if (_atomsVisible)
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//handleGui();

}

//void visual::handleGui()
//{
//	_theGui.handleFrame();
//
//	if (_theGui.VisualVisible())
//		setAtomsVisible(true);
//	else
//		setAtomsVisible(false);
//
//	_theGui.handleMenu();
//}