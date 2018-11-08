/*
	File name: parameters.h
	Date: 2018-11-06
	This file contains the definition of the class Parameters
*/

#pragma once
#include "../Material/Material.h"
#include <cmath>
#include <string>

class Parameters
{
private:
	unsigned int _numberOfAtoms;
	unsigned int _numberOfUnitCellsX;
	unsigned int _numberOfUnitCellsY;
	unsigned int _numberOfUnitCellsZ;
	unsigned int _timeStep;
	unsigned int _simulationTime;
	double _boltzmann{ 1.3806485279E-23 };
	double _temperature;
	double _collisionFrequency;
	bool _isThermostatOn;
	bool _is2D;
	Material _chosenMaterial;

public:
	Parameters();
	Parameters(
		unsigned int numberOfAtoms,
		unsigned int timeStep,
		unsigned int simulationTime,
		double temperature,
		double collisionFrequency,
		bool isThermostatOn,
		bool is2D,
		Material chosenMaterial);
	~Parameters();

	bool getIs2D() const;
	bool getIsThermostatOn() const;

	double getCollisionFrequency() const;
	unsigned int getNumberOfUnitCellsX() const;
	unsigned int getNumberOfUnitCellsY() const;
	unsigned int getNumberOfUnitCellsZ() const;
	double getBoltzmann() const;
	double getTemperature() const;

	Material getChosenMaterial() const;

	unsigned int getNumberOfAtoms() const;
	unsigned int getTimeStep() const;
	unsigned int getSimulationTime() const;

	void setNumberOfAtoms(unsigned int noa);
	void setTimeStep(unsigned int ts);
	void setSimulationTime(unsigned int st);
	void setTemperature(double temp);
	void setCollisionFrequency(double cf);
	void setIsThermostatOn(bool isT);
	void setIs2D(bool is2D);
	void setChosenMaterial(Material mat);


};