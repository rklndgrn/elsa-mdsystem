#ifndef RESULTS_H
#define RESULTS_H

#include <vector>

class Results
{
private:
	unsigned int arrayLength;
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
	Results(int, int, int);
	~Results() = default;

	double* getCohesiveEnergy() const;
	double* getDebyeTemperature() const;
	double* getDiffusionConstant() const;
	double* getInternalPressure() const;
	double* getMeanSquareDisplacement() const;
	double* getPositions() const;
	double* getSpecificHeat() const;
	double* getTemperature() const;
	double* getTotalEnergy() const;

	void setCohesiveEnergy(double, int);
	void setDebyeTemperature(double, int);
	void setDiffusionConstant(double, int);
	void setInternalPressure(double, int);
	void setMeanSquareDisplacement(double, int);
	void setPositions(double, double, double, int, int, int, int);
	void setSpecificHeat(double, int);
	void setTemperature(double, int);
	void setTotalEnergy(double, int);
};

#endif