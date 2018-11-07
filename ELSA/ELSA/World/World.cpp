#include "World.h"

using namespace std;

World::World(Parameters p)
{
	_myParameters = p;
	Results r{(int) p.getSimulationTime(), (int) p.getTimeStep(), (int) p.getNumberOfAtoms()};
	_myResults = r;
}

Atom World::getAtomInAtomList(unsigned int index)
{
	return _atomList.at(index);
}

void World::addAtomToAtomList(Atom a)
{
	_atomList.push_back(a);
}

void World::calcPotentialAndForce()
{
	double pot, f;
	array<double, 3> force;
	Atom a1, a2;
	array<double, 4> r;
	for (unsigned int i{ 0 }; i < _myParameters.getNumberOfAtoms(); i++)
	{
		a1 = _atomList.at(i);
		for (int j{ 0 }; j < a1.getNeighbourList().size(); j++)
		{
			a2 = *a1.getNeighbourList().at(j);

			r = _mySimulation.calcDistance(a1.getPosX(), a1.getPosY(), a1.getPosZ(), a2.getPosX(), a2.getPosY(), a2.getPosZ());
			f = _mySimulation.calcForce(r[0]);

			force = { f*r[1], f*r[2],f*r[3] };
			pot = _mySimulation.calcLJPotential(r[0]);

			a1.setPotential(a1.getPotential() + pot);
			a2.setPotential(a2.getPotential() + pot);

			a1.setForceX(a1.getForceX() + force[0]);
			a2.setForceX(a2.getForceX() - force[0]);
			a1.setForceY(a1.getForceY() + force[1]);
			a2.setForceY(a2.getForceY() - force[1]);
			a1.setForceZ(a1.getForceZ() + force[2]);
			a2.setForceZ(a2.getForceZ() - force[2]);
		}
	}
}

void World::solveEquationsOfMotion(double elapsedTime)
{
	Atom thisAtom;
	array<double, 3> oldR;
	array<double, 3> oldV;
	array<double, 3> oldA;
	array<double, 3> newR;
	array<double, 3> newV;

	double K{0}; //Kinetic energy.
	double T{0}; //Instantenous temperature
	double timeStep = _myParameters.getTimeStep();
	_myParameters.setTemperature(T);

	random_device rd;
	mt19937 generator(rd());
	uniform_real_distribution<double> distribution(0, 1);
	double randomValue;

	for (int i{ 0 }; i < _atomList.size(); i++)
	{
		thisAtom = _atomList.at(i);
		oldR = { thisAtom.getPosX(), thisAtom.getPosY(), thisAtom.getPosZ() };
		oldV = { thisAtom.getVelocityX(), thisAtom.getVelocityY(), thisAtom.getVelocityZ() };
		oldA = _mySimulation.calcAcceleration(thisAtom.getForceX(), thisAtom.getForceY(), thisAtom.getForceZ());

		randomValue = distribution(generator);

		newR = _mySimulation.calcPosition(oldR, oldV, oldA, timeStep);
		_myResults.setPositions(newR[0], newR[1], newR[2], elapsedTime/timeStep, i);
		
		newV = _mySimulation.calcVelocity(oldV, oldA, timeStep);

		K += _mySimulation.calcKineticEnergy(newV);
	}

	T = _mySimulation.calcTemperature(K, _myParameters.getBoltzmann(), _myParameters.getNumberOfAtoms());
	K = 0;

	//Anderson thermostat.
	for (int i{ 0 }; i < _atomList.size(); i++)
	{
		if (randomValue < _myParameters.getCollisionFrequency()*elapsedTime)
		{
			newV = _mySimulation.generateGaussianVelocity(T);
		}
	}

	_myResults.setKineticEnergy(K, elapsedTime/timeStep);
	_myResults.setTemperature(T, elapsedTime/timeStep);
}