#include "Results.h"

using namespace std;

Results::Results(int simulationTime, int timeStep, int numberOfParticles)
{
	arrayLength = (int) floor((double) simulationTime/timeStep);
	numberOfAtoms = numberOfParticles;

	cohesiveEnergy = new double[arrayLength];
	debyeTemperature = new double[arrayLength];
	diffusionConstant = new double[arrayLength];
	internalPressure = new double[arrayLength];
	meanSquareDisplacement = new double[arrayLength];
	//positions = new double[arrayLength][numberOfParticles][3];
	specificHeat = new double[arrayLength];
	temperature = new double[arrayLength];
	totalEnergy = new double[arrayLength];

	positions = new double**[arrayLength];
	for (unsigned int i = 0; i < arrayLength; i++)
	{
		positions[i] = new double*[numberOfParticles];
		for (int j = 0; j < numberOfParticles; j++)
		{
			positions[i][j] = new double[3];
		}
	}
}

double** Results::getCohesiveEnergy()
{
	return &cohesiveEnergy;
}

double** Results::getDebyeTemperature()
{
	return &debyeTemperature;
}

double** Results::getDiffusionConstant()
{
	return &diffusionConstant;
}

double** Results::getInternalPressure()
{
	return &internalPressure;
}

double** Results::getKineticEnergy()
{
	return &kineticEnergy;
}

double** Results::getMeanSquareDisplacement()
{
	return &meanSquareDisplacement;
}

double**** Results::getPositions()
{
	return &positions;
}

double** Results::getPotentialEnergy()
{
	return &potentialEnergy;
}

double** Results::getSpecificHeat()
{
	return &specificHeat;
}

double** Results::getTemperature()
{
	return &temperature;
}

double** Results::getTotalEnergy()
{
	return &totalEnergy;
}

void Results::printPositions()
{
	for (unsigned int t = 0; t < arrayLength; t++)
	{
		cout << "At time step " << t << ":" << endl;
		for (unsigned int n = 0; n < numberOfAtoms; n++)
		{
			cout << "   Atom " << n << " has position: (" << positions[t][n][0] << ", " << positions[t][n][1] << ", " << positions[t][n][2] << ")!" << endl;
		}
		cout << endl;
	}
}

void Results::setCohesiveEnergy(double value, int index)
{
	cohesiveEnergy[index] = value;
}

void Results::setDebyeTemperature(double value, int index)
{
	debyeTemperature[index] = value;
}

void Results::setDiffusionConstant(double value, int index)
{
	diffusionConstant[index] = value;
}

void Results::setInternalPressure(double value, int index)
{
	internalPressure[index] = value;
}

void Results::setKineticEnergy(double value, int index)
{
	kineticEnergy[index] = value;
}

void Results::setMeanSquareDisplacement(double value, int index)
{
	meanSquareDisplacement[index] = value;
}

void Results::setPositions(double x, double y, double z, int t, int n)
{
	positions[t][n][0] = x;
	positions[t][n][1] = y;
	positions[t][n][2] = z;
}

void Results::setPotentialEnergy(double energy, int index)
{
	potentialEnergy[index];
}

void Results::setSpecificHeat(double value, int index)
{
	specificHeat[index] = value;
}

void Results::setTemperature(double value, int index)
{
	temperature[index] = value;
}

void Results::setTotalEnergy(double value, int index)
{
	totalEnergy[index] = value;
}