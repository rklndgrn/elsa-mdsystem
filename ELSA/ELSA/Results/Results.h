#ifndef RESULTS_H
#define RESULTS_H

#include <array>
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <utility>
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

	//void swap(Results &) noexcept;

public:
	Results() = default;
	Results(double, double, unsigned int);
	~Results();
	Results(Results const&); //Copy constructor.
	Results & operator = (Results const&); //Copy allocation.
	//Results(Results &&); //Move constructor.
	//Results & operator = (Results &&); //Move allocation.

	void printPositions();

	//Getters.
	unsigned int getArrayLength() const;
	unsigned int getNumberOfAtoms() const;

	double* getCohesiveEnergy() const;
	double* getDebyeTemperature() const;
	double* getDiffusionConstant() const;
	double* getInternalPressure() const;
	double* getKineticEnergy() const;
	double* getMeanSquareDisplacement() const;
	double*** getPositions() const;
	double* getPotentialEnergy() const;
	double* getSpecificHeat() const;
	double* getTemperature() const;
	double* getTotalEnergy() const;

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