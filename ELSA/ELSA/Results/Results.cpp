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

double**** Results::getPositions()
{
	return &positions;
}

void Results::prettyPrint()
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

void Results::setPositions(double x, double y, double z, int t, int n)
{
	positions[t][n][0] = x;
	positions[t][n][1] = y;
	positions[t][n][2] = z;
}