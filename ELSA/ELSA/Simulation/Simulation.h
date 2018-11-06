#pragma once
#include "../Material/Material.h"

class Simulation
{
private:
	Material _mat;
public:
	double calcLJPot(double dist) const;
	double calcForce(double dist) const;

	Simulation();
	Simulation(Material mat);

};