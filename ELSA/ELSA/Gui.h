
// Include GLEW
#include <GL/glew.h>
#include <array>
#include <omp.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

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
	bool _saveResultWindow;
	bool _loadResultWindow;
	bool _simulationWindow;
	float _elementaryCharge = 1.602176620898e-19;
	

	bool _unableToOpenFile;

	std::string _crystalType;
	double _temperature;
	double _timeStep;
	double _simulationTime;
	bool _thermostat;
	bool _useLastSimulationState;
	bool _2D;
	double _latticeConstant;
	double _epsilon;
	double _sigma;
	double _cutOffDistance;
	double _mass;
	bool _visualization;
	int _numberOfThreads{omp_get_max_threads()};
	int _numberOfUnitCellsX;
	int _numberOfUnitCellsY;
	int _numberOfUnitCellsZ;
	double _collisionPercentage;
	char* _lastStateFileName;

	int _arrayLength{ -1 };
	int _numberOfAtoms{ 0 };
	int _numberOfTimeStepsPlot;

	bool _simulate;
	bool _isVisualisationPause = false;
	int _saveSpeed = 1;

	void printStatistics(double, double, double);
	void printTimeForVisualisation(int*, int);
	
public:
	Gui();
	~Gui();


	void setupGui(GLFWwindow *window);

	bool _visible{ false };
	bool _initializing{ false };

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
	bool isUseLastSimulationState();
	void setMainVisible(bool vis);
	bool is2D();
	int getNumberOfUnitCellsX();
	int getNumberOfUnitCellsY();
	int getNumberOfUnitCellsZ();
	int getNumberOfThreads();
	double getLatticeConstant();
	double getEpsilon();
	double getSigma();
	double getCutOffDistance();
	double getMass();
	double getCollisionPercentage();
	char* getLastStateFileName();

	int _counter;

	double* _cohesiveEnergy = nullptr;
	double* _debyeTemperature = nullptr;
	double* _kineticEnergy = nullptr;
	double* _meanSquareDisplacement = nullptr;
	double*** _pos = nullptr;
	double* _potentialEnergy = nullptr;
	double* _pressure = nullptr;
	double* _selfDiffusionCoeff = nullptr;
	double* _specificHeat = nullptr;
	double* _temp = nullptr;
	double* _totalEnergy = nullptr;

	bool showCrystalSelector(const char* label);
	bool showMaterialSelector(const char* label);
	bool showThreadSelector(const char* label);

	void handleFrame();

	void saveResultsWindow();
	void loadResultsWindow();

	void handleMenu(double, double, int*, int, int*);

	void handleCollapsingHeaders();
	void handleConfigurationHeader();
	void handleSettingsHeader();

	void handleProgressBar(double, double);

	void simulateButtonHandler();
	void stopButtonHandler();

	void Gui::visualWindow(int*, int, int*);

	void simulationWindow(double elapsedTime, double totalTime);

	bool VisualVisible() const;

	bool exitPressed() const;

	void setNumberOfTimeStepsPlot(int);

	void handlePlots();// const float* kinEn, const float* totEn, const float* temp);

	void updateArrays(int, int);
};