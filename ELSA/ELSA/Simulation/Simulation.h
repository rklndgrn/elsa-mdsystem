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
	double calcLJPotential(double dist) const;
	double calcForce(double) const;
	std::array<double, 3> calcAcceleration(double, double, double);
	std::array<double, 4> calcDistance(double, double, double, double, double, double) const;
	double calcKineticEnergy(double, double, double);
	std::array<double, 3> calcPosition(std::array<double, 3>, std::array<double, 3>, std::array<double, 3>, double);
	double calcTemperature(double, double, double);
	std::array<double, 3> calcVelocity(std::array<double, 3>, std::array<double, 3>, double);
	std::array<double, 3> generateGaussianVelocity(double);

	Simulation() = default;
	Simulation(Material mat);

	~Simulation() = default;
};