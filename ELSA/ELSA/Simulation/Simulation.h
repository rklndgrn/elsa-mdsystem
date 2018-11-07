#pragma once
#include "../Material/Material.h"
#include <array>

class Simulation
{
private:
	Material _mat;
public:
	double calcLJPot(double dist) const;
	double calcForce(double) const;
	std::array<double, 4> calcDist(double, double, double, double, double, double) const;

	Simulation() = default;
	Simulation(Material mat);

	~Simulation() = default;
};