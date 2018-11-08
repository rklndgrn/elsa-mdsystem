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
	unsigned int timeStep,
	unsigned int simulationTime,
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
		_numberOfAtoms = numberOfUnitCellsX*numberOfUnitCellsY*numberOfUnitCellsZ;
	}

	_numberOfCellsI = (unsigned int)ceil(_numberOfUnitCellsX * _chosenMaterial.getLatticeConstant() / _chosenMaterial.getCellSize());
	_numberOfCellsJ = (unsigned int)ceil(_numberOfUnitCellsY * _chosenMaterial.getLatticeConstant() / _chosenMaterial.getCellSize());
	_numberOfCellsK = (unsigned int)ceil(_numberOfUnitCellsZ * _chosenMaterial.getLatticeConstant() / _chosenMaterial.getCellSize());

}

Parameters::~Parameters()
{}

double Parameters::getBoltzmann() const
{
	return _boltzmann;
}

unsigned int Parameters::getNumberOfAtoms() const
{
	return _numberOfAtoms;
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

unsigned int Parameters::getTimeStep() const
{
	return _timeStep;
}

unsigned int Parameters::getSimulationTime() const
{
	return _simulationTime;
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

double Parameters::getTemperature() const
{
	return _temperature;
}

double Parameters::getCollisionFrequency() const
{
	return _collisionFrequency;
}

bool Parameters::getIsThermostatOn() const
{
	return _isThermostatOn;
}

bool Parameters::getIs2D() const
{
	return _is2D;
}

Material Parameters::getChosenMaterial() const
{
	return _chosenMaterial;
}

void Parameters::setNumberOfAtoms(unsigned int noa)
{
	_numberOfAtoms = noa;
}

void Parameters::setTimeStep(unsigned int ts)
{
	_timeStep = ts;
}

void Parameters::setSimulationTime(unsigned int st)
{
	_simulationTime = st;
}

void Parameters::setNumberOfUnitCellsX(unsigned int x)
{
	_numberOfUnitCellsX = x;
}

void Parameters::setNumberOfUnitCellsY(unsigned int y)
{
	_numberOfUnitCellsY = y;
}

void Parameters::setNumberOfUnitCellsZ(unsigned int z)
{
	_numberOfUnitCellsZ = z;
}

void Parameters::setNumberOfCellsI(unsigned int i)
{
	_numberOfCellsI = i;
}

void Parameters::setNumberOfCellsJ(unsigned int j)
{
	_numberOfCellsJ = j;
}

void Parameters::setNumberOfCellsK(unsigned int k)
{
	_numberOfCellsK = k;
}

void Parameters::setTemperature(double temp)
{
	_temperature = temp;
}

void Parameters::setCollisionFrequency(double cf)
{
	_collisionFrequency = cf;
}

void Parameters::setIsThermostatOn(bool isT)
{
	_isThermostatOn = isT;
}

void Parameters::setIs2D(bool is2)
{
	_is2D = is2;
}

void Parameters::setChosenMaterial(Material mat)
{
	_chosenMaterial = mat;
}


