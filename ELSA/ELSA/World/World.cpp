#include "World.h"
#include <random>
#include <cmath>

#define k_boltzmann 1.38064852E-23

using namespace std;

World::World(Parameters p)
{
	setupSystem(p);
}

void World::setupSystem(Parameters p)
{
	_myParameters = p;

	_myResults = Results{ (int)p.getSimulationTime(), (int)p.getTimeStep(), (int)p.getNumberOfAtoms() };
	_mySimulation = Simulation(p.getChosenMaterial());


	unsigned int nOfUnitCellsX{ _myParameters.getNumberOfUnitCellsX() };
	unsigned int nOfUnitCellsY{ _myParameters.getNumberOfUnitCellsY() };
	unsigned int nOfUnitCellsZ{ _myParameters.getNumberOfUnitCellsZ() };
	double latticeConstant{ _myParameters.getChosenMaterial().getLatticeConstant() };
	string crystalStructure{ _myParameters.getChosenMaterial().getCrystalStructure() };
	
	if (crystalStructure == "fcc")
	{
		generateAtomsAtFccLattice(latticeConstant, nOfUnitCellsX, nOfUnitCellsY, nOfUnitCellsZ);
	}
	else if (crystalStructure == "sc")
	{
		generateAtomsAtScLattice(latticeConstant, nOfUnitCellsX, nOfUnitCellsY, nOfUnitCellsZ);
	}

	setupNeighbourLists();
	distributeInitialVelocities();

	generateCells();
	populateCells();
}

void World::generateAtomsAtFccLattice(double latticeConstant, unsigned int nOfUnitCellsX, unsigned int nOfUnitCellsY, unsigned int nOfUnitCellsZ)
{
	unsigned int atomId{ 0 };
	for (unsigned int z = 0; z < nOfUnitCellsZ; z++)
	{
		for (unsigned int y = 0; y < nOfUnitCellsY; y++)
		{
			for (unsigned int x = 0; x < nOfUnitCellsX; x++)
			{
				Atom* a0 = new Atom(atomId++, x*latticeConstant, y*latticeConstant, z*latticeConstant);
				Atom* ax = new Atom(atomId++, x*latticeConstant, (y + 0.5)*latticeConstant, (z + 0.5)*latticeConstant);
				Atom* ay = new Atom(atomId++, (x + 0.5)*latticeConstant, y*latticeConstant, (z + 0.5)*latticeConstant);
				Atom* az = new Atom(atomId++, (x + 0.5)*latticeConstant, (y + 0.5)*latticeConstant, z*latticeConstant);
				addAtomToAtomList(a0);
				addAtomToAtomList(ax);
				addAtomToAtomList(ay);
				addAtomToAtomList(az);

			}
		}
	}
}

void World::generateAtomsAtScLattice(double latticeConstant, unsigned int nOfUnitCellsX, unsigned int nOfUnitCellsY, unsigned int nOfUnitCellsZ)
{
	unsigned int atomId{ 0 };
	for (unsigned int x = 0; x < nOfUnitCellsZ; x++)
	{
		for (unsigned int y = 0; y < nOfUnitCellsY; y++)
		{
			for (unsigned int z = 0; z < nOfUnitCellsX; z++)
			{
				Atom* a = new Atom(atomId++, x*latticeConstant, y*latticeConstant, z*latticeConstant);
				addAtomToAtomList(a);
			}
		}
	}
}

void World::setupNeighbourLists()
{
	double cutOffDistance{ _myParameters.getChosenMaterial().getCutOffDistance() };
	double atomDistance{ 0 };
	for (unsigned int atomId = 0; atomId < _myParameters.getNumberOfAtoms(); atomId++)
	{
		for (unsigned int higherId = atomId + 1; higherId < _myParameters.getNumberOfAtoms(); higherId++)
		{
			atomDistance = _mySimulation.calcDistance(getAtomInAtomList(atomId), getAtomInAtomList(higherId))[0];
			if (atomDistance < cutOffDistance)
			{
				getAtomInAtomList(atomId)->addToNeighbourList(getAtomInAtomList(higherId));
			}
		}
	}
}

void World::distributeInitialVelocities()
{
	double sigma = sqrt(k_boltzmann * _myParameters.getTemperature() / _myParameters.getChosenMaterial().getMass());
	
	random_device rand;
	mt19937 generator(rand());
	normal_distribution<double> distribution(0, sigma);

	for (unsigned int atomId = 0; atomId < _myParameters.getNumberOfAtoms(); atomId++)
	{

		_atomList.at(atomId)->setVelocityX( distribution(generator));
		_atomList.at(atomId)->setVelocityY( distribution(generator));
		_atomList.at(atomId)->setVelocityZ( distribution(generator));
	}
}

