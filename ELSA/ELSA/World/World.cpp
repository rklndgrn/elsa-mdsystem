#include "World.h"

using namespace std;

World::World(Parameters p)
{
	_mySimulation = Simulation(p.getChosenMaterial());
	_myParameters = p;
	_myResults = Results((int)p.getSimulationTime(), (int)p.getTimeStep(), (int)p.getNumberOfAtoms());
}

Atom* World::getAtomInAtomList(unsigned int index)
{
	return _atomList.at(index);
}

Results World::getResults()
{
	return _myResults;
}

double World::getWorldVolume() const
{
	double numberOfUnitCells = _myParameters.getNumberOfUnitCellsX()*_myParameters.getNumberOfUnitCellsY()*_myParameters.getNumberOfUnitCellsZ();
	return numberOfUnitCells*pow(_myParameters.getChosenMaterial().getLatticeConstant(), 3);
}

void World::addAtomToAtomList(Atom* a)
{
	_atomList.push_back(a);
}

void World::calcPotentialAndForce()
{
	double pot{0};
	double f{0};
	array<double, 3> force;
	Atom* a1;
	Atom* a2;
	array<double, 4> r;
	for (unsigned int i{0}; i < _myParameters.getNumberOfAtoms() - 1; i++)
	{
		a1 = _atomList.at(i);
		for (int j{ 0 }; j < a1->getNeighbourList().size(); j++)
		{
			a2 = a1->getNeighbourList().at(j);
			
			r = _mySimulation.calcDistance(a1->getPosX(), a1->getPosY(), a1->getPosZ(), a2->getPosX(), a2->getPosY(), a2->getPosZ());
			f = _mySimulation.calcForce(r[0]);

			pot = _mySimulation.calcLJPotential(r[0]);

			a1->setPotential(a1->getPotential() + pot);
			a2->setPotential(a2->getPotential() + pot);

			a1->setForceX(a1->getForceX() + force[0]);
			a2->setForceX(a2->getForceX() - force[0]);
			a1->setForceY(a1->getForceY() + force[1]);
			a2->setForceY(a2->getForceY() - force[1]);
			a1->setForceZ(a1->getForceZ() + force[2]);
			a2->setForceZ(a2->getForceZ() - force[2]);
		}
	}
}


void World::calcPotentialEnergy()
{
	Atom* a1;
	for (unsigned int i{ 0 }; i < _myParameters.getNumberOfAtoms() - 1; i++)
	{
		a1 = _atomList.at(i);
		_myResults.setPotentialEnergy(**_myResults.getPotentialEnergy() + a1->getPotential(),_myParameters.getTimeStep());
	}
}


void World::calcPressure(double elapsedTime)
{
	double pressure{0};
	int N = _myParameters.getNumberOfAtoms();
	int index = (int)(elapsedTime / _myParameters.getTimeStep());
	Atom *a1, *a2;
	double f{0};
	array<double, 4> r;

	for (int i{ 0 }; i < N; i++)
	{
		a1 = _atomList.at(i);
		for (int j{ 0 }; j < a1->getNeighbourList().size(); j++)
		{
			a2 = a1->getNeighbourList().at(j);
			r = _mySimulation.calcDistance(a1->getPosX(), a1->getPosY(), a1->getPosZ(), a2->getPosX(), a2->getPosY(), a2->getPosZ());
			f = _mySimulation.calcForce(r[0]);

			pressure += r[0]*f;
		}
	}
	cout << "The pork sum is " << pressure << "!" << endl;

	double V = getWorldVolume();
	double* tempArray = *_myResults.getTemperature();
	double T = tempArray[index];
	pressure /= elapsedTime * 6 * V;
	pressure += N * _myParameters.getBoltzmann() * T / V;

	cout << "The pressure is set to " << pressure << "!" << endl;
	_myResults.setInternalPressure(pressure, index);
}

//Function to solve the equations of motion. Finds new velocities and positions of atoms and calculates their kinetic energy and temperature.
void World::solveEquationsOfMotion(double elapsedTime)
{
	Atom* thisAtom;
	array<double, 3> oldR;
	array<double, 3> oldV;
	array<double, 3> oldA;
	array<double, 3> newR;
	array<double, 3> newV;

	double K{0}; //Kinetic energy.
	double T{0}; //Instantenous temperature
	double timeStep = _myParameters.getTimeStep();
	_myParameters.setTemperature(T);

	//Stuff related to the Gaussian velocity for the Anderson thermostat.
	random_device rd;
	mt19937 generator(rd());
	uniform_real_distribution<double> distribution(0, 1);
	double randomValue;

	//Go through the atom list and assign new positions and velocities using the Velociy Verlet Algorithm.
	for (int i{ 0 }; i < _atomList.size(); i++)
	{
		thisAtom = _atomList.at(i);
		oldR = {thisAtom->getPosX(), thisAtom->getPosY(), thisAtom->getPosZ()};
		oldV = {thisAtom->getVelocityX(), thisAtom->getVelocityY(), thisAtom->getVelocityZ()};
		oldA = _mySimulation.calcAcceleration(thisAtom->getForceX(), thisAtom->getForceY(), thisAtom->getForceZ());

		newR = _mySimulation.calcPosition(oldR, oldV, oldA, timeStep);
		_myResults.setPositions(newR[0], newR[1], newR[2], (int) (elapsedTime/timeStep), i);
		
		newV = _mySimulation.calcVelocity(oldV, oldA, timeStep);

		thisAtom->setPosition(newR);
		thisAtom->setVelocity(newV);
		
		K += _mySimulation.calcKineticEnergy(newV[0], newV[1], newV[2]);
	}

	T = _mySimulation.calcTemperature(K, _myParameters.getBoltzmann(), _myParameters.getNumberOfAtoms());

	cout << "K is " << K << endl;

	//Save the kinetic energy and temperature for the results presentation.
	_myResults.setKineticEnergy(K, (int) (elapsedTime / timeStep));
	_myResults.setTemperature(T, (int) (elapsedTime / timeStep));

	if (T > 0)
	{
		//Anderson thermostat.
		for (int i{ 0 }; i < _atomList.size(); i++)
		{
			thisAtom = _atomList.at(i);

			randomValue = distribution(generator);

			if (randomValue < _myParameters.getCollisionFrequency()*elapsedTime)
			{
				cout << "Gaussian velocities for particle " << i << " when T is " << T << "... may God have mercy on us all..." << endl;
				newV = _mySimulation.generateGaussianVelocity(T);
				thisAtom->setVelocity(newV);
			}
		}
	}
}
