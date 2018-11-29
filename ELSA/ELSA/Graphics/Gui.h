
// Include GLEW
#include <GL/glew.h>
#include <array>
#include <omp.h>
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
#define _maxTimeSteps 7500
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

	bool _simulate;
	bool _isVisualisationPause = false;
	int _saveSpeed = 1;

	void printStatistics(double, double, double);
	void printTimeForVisualisation(int*, int);
	
public:
	Gui();
	~Gui() = default;


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

	double* _cohesiveEnergy;
	double* _debyeTemperature;
	double* _kineticEnergy;
	double* _meanSquareDisplacement;
	double*** _pos;
	double* _potentialEnergy;
	double* _pressure;
	double* _selfDiffusionCoeff;
	double* _specificHeat;
	double* _temp;
	double* _totalEnergy;

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

	void handlePlots();// const float* kinEn, const float* totEn, const float* temp);
};