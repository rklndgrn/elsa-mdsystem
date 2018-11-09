#include "World.h"

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
	generateCells();
	populateCells();

	setupNeighbourLists();
	distributeInitialVelocities();

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
	unsigned int maxI = _myParameters.getNumberOfCellsI() - 1;
	unsigned int maxJ = _myParameters.getNumberOfCellsJ() - 1;
	unsigned int maxK = _myParameters.getNumberOfCellsK() - 1;
	unsigned int i, j, k, lowerNeighbourI, upperNeighbourI, lowerNeighbourJ, upperNeighbourJ, lowerNeighbourK, upperNeighbourK;
	for (unsigned int atomId = 0; atomId < _myParameters.getNumberOfAtoms(); atomId++)
	{
		array<array<unsigned int,3>, 27> index; 
		i = getAtomInAtomList(atomId)->getCellIndex()[0];
		j = getAtomInAtomList(atomId)->getCellIndex()[1];
		k = getAtomInAtomList(atomId)->getCellIndex()[2];
		
		lowerNeighbourI = i - 1;
		upperNeighbourI = i + 1 ;
		lowerNeighbourJ = j - 1;
		upperNeighbourJ = j + 1;
		lowerNeighbourK = k - 1;
		upperNeighbourK = k + 1;

		if (i == 0)
		{
			lowerNeighbourI = maxI;
			upperNeighbourI = i + 1;
		}
		else if (i == maxI)
		{
			upperNeighbourI = 0;
			lowerNeighbourI = i - 1;
		}
		if (j == 0)
		{
			lowerNeighbourJ = maxJ;
			upperNeighbourJ = j + 1;
		}
		else if (j == maxJ)
		{
			upperNeighbourJ = 0;
			lowerNeighbourJ = j - 1;
		}
		if (k == 0)
		{
			lowerNeighbourK = maxK;
			upperNeighbourK = k + 1;
		}
		else if (k == maxK)
		{
			lowerNeighbourK = k - 1;
			upperNeighbourK = 0;
		}

		index.at(0) = { lowerNeighbourI, lowerNeighbourJ, lowerNeighbourK };
		index.at(1) = { lowerNeighbourI, lowerNeighbourJ, k };
		index.at(2) = { lowerNeighbourI, lowerNeighbourJ, upperNeighbourK };
		index.at(3) = { lowerNeighbourI, j, lowerNeighbourK };
		index.at(4) = { lowerNeighbourI, j, k };
		index.at(5) = { lowerNeighbourI, j, upperNeighbourK };
		index.at(6) = { lowerNeighbourI, upperNeighbourJ, lowerNeighbourK };
		index.at(7) = { lowerNeighbourI, upperNeighbourJ, k };
		index.at(8) = { lowerNeighbourI, upperNeighbourJ, upperNeighbourK };

		index.at(9) = { i, lowerNeighbourJ, lowerNeighbourK };
		index.at(10) = { i, lowerNeighbourJ, k };
		index.at(11) = { i, lowerNeighbourJ, upperNeighbourK };
		index.at(12) = { i, j, lowerNeighbourK };
		index.at(13) = { i, j, k };
		index.at(14) = { i, j, upperNeighbourK };
		index.at(15) = { i, upperNeighbourJ, lowerNeighbourK };
		index.at(16) = { i, upperNeighbourJ, k };
		index.at(17) = { i, upperNeighbourJ, upperNeighbourK };

		index.at(18) = { upperNeighbourI, lowerNeighbourJ, lowerNeighbourK };
		index.at(19) = { upperNeighbourI, lowerNeighbourJ, k };
		index.at(20) = { upperNeighbourI, lowerNeighbourJ, upperNeighbourK };
		index.at(21) = { upperNeighbourI, j, lowerNeighbourK };
		index.at(22) = { upperNeighbourI, j, k };
		index.at(23) = { upperNeighbourI, j, upperNeighbourK };
		index.at(24) = { upperNeighbourI, upperNeighbourJ, lowerNeighbourK };
		index.at(25) = { upperNeighbourI, upperNeighbourJ, k };
		index.at(26) = { upperNeighbourI, upperNeighbourJ, upperNeighbourK };
		
		for (unsigned int m{ 0 }; m <= 26; m++)
		{	
			for (unsigned int n{ 0 }; n < getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().size(); n++)
			{
				atomDistance = _mySimulation.calcDistance(getAtomInAtomList(atomId), getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().at(n))[0];
				if (atomDistance < cutOffDistance /*&& atomId < getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().at(n)->getID()*/)
				{
					getAtomInAtomList(atomId)->addToNeighbourList(getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().at(n));
				}
			}
		}

	}
}

void World::distributeInitialVelocities()
{
	double sigma = sqrt(_myParameters.getBoltzmann() * _myParameters.getTemperature() / _myParameters.getChosenMaterial().getMass());
	
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
		a->setCellIndex(i, j, k);
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
	double pot, f;
	array<double, 3> force;
	Atom* a1;
	Atom* a2;
	array<double, 4> r;
	for (unsigned int i{ 0 }; i < _myParameters.getNumberOfAtoms() - 1; i++)
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