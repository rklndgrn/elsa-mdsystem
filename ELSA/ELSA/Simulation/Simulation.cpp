#include "Simulation.h"
#include <cmath>

Simulation::Simulation() :
	_mat{ Material() }
{};

Simulation::Simulation(Material mat) :
	_mat{mat}
{};


double Simulation::calcLJPot(double dist) const
{
	return 4 * _mat.getEpsilon() *
		(
			pow((_mat.getSigma() / dist),  12) - 
			pow((_mat.getSigma() / dist), 6)
		);
};

double Simulation::calcForce(double dist) const
{
	return 24 * _mat.getEpsilon() / _mat.getSigma() *
		(
			2 * pow((_mat.getSigma() / dist), 13) -
			pow((_mat.getSigma() / dist), 7)
			);
}

double Simulation::calcDist(double x1, double y1, double z1, double x2, double y2, double z2) const
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
}