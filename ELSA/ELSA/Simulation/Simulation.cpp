#include "Simulation.h"
#include <iostream>

using namespace std;

Simulation::Simulation(Material mat) :
	_mat{mat}
{};


//Get debye temperature
double Simulation::calcDebyeTemperature(double hBar, double T, double m, double kB, double MSD)
{
	if (MSD == 0)
	{
		return 0;
	}
	else
	{
		return hBar * sqrt((3 * T) / (m*kB*MSD));

	}
}

//Norm of force contribution from an atom pair 
//calculated by the gradient of the Lennard-Jones potential
double Simulation::calcForce(double dist) const
{
	return 24 * _mat.getEpsilon() / _mat.getSigma() *
		(2 * pow((_mat.getSigma() / dist), 13) -
			pow((_mat.getSigma() / dist), 7));
}

//Get kinetic energy for one atom
double Simulation::calcKineticEnergy(double vx, double vy, double vz)
{
	double m = _mat.getMass();

	return 0.5*m*(pow(vx, 2) + pow(vy, 2) + pow(vz, 2));
}

//Calculate potential contribution from an atom pair with Lennard-Jones potential
double Simulation::calcLJPotential(double dist) const
{
	return 4 * _mat.getEpsilon() *
		(pow((_mat.getSigma() / dist), 12) -
		pow((_mat.getSigma() / dist), 6));
}

//Calculate the average deviation from the atoms' initial positions.
double Simulation::calcMeanSquareDisplacement(double** currentPositionArray, double** initPositionArray, unsigned int numberOfAtoms, double lengthX, double lengthY, double lengthZ, bool is2D)
{
	array<double, 3> currPos;
	array<double, 3> initPos;
	array<double, 4> r;

	double sum{ 0 };
	for (int i{ 0 }; i < (int) numberOfAtoms; i++)
	{
		for (int j{ 0 }; j < 3; j++)
		{
			currPos[j] = currentPositionArray[i][j];
			initPos[j] = initPositionArray[i][j];
		}

		r = calcDistance(currPos, initPos, lengthX, lengthY, lengthZ, is2D);
		sum += pow(r[0], 2);
	}
	return sum / ((double)numberOfAtoms);
}

//Calculate the self-diffusion coefficient by comparing the MSD at 2 different points in time.
double Simulation::calcSelfDiffusionCoefficient(double*** positionsArray, double t1, double t2, double timeStep, unsigned int numberOfAtoms, double lengthX, double lengthY, double lengthZ, bool is2D)
{
	double msd1 = calcMeanSquareDisplacement(positionsArray[(int) round(t1/timeStep)], positionsArray[0],
												numberOfAtoms, lengthX, lengthY, lengthZ, is2D);
	double msd2 = calcMeanSquareDisplacement(positionsArray[(int) round(t2/timeStep)], positionsArray[0], 
												numberOfAtoms, lengthX, lengthY, lengthZ, is2D);

	return (msd2 - msd1) / ((double)6 * (t2 - t1));
}

//Calculate the specific heat from the mean of the temperature and the square of the temperature.
double Simulation::calcSpecificHeat(unsigned int numberOfAtoms, double kB, double numberOfTimeSteps, double* tempArray)
{
	double expT{ 0 }, expT2{ 0 }, tmp{ 0 };
	for (int i{ 0 }; i < numberOfTimeSteps; i++)
	{
		expT += tempArray[i];
		expT2 += pow(tempArray[i], 2);
	}
	expT /= numberOfTimeSteps;
	expT2 /= numberOfTimeSteps;

	//Check that the temperature is greter than 0. If not the specific heat is set to 0.
	if (expT > 0)
	{
		tmp = expT2 - pow(expT, 2);
		tmp *= ((2 * numberOfAtoms) / (3 * pow(expT, 2)));
		tmp = 1 - tmp;
		tmp = 1 / tmp;
		tmp *= (3 * numberOfAtoms*kB)/2;
	}

	return tmp;
}

