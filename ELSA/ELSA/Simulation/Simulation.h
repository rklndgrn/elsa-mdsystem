#pragma once
#include "../Material/Material.h"
#include "../Atom/Atom.h"

#include <array>
#include <cmath>
#include <random>

class Simulation
{
private:
	Material _mat;
public:
	Simulation() = default;
	Simulation(Material mat);

	~Simulation() = default;

	double calcDebyeTemperature(double, double, double, double, double);
	double calcForce(double) const;
	double calcKineticEnergy(double, double, double);
	double calcLJPotential(double dist) const;
	double calcMeanSquareDisplacement(double**, double**, unsigned int, double, double, double, bool);
	double calcSelfDiffusionCoefficient(double ***, double, double, double, unsigned int, double, double, double, bool);
	double calcSpecificHeat(unsigned int, double, double, double*);
	double calcTemperature(double, double, double);
	double calcTotalEnergy(double, double);

	std::array<double, 3> calcAcceleration(double, double, double);
	std::array<double, 4> calcDistance(Atom*, Atom*, double, double, double, bool) const;
	std::array<double, 4> calcDistance(std::array<double, 3>, std::array<double, 3>, double, double, double, bool) const;
	std::array<double, 3> calcPosition(std::array<double, 3>, std::array<double, 3>, std::array<double, 3>, double);
	std::array<double, 3> calcVelocity(std::array<double, 3>, std::array<double, 3>, std::array<double, 3>, double);
	std::array<double, 3> generateGaussianVelocity(double);

};