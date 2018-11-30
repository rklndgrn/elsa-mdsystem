
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

#pragma once

class Gui
{
private:
	bool _2D;
	bool _exitPressed;
	bool _initializing{ false };
	bool _isVisualisationPause = false;
	bool _mainVisible;
	bool _loadResultWindow;
	bool _plotVisible;
	bool _saveResultWindow;
	bool _simulate;
	bool _simulationWindow;
	bool _thermostat;
	bool _unableToOpenFile;
	bool _useLastSimulationState;
	bool _visible{ false };
	bool _visualization;
	bool _visualVisible;

	const char* _name;
	char* _lastStateFileName;

	double _collisionPercentage;
	double _cutOffDistance;
	double _epsilon;
	double _latticeConstant;
	double _mass;
	double _sigma;
	double _simulationTime;
	double _initialTemperature;
	double _timeStep;

	double* _cohesiveEnergy;
	double* _debyeTemperature;
	double* _kineticEnergy;
	double* _meanSquareDisplacement;
	double* _potentialEnergy;
	double* _pressure;
	double* _selfDiffusionCoefficient;
	double* _specificHeat;
	double* _simulatedTemperature;
	double* _totalEnergy;

	double*** _positions;

	float _elementaryCharge = 1.602176620898e-19;
	
	int _counter;
	int _numberOfThreads{ omp_get_max_threads() };
	int _numberOfTimeStepsPlot;
	int _numberOfUnitCellsX;
	int _numberOfUnitCellsY;
	int _numberOfUnitCellsZ;
	int _saveSpeed = 1;

	std::string _crystalType;
	
	void printStatistics(double, double, double);
	
public:
	Gui();
	~Gui() = default;

	// ------ FUNCTIONS ------
	
	bool exitPressed() const;
	bool isInitializing() const;
	bool is2D();
	bool isThermostat();
	bool isUseLastSimulationState();
	bool showCrystalSelector(const char* label);
	bool showMaterialSelector(const char* label);
	bool showThreadSelector(const char* label);
	bool simulate() const;
	bool visible() const;
	bool VisualVisible() const;

	char* getLastStateFileName();

	double getCollisionPercentage();
	double getCutOffDistance();
	double getEpsilon();
	double getLatticeConstant();
	double getMass();
	double getSigma();
	double getSimulationTime();
	double getTemperature();
	double getTimeStep();

	double* getCohesiveEnergy() const;
	double* getDebyeTemperature() const;
	double* getKineticEnergy() const;
	double* getMeanSquareDisplacement() const;
	double* getPotentialEnergy() const;
	double* getPressure() const;
	double* getSelfDiffusionCoefficient() const;
	double* getSpecificHeat() const;
	double* getSimulatedTemperature() const;
	double* getTotalEnergy() const;

	double*** getPositions() const;

	int getNumberOfUnitCellsX();
	int getNumberOfUnitCellsY();
	int getNumberOfUnitCellsZ();
	int getNumberOfThreads();

	static void showHelpMarker(const char* desc);

	std::string getCrystalType();
	
	void handleCollapsingHeaders();
	void handleConfigurationHeader();
	void handleFrame();
	void handleMenu(double, double, int*, int, int*);
	void handlePlots();
	void handleProgressBar(double, double);
	void handleSettingsHeader();
	void hide();
	void loadResultsWindow();
	void saveResultsWindow();
	void setCohesiveEnergy(double*);
	void setDebyeTemperature(double*);
	void setInitializing(bool);
	void setKineticEnergy(double*);
	void setMainVisible(bool vis);
	void setMeanSquareDisplacement(double*);
	void setNumberOfTimeStepsPlot(int);
	void setPositions(double***);
	void setPotentialEnergy(double*);
	void setPressure(double*);
	void setSelfDiffusionCoefficient(double*);
	void setSpecificHeat(double*);
	void setSimulatedTemperature(double*);
	void setTotalEnergy(double*);
	void setupGui(GLFWwindow *window);
	void show();
	void simulateButtonHandler();
	void simulationWindow(double elapsedTime, double totalTime);
	void stopButtonHandler();
	void stopSimulate();
	void visualWindow(int*, int, int*);	
};