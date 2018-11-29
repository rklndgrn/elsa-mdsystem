#ifndef RESULTS_H
#define RESULTS_H

#include <array>
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
	//ICE:
	/*
	std::vector<double> _cohesiveEnergy;
	std::vector<double> _debyeTemperature;
	std::vector<double> _diffusionConstant;
	std::vector<double> _internalPressure;
	std::vector<double> _kineticEnergy;
	std::vector<double> _meanSquareDisplacement;
	std::vector<std::vector<std::vector<double>>> _positions;
	std::vector<double> _potentialEnergy;
	std::vector<double> _specificHeat;
	std::vector<double> _temperature;
	std::vector<double> _totalEnergy;
	*/

public:
	Results() = default;
	Results(double, double, unsigned int);
	~Results();

	void printPositions();

	//Getters.
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

	//Setters.
	void setCohesiveEnergy(int);
	void setDebyeTemperature(double, int);
	void setDiffusionConstant(double, int);
	void setInternalPressure(double, int);
	void setKineticEnergy(double, int);
	void setMeanSquareDisplacement(double, int);
	void setPositions(double, double, double, int, int);
	void setPotentialEnergy(double, int);
	void setSpecificHeat(double, int);
	void setTemperature(double, int);
	void setTotalEnergy(int);
};

#endif