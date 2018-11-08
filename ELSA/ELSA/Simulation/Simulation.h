#pragma once
#include "../Material/Material.h"

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

	double calcCohesiveEnergy(double, double);
	double calcForce(double) const;
	double calcLJPotential(double dist) const;
	double calcKineticEnergy(double, double, double);
	double calcTemperature(double, double, double);
	double calcTotalEnergy(double, double);

	std::array<double, 3> calcAcceleration(double, double, double);
	std::array<double, 4> calcDistance(double, double, double, double, double, double) const;
	std::array<double, 3> calcPosition(std::array<double, 3>, std::array<double, 3>, std::array<double, 3>, double);
	std::array<double, 3> calcVelocity(std::array<double, 3>, std::array<double, 3>, double);
	std::array<double, 3> generateGaussianVelocity(double);
};