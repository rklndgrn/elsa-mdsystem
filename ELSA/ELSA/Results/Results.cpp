#include "Results.h"
#include <math.h>

Results::Results(int simulationTime, int timeStep, int numberOfParticles)
{
	arrayLength = (int) floor((double) simulationTime/timeStep);

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