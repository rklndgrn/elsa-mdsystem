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
	for (unsigned int z = 0; z <= nOfUnitCellsZ; z++)
	{
		for (unsigned int y = 0; y <= nOfUnitCellsY; y++)
		{
			for (unsigned int x = 0; x <= _myParameters.getNumberOfUnitCellsX(); x++)
			{
				Atom* a0 = new Atom(atomId++, x*latticeConstant, y*latticeConstant, z*latticeConstant);
				addAtomToAtomList(a0);

				if (x != nOfUnitCellsX && y != nOfUnitCellsY && z != nOfUnitCellsZ)
				{
					Atom* ax = new Atom(atomId++, x*latticeConstant, (y + 0.5)*latticeConstant, (z + 0.5)*latticeConstant);
					Atom* ay = new Atom(atomId++, (x + 0.5)*latticeConstant, y*latticeConstant, (z + 0.5)*latticeConstant);
					Atom* az = new Atom(atomId++, (x + 0.5)*latticeConstant, (y + 0.5)*latticeConstant, z*latticeConstant);
					addAtomToAtomList(ax);
					addAtomToAtomList(ay);
					addAtomToAtomList(az);
				}
				else if (x == nOfUnitCellsX && y != nOfUnitCellsY && z != nOfUnitCellsZ)
				{
					Atom* ax = new Atom(atomId++, x*latticeConstant, (y + 0.5)*latticeConstant, (z + 0.5)*latticeConstant);
					addAtomToAtomList(ax);
				}
				else if (x != nOfUnitCellsX && y == nOfUnitCellsY && z != nOfUnitCellsZ)
				{
					Atom* ay = new Atom(atomId++, (x + 0.5)*latticeConstant, y*latticeConstant, (z + 0.5)*latticeConstant);
					addAtomToAtomList(ay);
				}
				else if (x != nOfUnitCellsX && y != nOfUnitCellsY && z == nOfUnitCellsZ)
				{
					Atom* az = new Atom(atomId++, (x + 0.5)*latticeConstant, (y + 0.5)*latticeConstant, z*latticeConstant);
					addAtomToAtomList(az);
				}
			}
		}
	}
}

void World::generateAtomsAtScLattice(double latticeConstant, unsigned int nOfUnitCellsX, unsigned int nOfUnitCellsY, unsigned int nOfUnitCellsZ)
{
	unsigned int atomId{ 0 };
	for (unsigned int x = 0; x <= _myParameters.getNumberOfUnitCellsX(); x++)
	{
		for (unsigned int y = 0; y <= _myParameters.getNumberOfUnitCellsY(); y++)
		{
			for (unsigned int z = 0; z <= _myParameters.getNumberOfUnitCellsZ(); z++)
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
	default_random_engine generator;
	normal_distribution<double> distribution(0, 1);

	for (unsigned int atomId = 0; atomId < _myParameters.getNumberOfAtoms(); atomId++)
	{
		_atomList.at(atomId)->setVelocityX(sqrt(k_boltzmann * _myParameters.getTemperature() / _myParameters.getChosenMaterial().getMass()) * distribution(generator));
		_atomList.at(atomId)->setVelocityY(sqrt(k_boltzmann * _myParameters.getTemperature() / _myParameters.getChosenMaterial().getMass()) * distribution(generator));
		_atomList.at(atomId)->setVelocityZ(sqrt(k_boltzmann * _myParameters.getTemperature() / _myParameters.getChosenMaterial().getMass()) * distribution(generator));
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
	unsigned int nOfUnitCellsX{ _myParameters.getNumberOfUnitCellsX() };
	unsigned int nOfUnitCellsY{ _myParameters.getNumberOfUnitCellsY() };
	unsigned int nOfUnitCellsZ{ _myParameters.getNumberOfUnitCellsZ() };
	double latticeConstant{ _myParameters.getChosenMaterial().getLatticeConstant() };
	double cellSize{ _myParameters.getChosenMaterial().getCellSize() };

	unsigned int numberOfCellsI{ (unsigned int)ceil(nOfUnitCellsX * latticeConstant / cellSize) };
	unsigned int numberOfCellsJ{ (unsigned int)ceil(nOfUnitCellsY * latticeConstant / cellSize) };
	unsigned int numberOfCellsK{ (unsigned int)ceil(nOfUnitCellsZ * latticeConstant / cellSize) };


	for (unsigned int atomId = 0; atomId < _myParameters.getNumberOfAtoms(); atomId++)
	{
		Atom* a = getAtomInAtomList(atomId);

		i = (unsigned int)floor(a->getPosX() / cellSize);
		j = (unsigned int)floor(a->getPosY() / cellSize);
		k = (unsigned int)floor(a->getPosZ() / cellSize);

		getCellInCellList(i + j * numberOfCellsI + k * numberOfCellsI*numberOfCellsJ)->addAtomToCellList(a);
	}
}

Atom* World::getAtomInAtomList(unsigned int index)
{
	return _atomList.at(index);
}

Cell* World::getCellInCellList(unsigned int index)
{
	return _cellList.at(index);
}

//Cell* World::getCellInCellList(unsigned int i, unsigned int j, unsigned int k)
//{
//	
//}

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
	double pot, f;
	array<double, 3> force;
	Atom* a1;
	Atom* a2;
	array<double, 4> r;
	for (unsigned int i{ 0 }; i < _myParameters.getNumberOfAtoms() - 1/*?*/; i++)
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

/*void World::calcKineticEnergy()
{
	Atom a1;
	for (unsigned int i{ 0 }; i < _myParameters.getNumberOfAtoms() - 1; i++)
	{
		a1 = _atomList.at(i);
		_myResults.setKineticEnergy(**_myResults.getKineticEnergy() )
	}

}*/