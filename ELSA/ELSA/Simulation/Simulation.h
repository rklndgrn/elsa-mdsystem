#pragma once
#include "../Material/Material.h"
#include "../Atom/Atom.h"

#include <array>
#include <cmath>

class Simulation
{
private:
	Material _mat;
public:
	double calcLJPotential(double dist) const;
	double calcForce(double) const;
	std::array<double, 3> calcAcceleration(std::array<double, 4>);
	std::array<double, 4> calcDistance(double, double, double, double, double, double) const;
	std::array<double, 4> calcDistance(Atom*, Atom*) const;
	std::array<double, 4> calcDistanceWithBoundaryCondition(Atom*, Atom*, double, double, double) const;
	std::array<double, 3> calcPosition(std::array<double, 3>, std::array<double, 3>, std::array<double, 3>, double);
	std::array<double, 3> calcVelocity(std::array<double, 3>, std::array<double, 3>, double);

	Simulation() = default;
	Simulation(Material mat);

	~Simulation() = default;
};