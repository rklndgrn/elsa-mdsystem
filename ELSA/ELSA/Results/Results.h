#ifndef RESULTS_H
#define RESULTS_H

#include <iostream>
#include <math.h>
#include <vector>

class Results
{
private:
	unsigned int _arrayLength;
	unsigned int _numberOfAtoms;

	double* _cohesiveEnergy;
	double* _debyeTemperature;
	double* _diffusionConstant;
	double* _internalPressure;
	double* _kineticEnergy;
	double* _meanSquareDisplacement;
	double*** _positions;
	double* _potentialEnergy;
	double* _specificHeat;
	double* _temperature;
	double* _totalEnergy;

public:
	Results() = default;
	Results(int, int, int);
	~Results() = default;

	void printPositions();

	//getters
	double** getCohesiveEnergy();
	double** getDebyeTemperature();
	double** getDiffusionConstant();
	double** getInternalPressure();
	double** getKineticEnergy();
	double** getMeanSquareDisplacement();
	double**** getPositions();
	double** getPotentialEnergy();
	double** getSpecificHeat();
	double** getTemperature();
	double** getTotalEnergy();

	//setters
	void setCohesiveEnergy(double, int);
	void setDebyeTemperature(double, int);
	void setDiffusionConstant(double, int);
	void setInternalPressure(double, int);
	void setKineticEnergy(double, int);
	void setMeanSquareDisplacement(double, int);
	void setPositions(double, double, double, int, int);
	void setPotentialEnergy(double, int);
	void setSpecificHeat(double, int);
	void setTemperature(double, int);
	void setTotalEnergy(double, int);
};

#endif