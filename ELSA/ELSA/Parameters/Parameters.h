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
	unsigned int _simulationTime;
	unsigned int _timeStep;

	double _boltzmann{ 1.3806485279E-23 };
	double _collisionFrequency;
	double _lengthX;
	double _lengthY;
	double _lengthZ;
	double _temperature;

	bool _isThermostatOn;
	bool _is2D;

	Material _chosenMaterial;

public:
	Parameters();
	~Parameters() = default;
	Parameters(
		unsigned int,
		unsigned int,
		unsigned int,
		unsigned int,
		unsigned int,
		double,
		double,
		bool,
		bool,
		Material);

	//getters
	unsigned int getNumberOfAtoms() const;
	unsigned int getNumberOfAtoms() const;
	unsigned int getNumberOfCellsI() const;
	unsigned int getNumberOfCellsJ() const;
	unsigned int getNumberOfCellsK() const;
	unsigned int getNumberOfUnitCellsX() const;
	unsigned int getNumberOfUnitCellsY() const;
	unsigned int getNumberOfUnitCellsZ() const;
	unsigned int getSimulationTime() const;
	unsigned int getTimeStep() const;

	double getBoltzmann() const;
	double getCollisionFrequency() const;
	double getLengthX() const;
	double getLengthY() const;
	double getLengthZ() const;
	double getTemperature() const;

	bool getIs2D() const;
	bool getIsThermostatOn() const;

	Material getChosenMaterial() const;

	//setters
	void setChosenMaterial(Material mat);
	void setCollisionFrequency(double cf);
	void setIsThermostatOn(bool isT);
	void setIs2D(bool is2D);
	void setNumberOfAtoms(unsigned int noa);
	void setSimulationTime(unsigned int st);
	void setTemperature(double temp);
	void setTimeStep(unsigned int ts);


};