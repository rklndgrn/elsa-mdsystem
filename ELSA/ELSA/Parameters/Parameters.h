#pragma once
#include "../Material/Material.h"
#include <cmath>
#include <string>

/*
	File name: parameters.h
	Date: 2018-11-06
	This file contains the definition of the class Parameters
*/

class Parameters
{
private:
	unsigned int _numberOfAtoms;
	unsigned int _numberOfCellsI;
	unsigned int _numberOfCellsJ;
	unsigned int _numberOfCellsK;
	unsigned int _numberOfUnitCellsX;
	unsigned int _numberOfUnitCellsY;
	unsigned int _numberOfUnitCellsZ;


	double _boltzmann{ 1.3806485279E-23 };
	double _collisionFrequency;
	double _lengthX;
	double _lengthY;
	double _lengthZ;
	double _normalizedPlanck{ 1.05457180013e-34 };
	double _simulationTime;
	double _temperature;
	double _timeStep;

	bool _isThermostatOn;
	bool _isUseLastSimulationState;
	bool _is2D;

	char* _lastStateFileName;

	Material _chosenMaterial;

public:
	Parameters();
	~Parameters() = default;
	Parameters(
		double,
		double,
		unsigned int,
		unsigned int,
		unsigned int,
		double,
		double,
		bool,
		bool,
		bool,
		char*,
		Material);
	Parameters(Parameters const&); //Copy constructor.
	Parameters & operator = (Parameters const&); //Copy allocation.


	//Getters.
	unsigned int getNumberOfAtoms() const;
	unsigned int getNumberOfCellsI() const;
	unsigned int getNumberOfCellsJ() const;
	unsigned int getNumberOfCellsK() const;
	unsigned int getNumberOfUnitCellsX() const;
	unsigned int getNumberOfUnitCellsY() const;
	unsigned int getNumberOfUnitCellsZ() const;

	double getBoltzmann() const;
	double getCollisionFrequency() const;
	double getLengthX() const;
	double getLengthY() const;
	double getLengthZ() const;
	double getNormalizedPlanck() const;
	double getSimulationTime() const;
	double getTemperature() const;
	double getTimeStep() const;

	bool getIs2D() const;
	bool getIsThermostatOn() const;
	bool getIsUseLastSimulationState() const;

	char* getLastStateFileName() const;

	Material getChosenMaterial() const;

	//Setters.
	void setChosenMaterial(Material);
	void setCollisionFrequency(double);
	void setIsThermostatOn(bool);
	void setIsUseLastSimulationState(bool);
	void setIs2D(bool);
	void setNumberOfAtoms(unsigned int);
	void setSimulationTime(double);
	void setTemperature(double);
	void setTimeStep(double);


};