void World::generateCells()
{
	unsigned int nOfUnitCellsX{ _myParameters.getNumberOfUnitCellsX() };
	unsigned int nOfUnitCellsY{ _myParameters.getNumberOfUnitCellsY() };
	unsigned int nOfUnitCellsZ{ _myParameters.getNumberOfUnitCellsZ() };
	double latticeConstant{ _myParameters.getChosenMaterial().getLatticeConstant() };
	double cellSize{ _myParameters.getChosenMaterial().getCellSize() };

	unsigned int numberOfCellsI{ (unsigned int)ceil(nOfUnitCellsX * latticeConstant / cellSize) };
	unsigned int numberOfCellsJ{ (unsigned int)ceil(nOfUnitCellsY * latticeConstant / cellSize) };
	unsigned int numberOfCellsK{ (unsigned int)ceil(nOfUnitCellsZ * latticeConstant / cellSize) };

	for (unsigned int k = 0; k < numberOfCellsK; k++)
	{
		for (unsigned int j = 0; j < numberOfCellsJ; j++)
		{
			for (unsigned int i = 0; i < numberOfCellsI; i++)
			{
				Cell* c = new Cell(i, j, k);
				addCellToCellList(c);
			}
		}
	}
}

void World::populateCells()
{
	unsigned int i;
	unsigned int j;
	unsigned int k;

	double cellSize{ _myParameters.getChosenMaterial().getCellSize() };

	for (unsigned int atomId = 0; atomId < _myParameters.getNumberOfAtoms(); atomId++)
	{
		Atom* a = getAtomInAtomList(atomId);

		i = (unsigned int)floor(a->getPosX() / cellSize);
		j = (unsigned int)floor(a->getPosY() / cellSize);
		k = (unsigned int)floor(a->getPosZ() / cellSize);

		getCellInCellList(i, j, k)->addAtomToCellList(a);
	}
}

Atom* World::getAtomInAtomList(unsigned int index)
{
	return _atomList.at(index);
}


Cell* World::getCellInCellList(unsigned int i, unsigned int j, unsigned int k)
{
	unsigned int index = i + j * _myParameters.getNumberOfCellsI() + k * _myParameters.getNumberOfCellsI()*_myParameters.getNumberOfCellsJ();
	return _cellList.at(index);
}

Results World::getResults()
{
	return _myResults;
}

void World::addAtomToAtomList(Atom* a)
{
	_atomList.push_back(a);
}

void World::addCellToCellList(Cell* c)
{
	_cellList.push_back(c);
}


//	Calculates the force and potential and stores them in the atoms. The force is directional but the potential is not.
void World::calcPotentialAndForce()
{
	double pot{0};
	double f{0};
	double pressure{0};
	array<double, 3> force;
	Atom* a1;
	Atom* a2;
	array<double, 4> r;
	for (unsigned int i{0}; i < _myParameters.getNumberOfAtoms() - 1; i++)
	{
		a1 = _atomList.at(i);
		// For all atoms close to a1
		for (int j{ 0 }; j < a1->getNeighbourList().size(); j++)
		{
			a2 = a1->getNeighbourList().at(j);
			
			// Returns the distance as a homogeneous vector
			r = _mySimulation.calcDistance(a1->getPosX(), a1->getPosY(), a1->getPosZ(), a2->getPosX(), a2->getPosY(), a2->getPosZ());
			f = _mySimulation.calcForce(r[0]);

			force = { f*r[1], f*r[2],f*r[3] };
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

// Calculate the potential energy as the sum of the potential of all the atoms.
void World::calcPotentialEnergy()
{
	Atom* a1;
	for (unsigned int i{ 0 }; i < _myParameters.getNumberOfAtoms() - 1; i++)
	{
		a1 = _atomList.at(i);
		_myResults.setPotentialEnergy(**_myResults.getPotentialEnergy() + a1->getPotential(),_myParameters.getTimeStep());
	}
}

/*
void World::calcKineticEnergy()
{
	Atom* a1;
	for (unsigned int i{ 0 }; i < _myParameters.getNumberOfAtoms() - 1; i++)
	{
		a1 = _atomList.at(i);
		_myResults.setKineticEnergy(**_myResults.getKineticEnergy())
	}

}
*/

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
		_myResults.setPositions(newR[0], newR[1], newR[2], (int) elapsedTime/timeStep, i);
		
		newV = _mySimulation.calcVelocity(oldV, oldA, timeStep);

		thisAtom->setPosition(newR);
		thisAtom->setVelocity(newV);
		
		K += _mySimulation.calcKineticEnergy(newV[0], newV[1], newV[2]);
	}

	T = _mySimulation.calcTemperature(K, _myParameters.getBoltzmann(), _myParameters.getNumberOfAtoms());


	//Save the kinetic energy and temperature for the results presentation.
	_myResults.setKineticEnergy(K, elapsedTime / timeStep);
	_myResults.setTemperature(T, elapsedTime / timeStep);

	if (T > 0)
	{
		//Anderson thermostat.
		for (int i{ 0 }; i < _atomList.size(); i++)
		{
			thisAtom = _atomList.at(i);

			randomValue = distribution(generator);

			if (randomValue < _myParameters.getCollisionFrequency()*elapsedTime)
			{
				newV = _mySimulation.generateGaussianVelocity(T);
				thisAtom->setVelocity(newV);
			}
		}
	}


}
