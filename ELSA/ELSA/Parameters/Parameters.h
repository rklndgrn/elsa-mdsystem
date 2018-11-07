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
	unsigned int _timeStep;
	unsigned int _simulationTime;
	double _boltzmann = 1.3806485279 * pow(10, -23);
	double _temperature;
	double _collisionFrequency;
	bool _isThermostatOn;
	bool _is2D;
	Material _chosenMaterial;

public:
	double getBoltzmann() const;
	Material getChosenMaterial() const;
	double getCollisionFrequency() const;
	bool getIs2D() const;
	bool getIsThermostatOn() const;
	unsigned int getNumberOfAtoms() const;
	unsigned int getTimeStep() const;
	unsigned int getSimulationTime() const;
	double getTemperature() const;

	void setNumberOfAtoms(unsigned int noa);
	void setTimeStep(unsigned int ts);
	void setSimulationTime(unsigned int st);
	void setTemperature(double temp);
	void setCollisionFrequency(double cf);
	void setIsThermostatOn(bool isT);
	void setIs2D(bool is2D);
	void setChosenMaterial(Material mat);

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
};