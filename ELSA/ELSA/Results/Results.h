#ifndef RESULTS_H
#define RESULTS_H

#include <iostream>
#include <math.h>
#include <vector>

class Results
{
private:
	unsigned int arrayLength;
	unsigned int numberOfAtoms;
	double *cohesiveEnergy;
	double *debyeTemperature;
	double *diffusionConstant;
	double *internalPressure;
	double *meanSquareDisplacement;
	double ***positions;
	double *specificHeat;
	double *temperature;
	double *totalEnergy;

public:
	Results() = default;
	Results(int, int, int);
	~Results() = default;

	double** getCohesiveEnergy();
	double** getDebyeTemperature();
	double** getDiffusionConstant();
	double** getInternalPressure();
	double** getMeanSquareDisplacement();
	double**** getPositions();
	double** getSpecificHeat();
	double** getTemperature();
	double** getTotalEnergy();

	void printPositions();

	void setCohesiveEnergy(double, int);
	void setDebyeTemperature(double, int);
	void setDiffusionConstant(double, int);
	void setInternalPressure(double, int);
	void setMeanSquareDisplacement(double, int);
	void setPositions(double, double, double, int, int);
	void setSpecificHeat(double, int);
	void setTemperature(double, int);
	void setTotalEnergy(double, int);
};

#endif