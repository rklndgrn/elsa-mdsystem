#ifndef CONTROLS_HPP
#define CONTROLS_HPP

//extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

void computeMatricesFromInputs(GLFWwindow *);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif