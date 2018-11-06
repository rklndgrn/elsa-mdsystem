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

double** getCohesiveEnergy()
{
	return &cohesiveEnergy;
}

double** getDebyeTemperature()
{
	return &debyeTemperature;
}

double** getDiffusionConstant()
{
	return &diffusionConstant;
}

double** getInternalPressure()
{
	return &internalPressure;
}

double** getMeanSquareDisplacement()
{
	return &meanSquareDisplacement;
}

double**** Results::getPositions()
{
	return &positions;
}

double** getSpecificHeat()
{
	return &specificHeat;
}

double** getTemperature()
{
	return &temperature;
}

double** getTotalEnergy()
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

void setCohesiveEnergy(double value, int index)
{
	cohesiveEnergy[index] = value;
}

void setDebyeTemperature(double value, int index)
{
	debyeTemperature[index] = value;
}

void setDiffusionConstant(double value, int index)
{
	diffusionConstant[index] = value;
}

void setInternalPressure(double value, int index)
{
	internalPressure[index] = value;
}

void setMeanSquareDisplacement(double value, int index)
{
	meanSquareDisplacement[index] = value;
}

void Results::setPositions(double x, double y, double z, int t, int n)
{
	positions[t][n][0] = x;
	positions[t][n][1] = y;
	positions[t][n][2] = z;
}

void setSpecificHeat(double value, int index)
{
	specificHeat[index] = value;
}

void setTemperature(double value, int index)
{
	temperature[index] = value;
}

void setTotalEnergy(double value, int index)
{
	totalEnergy[index] = value;
}