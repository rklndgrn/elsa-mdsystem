#pragma once
#include "../Material/Material.h"

class Simulation
{
private:
	Material _mat;
public:
	double calcLJPot(double dist) const;
	double calcForce(double dist) const;
	double calcDist(double, double, double, double, double, double) const;

	Simulation();
	Simulation(Material mat);

};