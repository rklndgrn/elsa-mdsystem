#include "Results.h"

using namespace std;

Results::Results(double simulationTime, double timeStep, unsigned int numberOfParticles)
{
	_vectorLength = (int) round(simulationTime/timeStep);
	_numberOfAtoms = numberOfParticles;

	/*
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
	*/
	_cohesiveEnergy.resize(_vectorLength);
	_debyeTemperature.resize(_vectorLength);
	_diffusionConstant.resize(_vectorLength);
	_internalPressure.resize(_vectorLength);
	_kineticEnergy.resize(_vectorLength);
	_meanSquareDisplacement.resize(_vectorLength);
	_potentialEnergy.resize(_vectorLength);
	_specificHeat.resize(_vectorLength);;
	_temperature.resize(_vectorLength);
	_totalEnergy.resize(_vectorLength);

	//Create a 3D array for positions for each atom over time.
	_positions.resize(_vectorLength);
	for (unsigned int i = 0; i < _vectorLength; i++)
	{
		_positions[i].resize(numberOfParticles);
		for (int j = 0; j < numberOfParticles; j++)
		{
			_positions[i][j][0] = 0;
			_positions[i][j][1] = 0;
			_positions[i][j][2] = 0;
		}
	}

}

//getters
std::vector<double>*  Results::getCohesiveEnergy()
{
	return &_cohesiveEnergy;
}

std::vector<double>*  Results::getDebyeTemperature()
{
	return &_debyeTemperature;
}

std::vector<double>*  Results::getDiffusionConstant()
{
	return &_diffusionConstant;
}

std::vector<double>*  Results::getInternalPressure()
{
	return &_internalPressure;
}

std::vector<double>*  Results::getKineticEnergy()
{
	return &_kineticEnergy;
}

std::vector<double>*  Results::getMeanSquareDisplacement()
{
	return &_meanSquareDisplacement;
}

std::vector<std::vector<std::array<double, 3>>>* Results::getPositions()
{
	return &_positions;
}

std::vector<double>*  Results::getPotentialEnergy()
{
	return &_potentialEnergy;
}

std::vector<double>*  Results::getSpecificHeat()
{
	return &_specificHeat;
}

std::vector<double>*  Results::getTemperature()
{
	return &_temperature;
}

std::vector<double>*  Results::getTotalEnergy()
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