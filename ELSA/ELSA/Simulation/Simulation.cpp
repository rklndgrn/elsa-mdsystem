#include "Simulation.h"

using namespace std;

Simulation::Simulation(Material mat) :
	_mat{mat}
{};

double Simulation::calcCohesiveEnergy(double potentialEnergy, double numberOfAtoms)
{
	return potentialEnergy/numberOfAtoms;
}

double Simulation::calcForce(double dist) const
{
	return 24 * _mat.getEpsilon() / _mat.getSigma() *
		(
			2 * pow((_mat.getSigma() / dist), 13) -
			pow((_mat.getSigma() / dist), 7)
			);
}

double Simulation::calcLJPotential(double dist) const
{
	return 4 * _mat.getEpsilon() *
		(
			pow((_mat.getSigma() / dist), 12) -
			pow((_mat.getSigma() / dist), 6)
			);
};

double Simulation::calcKineticEnergy(double vx, double vy, double vz)
{
	double m = _mat.getMass();

	return 0.5*m*(pow(vx, 2) + pow(vy, 2) + pow(vz, 2));
}

double Simulation::calcTemperature(double K, double kB, double N)
{
	if (N > 0)
	{
		return (2 * K) / (3 * kB*N);
	}
	else
	{
		return 0;
	}
}

double Simulation::calcTotalEnergy(double potentialEnergy, double kineticEnergy)
{
	return potentialEnergy + kineticEnergy;
}

//Function for assigning a new Gaussian velocity if an Anderson thermostat is to be used. T is the variance/temperature. 
array<double, 3> Simulation::generateGaussianVelocity(double T)
{
	random_device rdX;
	random_device rdY;
	random_device rdZ;
	mt19937 generatorX(rdX());
	mt19937 generatorY(rdY());
	mt19937 generatorZ(rdZ());
	normal_distribution<double> distributionX(0, sqrt(T));
	normal_distribution<double> distributionY(0, sqrt(T));
	normal_distribution<double> distributionZ(0, sqrt(T));

	array<double, 3> v = {distributionX(generatorX), distributionY(generatorY), distributionZ(generatorZ)};
	return v;
}

array<double, 3> Simulation::calcAcceleration(double fx, double fy, double fz)
{
	double m = _mat.getMass();
	array<double, 3> a = {m*fx, m*fy, m*fz};
	return a;
}

array<double, 4> Simulation::calcDistance(Atom* a1, Atom* a2) const
{
	double rx, ry, rz, r;
	rx = a1->getPosX() - a2->getPosX();
	ry = a1->getPosY() - a2->getPosY();
	rz = a1->getPosZ() - a2->getPosZ();
	r = sqrt(pow(rx, 2) + pow(ry, 2) + pow(rz, 2));
	rx = rx / r;
	ry = ry / r;
	rz = rz / r;
	array<double, 4> temp = { r, rx, ry, rz };
	return temp;
}

//Function to calculate the position using the Velocity Verlet Algorithm.
array<double, 3> Simulation::calcPosition(std::array<double, 3> r, std::array<double, 3> v, std::array<double, 3> a, double timeStep)
{
	double rX = r[0] + timeStep * v[0] + 0.5*pow(timeStep, 2)*a[0];
	double rY = r[1] + timeStep * v[1] + 0.5*pow(timeStep, 2)*a[1];
	double rZ = r[2] + timeStep * v[2] + 0.5*pow(timeStep, 2)*a[2];

	array<double, 3> newR = { rX, rY, rZ };
	return newR;
}

//Function to calculate the velocity using the Velocity Verlet Algorithm.
array<double, 3> Simulation::calcVelocity(std::array<double, 3> v, std::array<double, 3> a, double timeStep)
{
	double halfStepVX = v[0] + 0.5*timeStep*a[0];
	double halfStepVY = v[1] + 0.5*timeStep*a[1];
	double halfStepVZ = v[2] + 0.5*timeStep*a[2];

	double newVX = halfStepVX + 0.5*timeStep*a[0];
	double newVY = halfStepVY + 0.5*timeStep*a[1];
	double newVZ = halfStepVZ + 0.5*timeStep*a[2];

	array<double, 3> newV = {newVX, newVY, newVZ};

	return newV;
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

