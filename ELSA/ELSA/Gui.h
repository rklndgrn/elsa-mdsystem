
// Include GLEW
#include <GL/glew.h>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Include GLFW
#include <GLFW/glfw3.h>

#include "imgui-master\imgui.h"
#include "imgui-master\imgui_internal.h"
#include "imgui-master\imgui_impl_glfw.h"
#include "imgui-master\imgui_impl_opengl3.h"
//#include "imgui-master\imgui_plot.h"

#pragma once

class Gui
{
private:
	const char* _name;
	bool _plotVisible;
	bool _visualVisible;
	bool _mainVisible;
	bool _exitPressed;

	std::string _crystalType;
	double _temperature;
	double _timeStep;
	double _simulationTime;
	bool _thermostat;
	bool _2D;
	double _latticeConstant;
	double _epsilon;
	double _sigma;
	double _cutOffDistance;
	double _mass;
	bool _visualization;
	int _numberOfUnitCellsX;
	int _numberOfUnitCellsY;
	int _numberOfUnitCellsZ;
	double _collisionFrequency;

	bool _simulate;
	
public:
	Gui();
	~Gui() = default;


	void setupGui(GLFWwindow *window);

	bool _visible{ false };

	void show();
	bool visible() const;
	bool simulate() const;
	void stopSimulate();
	void hide();

	static void showHelpMarker(const char* desc);

	std::string getCrystalType();
	double getTemperature();
	double getTimeStep();
	double getSimulationTime();
	bool isThermostat();
	bool is2D();
	int getNumberOfUnitCellsX();
	int getNumberOfUnitCellsY();
	int getNumberOfUnitCellsZ();
	double getLatticeConstant();
	double getEpsilon();
	double getSigma();
	double getCutOffDistance();
	double getMass();
	double getCollisionFrequency();
	int _counter;

	double* _kinArray;
	
	bool showCrystalSelector(const char* label);

	void handleFrame();

	void handleMenu();

	void handleCollapsingHeaders();
	void handleConfigurationHeader();
	void handleSettingsHeader();

	void simulateButtonHandler();

	bool VisualVisible() const;

	bool exitPressed() const;

	void handlePlots(const double* potEn);// const float* kinEn, const float* totEn, const float* temp);
};