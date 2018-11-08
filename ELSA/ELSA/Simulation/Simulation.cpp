#include "Simulation.h"
#include <iostream>

using namespace std;

Simulation::Simulation(Material mat) :
	_mat{mat}
{};

double Simulation::calcCohesiveEnergy(double potentialEnergy, double numberOfAtoms)
{
	return potentialEnergy/numberOfAtoms;
}

double Simulation::calcDebyeTemperature(double hBar, double T, double m, double kB, double MSD)
{
	return hBar * sqrt((3 * T) / (m*kB*MSD));
}

double Simulation::calcForce(double dist) const
{
	return 24 * _mat.getEpsilon() / _mat.getSigma() *
		(
			2 * pow((_mat.getSigma() / dist), 13) -
			pow((_mat.getSigma() / dist), 7)
			);
}

double Simulation::calcKineticEnergy(double vx, double vy, double vz)
{
	double m = _mat.getMass();

	return 0.5*m*(pow(vx, 2) + pow(vy, 2) + pow(vz, 2));
}

double Simulation::calcLJPotential(double dist) const
{
	return 4 * _mat.getEpsilon() *
		(
			pow((_mat.getSigma() / dist), 12) -
			pow((_mat.getSigma() / dist), 6)
			);
};

double Simulation::calcMeanSquareDisplacement(double** currentPositionArray, double** initPositionArray, unsigned int numberOfAtoms)
{
	array<double, 3> currPos;
	array<double, 3> initPos;

	double sum{0};
	for (unsigned int i{ 0 }; i < numberOfAtoms; i++)
	{
		for (int j{ 0 }; j < 3; j++)
		{
			currPos[j] = currentPositionArray[i][j];
			initPos[j] = initPositionArray[i][j];
		}

		sum += pow(currPos[0] - initPos[0], 2) + pow(currPos[1] - initPos[1], 2) + pow(currPos[2] - initPos[2], 2);

		cout << "currPos is " << currPos[0] << ", " << currPos[1] << ", " << currPos[2] << "!" << endl;
		cout << "initPos is " << initPos[0] << ", " << initPos[1] << ", " << initPos[2] << "!" << endl;
	}
	return sum/((double) numberOfAtoms);
}

double Simulation::calcSpecificHeat(unsigned int numberOfAtoms, double kB, double numberOfTimeSteps, double* tempArray)
{
	double expT{ 0 }, expT2{0};
	for (int i{ 0 }; i < numberOfTimeSteps; i++)
	{
		expT += tempArray[i];
		expT2 += pow(tempArray[i], 2);
	}
	expT /= numberOfTimeSteps;
	expT2 /= numberOfTimeSteps;

	double tmp = expT2 - pow(expT, 2);
	tmp *= ((2*numberOfAtoms)/(3*pow(expT, 2)));
	tmp = 1 - tmp;
	tmp = 1/tmp;
	tmp *= 2 / (3 * numberOfAtoms*kB);

	return tmp;
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

