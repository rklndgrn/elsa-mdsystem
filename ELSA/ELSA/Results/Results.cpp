#include "Results.h"

using namespace std;

Results::Results(double simulationTime, double timeStep, unsigned int numberOfParticles)
{
	_arrayLength = (int) round(simulationTime/timeStep);
	_numberOfAtoms = numberOfParticles;

	_cohesiveEnergy = new double[_arrayLength];
	_debyeTemperature = new double[_arrayLength];
	_diffusionConstant = new double[_arrayLength];
	_internalPressure = new double[_arrayLength];
	_kineticEnergy = new double[_arrayLength];
	_meanSquareDisplacement = new double[_arrayLength];
	_potentialEnergy = new double[_arrayLength];
	_specificHeat = new double[_arrayLength];
	_temperature = new double[_arrayLength];
	_totalEnergy = new double[_arrayLength];

	/*
	_momentum = new double*[_arrayLength];
	for (unsigned int i = 0; i < _arrayLength; i++)
	{
		_momentum[i] = new double[3];
		_momentum[i][0] = 0;
		_momentum[i][1] = 0;
		_momentum[i][2] = 0;
	}
	*/

	//Create a 3D array for positions for each atom over time.
	_positions = new double**[_arrayLength];
	for (unsigned int i = 0; i < _arrayLength; i++)
	{
		_positions[i] = new double*[numberOfParticles];
		for (unsigned int j = 0; j < numberOfParticles; j++)
		{
			_positions[i][j] = new double[3];
		}
	}
}

//For testing the 3D array. Delete if unnecessary.
void Results::printPositions()
{
	for (unsigned int t = 0; t < _arrayLength; t++)
	{
		cout << "At time step " << t << ":" << endl;
		for (unsigned int n = 0; n < _numberOfAtoms; n++)
		{
			cout << "   Atom " << n << " has position: (" << _positions[t][n][0] << ", " << _positions[t][n][1] << ", " << _positions[t][n][2] << ")!" << endl;
		}
		cout << endl;
	}
}

/*
array<double, 3> Results::getMomentumVector(int index)
{
	array<double, 3> p;
	p[0] = _momentum[index][0];
	p[1] = _momentum[index][1];
	p[2] = _momentum[index][2];

	return p;
}

void Results::addToMomentum(double px, double py, double pz, int t)
{
	_momentum[t][0] += px;
	_momentum[t][1] += py;
	_momentum[t][2] += pz;
}
*/

//getters
double** Results::getCohesiveEnergy()
{
	return &_cohesiveEnergy;
}

double** Results::getDebyeTemperature()
{
	return &_debyeTemperature;
}

double** Results::getDiffusionConstant()
{
	return &_diffusionConstant;
}

double** Results::getInternalPressure()
{
	return &_internalPressure;
}

double** Results::getKineticEnergy()
{
	return &_kineticEnergy;
}

double** Results::getMeanSquareDisplacement()
{
	return &_meanSquareDisplacement;
}

/*
double*** Results::getMomentum()
{
	return &_momentum;
}
*/

double**** Results::getPositions()
{
	return &_positions;
}

double** Results::getPotentialEnergy()
{
	return &_potentialEnergy;
}

double** Results::getSpecificHeat()
{
	return &_specificHeat;
}

double** Results::getTemperature()
{
	return &_temperature;
}

double** Results::getTotalEnergy()
{
	return &_totalEnergy;
}

//setters
void Results::setCohesiveEnergy(int index)
{
	_cohesiveEnergy[index] = _potentialEnergy[index]/_numberOfAtoms;
}

void Results::setDebyeTemperature(double value, int index)
{
	_debyeTemperature[index] = value;
}

void Results::setDiffusionConstant(double value, int index)
{
	_diffusionConstant[index] = value;
}

void Results::setInternalPressure(double value, int index)
{
	_internalPressure[index] = value;
}

void Results::setKineticEnergy(double value, int index)
{
	_kineticEnergy[index] = value;
}

void Results::setMeanSquareDisplacement(double value, int index)
{
	_meanSquareDisplacement[index] = value;
}

/*
void Results::setMomentum(double px, double py, double pz, int t)
{
	_momentum[t][0] = px;
	_momentum[t][1] = py;
	_momentum[t][2] = pz;
}
*/
void Results::setPositions(double x, double y, double z, int t, int n)
{
	_positions[t][n][0] = x;
	_positions[t][n][1] = y;
	_positions[t][n][2] = z;
}

void Results::setPotentialEnergy(double value, int index)
{
	_potentialEnergy[index] = value;
}

void Results::setSpecificHeat(double value, int index)
{
	_specificHeat[index] = value;
}

void Results::setTemperature(double value, int index)
{
	_temperature[index] = value;
}

void Results::setTotalEnergy(int index)
{
	_totalEnergy[index] = _potentialEnergy[index] + _kineticEnergy[index];
}