//Calculate the temperature from the kinetic energy.
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

//Get the total energy by summing the potential and kinetic energy.
double Simulation::calcTotalEnergy(double potentialEnergy, double kineticEnergy)
{
	return potentialEnergy + kineticEnergy;
}

//Newton second law to calculate acceleration
array<double, 3> Simulation::calcAcceleration(double fx, double fy, double fz)
{
	double m = _mat.getMass();
	array<double, 3> a = { fx/m, fy/m, fz/m };
	return a;
}

//Calc Euclidean distance with respect to periodic boundary conditions
//element 0: total distance; element 1, 2, 3: normalized x-,y-,z-components.
array<double, 4> Simulation::calcDistance(Atom* a1, Atom* a2, double lengthX, double lengthY, double lengthZ, bool is2D) const
{
	double rx, ry, rz, r;
	rx = a1->getPositionX() - a2->getPositionX();
	ry = a1->getPositionY() - a2->getPositionY();
	rz = a1->getPositionZ() - a2->getPositionZ();

	if (rx > lengthX / 2.0)
	{
		rx -= lengthX;
	}
	else if (rx < -lengthX / 2.0)
	{
		rx += lengthX;
	}

	if (ry > lengthY / 2.0)
	{
		ry -= lengthY;
	}
	else if (ry < -lengthY / 2.0)
	{
		ry += lengthY;
	}

	if (!is2D)
	{
		if (rz > lengthZ / 2.0)
		{
			rz -= lengthZ;
		}
		else if (rz < -lengthZ / 2.0)
		{
			rz += lengthZ;
		}
	}

	r = sqrt(pow(rx, 2) + pow(ry, 2) + pow(rz, 2));
	rx = rx / r;
	ry = ry / r;
	rz = rz / r;
	array<double, 4> temp = { r, rx, ry, rz };
	return temp;
}

//Calc Euclidean distance with respect to periodic boundary conditions
//element 0: total distance; element 1, 2, 3: normalized x-,y-,z-components.
array<double, 4> Simulation::calcDistance(array<double, 3> pos1, array<double, 3> pos2,
	double lengthX, double lengthY, double lengthZ, bool is2D) const
{
	double rx, ry, rz, r;
	rx = pos1[0] - pos2[0];
	ry = pos1[1] - pos2[1];
	rz = pos1[2] - pos2[2];

	if (rx > lengthX / 2.0)
	{
		rx -= lengthX;
	}
	else if (rx < -lengthX / 2.0)
	{
		rx += lengthX;
	}

	if (ry > lengthY / 2.0)
	{
		ry -= lengthY;
	}
	else if (ry < -lengthY / 2.0)
	{
		ry += lengthY;
	}

	if (!is2D)
	{
		if (rz > lengthZ / 2.0)
		{
			rz -= lengthZ;
		}
		else if (rz < -lengthZ / 2.0)
		{
			rz += lengthZ;
		}
	}

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
array<double, 3> Simulation::calcVelocity(std::array<double, 3> v, std::array<double, 3> aOld, std::array<double, 3> aNew, double timeStep)
{
	double newVX = v[0] + 0.5*timeStep*(aOld[0] + aNew[0]);
	double newVY = v[1] + 0.5*timeStep*(aOld[1] + aNew[1]);
	double newVZ = v[2] + 0.5*timeStep*(aOld[2] + aNew[2]);

	array<double, 3> newV = { newVX, newVY, newVZ };

	return newV;
}

//Function for assigning a new Gaussian velocity for initial velocity distribution and if an Anderson thermostat is to be used. 
array<double, 3> Simulation::generateGaussianVelocity(double variance)
{
	random_device rd;
	mt19937 generator(rd());
	normal_distribution<double> distribution(0, sqrt(variance));
	
	array<double, 3> v;
	for (int i = 0; i < 3; i++)
	{
		v[i] = distribution(generator);
	}

	return v;
}





