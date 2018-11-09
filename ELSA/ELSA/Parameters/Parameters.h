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
	unsigned int _timeStep;
	unsigned int _simulationTime;
	unsigned int _numberOfUnitCellsX;
	unsigned int _numberOfUnitCellsY;
	unsigned int _numberOfUnitCellsZ;
	unsigned int _numberOfCellsI;
	unsigned int _numberOfCellsJ;
	unsigned int _numberOfCellsK;

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
		unsigned int timeStep,
		unsigned int simulationTime,
		unsigned int numberOfUnitCellsX,
		unsigned int numberOfUnitCellsY,
		unsigned int numberOfUnitCellsZ,
		double temperature,
		double collisionFrequency,
		bool isThermostatOn,
		bool is2D,
		Material chosenMaterial);


	bool getIs2D() const;
	bool getIsThermostatOn() const;

	double getCollisionFrequency() const;
	double getBoltzmann() const;
	double getTemperature() const;

	Material getChosenMaterial() const;

	unsigned int getNumberOfAtoms() const;
	unsigned int getTimeStep() const;
	unsigned int getSimulationTime() const;

	unsigned int getNumberOfUnitCellsX() const;
	unsigned int getNumberOfUnitCellsY() const;
	unsigned int getNumberOfUnitCellsZ() const;
	unsigned int getNumberOfCellsI() const;
	unsigned int getNumberOfCellsJ() const;
	unsigned int getNumberOfCellsK() const;

	double getLengthX() const;
	double getLengthY() const;
	double getLengthZ() const;

	void setNumberOfAtoms(unsigned int noa);
	void setTimeStep(unsigned int ts);
	void setSimulationTime(unsigned int st);
	void setNumberOfUnitCellsX(unsigned int);
	void setNumberOfUnitCellsY(unsigned int);
	void setNumberOfUnitCellsZ(unsigned int);
	void setNumberOfCellsI(unsigned int);
	void setNumberOfCellsJ(unsigned int);
	void setNumberOfCellsK(unsigned int);
	void setTemperature(double temp);
	void setCollisionFrequency(double cf);
	void setIsThermostatOn(bool isT);
	void setIs2D(bool is2D);
	void setChosenMaterial(Material mat);


};