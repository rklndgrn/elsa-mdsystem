#include "Simulation.h"
#include <cmath>

using namespace std;

Simulation::Simulation(Material mat) :
	_mat{mat}
{};

std::array<double, 3> Simulation::calcAcceleration(std::array<double, 4> force)
{
	double m = _mat.getMass();
	array<double, 3> a = {m*force[1], m*force[2], m*force[3]};
	return a;
}

double Simulation::calcLJPotential(double dist) const
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

array<double, 4> Simulation::calcDistance(double x1, double y1, double z1, double x2, double y2, double z2) const
{
	double rx, ry, rz, r;
	rx = x1 - x2;
	ry = y1 - y2;
	rz = z1 - z2;
	r = sqrt(pow(rx, 2) + pow(ry, 2) + pow(rz, 2));
	rx = rx / r;
	ry = ry / r;
	rz = rz / r;
	array<double, 4> temp = { r, rx, ry, rz };
	return temp;
}

array<double, 3> Simulation::calcPosition(std::array<double, 3> r, std::array<double, 3> v, std::array<double, 3> a, double timeStep)
{
	double rX = r[0] + timeStep * v[0] + 0.5*pow(timeStep, 2)*a[0];
	double rY = r[1] + timeStep * v[1] + 0.5*pow(timeStep, 2)*a[1];
	double rZ = r[2] + timeStep * v[2] + 0.5*pow(timeStep, 2)*a[2];

	array<double, 3> newR = { rX, rY, rZ };
	return newR;
}
