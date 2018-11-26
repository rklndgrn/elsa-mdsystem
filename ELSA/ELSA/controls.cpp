// Include GLFW
#include <GLFW/glfw3.h>


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, -10); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 0.0f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 20.0f;

float speed = 0.1f; // 3 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs(GLFWwindow * window){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 
		glfwGetVideoMode(glfwGetPrimaryMonitor())->width /2, 
		glfwGetVideoMode(glfwGetPrimaryMonitor())->height /2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(glfwGetVideoMode(glfwGetPrimaryMonitor())->width /2 - xpos );
	verticalAngle   += mouseSpeed * float(glfwGetVideoMode(glfwGetPrimaryMonitor())->height /2 - ypos );

	//rotation matrices
	glm::mat3 horizontalRotationPos(cos(mouseSpeed), 0, -sin(mouseSpeed), 0, 1, 0, sin(mouseSpeed), 0, cos(mouseSpeed));
	glm::mat3 horizontalRotationNeg(cos(mouseSpeed), 0, -sin(-mouseSpeed), 0, 1, 0, sin(-mouseSpeed), 0, cos(-mouseSpeed));

	glm::mat3 verticalRotationPos(1, 0, 0, 0, cos(mouseSpeed), sin(mouseSpeed), 0, -sin(mouseSpeed), cos(mouseSpeed));
	glm::mat3 verticalRotationNeg(1, 0, 0, 0, cos(-mouseSpeed), sin(-mouseSpeed), 0, -sin(-mouseSpeed), cos(-mouseSpeed));


	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		horizontalAngle -= mouseSpeed;
		position = horizontalRotationNeg * position;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		horizontalAngle += mouseSpeed;
		position = horizontalRotationPos * position;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		verticalAngle -= mouseSpeed;
		position = verticalRotationPos * position;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		verticalAngle += mouseSpeed;
		position = verticalRotationNeg * position;
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if ((glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS) ||
	(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)){
		position += direction  * speed;
	}
	// Move backward
	if ((glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS) ||
		(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)) {
		position -= direction * speed;
	}
	// Strafe right
	if ((glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS) ||
		(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {
		position += right * speed;
	}
	// Strafe left
	if ((glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS) ||
		(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)) {
		position -= right * speed;
	}

	

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}