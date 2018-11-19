/*
	File name: parameters.cpp
	Date: 2018-11-06
	This file contains the definition of the class Parameters
*/

#include "Parameters.h"

using namespace std;

Parameters::Parameters() :
	_numberOfAtoms{0},
	_timeStep{0},
	_simulationTime{0},
	_temperature{0},
	_collisionFrequency{0},
	_isThermostatOn{false},
	_is2D{false}
{}

Parameters::Parameters(
	double timeStep,
	double simulationTime,
	unsigned int numberOfUnitCellsX,
	unsigned int numberOfUnitCellsY,
	unsigned int numberOfUnitCellsZ,
	double temperature,
	double collisionFrequency,
	bool isThermostatOn,
	bool is2D,
	Material chosenMaterial) : 
	_timeStep{ timeStep },
	_simulationTime{ simulationTime },
	_numberOfUnitCellsX{numberOfUnitCellsX},
	_numberOfUnitCellsY{ numberOfUnitCellsY },
	_numberOfUnitCellsZ{ numberOfUnitCellsZ },
	_temperature{ temperature },
	_collisionFrequency{ collisionFrequency },
	_isThermostatOn{ isThermostatOn },
	_is2D{ is2D },
	_chosenMaterial{ chosenMaterial }
{
	if (chosenMaterial.getCrystalStructure() == "fcc")
	{
		_numberOfAtoms = 4 * numberOfUnitCellsX*numberOfUnitCellsY*numberOfUnitCellsZ;
	}
	else if (chosenMaterial.getCrystalStructure() == "sc")
	{
		_numberOfAtoms = numberOfUnitCellsX * numberOfUnitCellsY*numberOfUnitCellsZ;
	}

	//Length of simulation box
	_lengthX = getNumberOfUnitCellsX()*getChosenMaterial().getLatticeConstant();
	_lengthY = getNumberOfUnitCellsY()*getChosenMaterial().getLatticeConstant();
	_lengthZ = getNumberOfUnitCellsZ()*getChosenMaterial().getLatticeConstant();

	//Number of cells in each dimension of the simulation box
	_numberOfCellsI = (unsigned int)ceil(
		_numberOfUnitCellsX * _chosenMaterial.getLatticeConstant() / 
		_chosenMaterial.getCellSize());
	_numberOfCellsJ = (unsigned int)ceil(
		_numberOfUnitCellsY * _chosenMaterial.getLatticeConstant() /
		_chosenMaterial.getCellSize());
	_numberOfCellsK = (unsigned int)ceil(
		_numberOfUnitCellsZ * _chosenMaterial.getLatticeConstant() /
		_chosenMaterial.getCellSize());


}



//Getters
unsigned int Parameters::getNumberOfAtoms() const
{
	return _numberOfAtoms;
}

unsigned int Parameters::getNumberOfCellsI() const
{
	return _numberOfCellsI;
}

unsigned int Parameters::getNumberOfCellsJ() const
{
	return _numberOfCellsJ;
}

unsigned int Parameters::getNumberOfCellsK() const
{
	return _numberOfCellsK;
}

unsigned int Parameters::getNumberOfUnitCellsX() const
{
	return _numberOfUnitCellsX;
}

unsigned int Parameters::getNumberOfUnitCellsY() const
{
	return _numberOfUnitCellsY;
}

unsigned int Parameters::getNumberOfUnitCellsZ() const
{
	return _numberOfUnitCellsZ;
}

double Parameters::getBoltzmann() const
{
	return _boltzmann;
}

double Parameters::getCollisionFrequency() const
{
	return _collisionFrequency;
}

double Parameters::getLengthX() const
{
	return _lengthX;
}

double Parameters::getLengthY() const
{
	return _lengthY;
}

double Parameters::getLengthZ() const
{
	return _lengthZ;
}

double Parameters::getNormalizedPlanck() const
{
	return _normalizedPlanck;
}

double Parameters::getSimulationTime() const
{
	return _simulationTime;
}

double Parameters::getTemperature() const
{
	return _temperature;
}

double Parameters::getTimeStep() const
{
	return _timeStep;
}

bool Parameters::getIs2D() const
{
	return _is2D;
}

bool Parameters::getIsThermostatOn() const
{
	return _isThermostatOn;
}

Material Parameters::getChosenMaterial() const
{
	return _chosenMaterial;
}


//Setters.

void Parameters::setChosenMaterial(Material mat)
{
	_chosenMaterial = mat;
}

void Parameters::setCollisionFrequency(double cf)
{
	_collisionFrequency = cf;
}

void Parameters::setIs2D(bool is2)
{
	_is2D = is2;
}

void Parameters::setIsThermostatOn(bool isT)
{
	_isThermostatOn = isT;
}

void Parameters::setNumberOfAtoms(unsigned int noa)
{
	_numberOfAtoms = noa;
}

void Parameters::setSimulationTime(double st)
{
	_simulationTime = st;
}


void Parameters::setTemperature(double temp)
{
	_temperature = temp;
}

void Parameters::setTimeStep(double ts)
{
	_timeStep = ts;
}




