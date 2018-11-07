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

	unsigned int atomId{ 0 };
	unsigned int nOfUnitCellsX{ _myParameters.getNumberOfUnitCellsX() };
	unsigned int nOfUnitCellsY{ _myParameters.getNumberOfUnitCellsY() };
	unsigned int nOfUnitCellsZ{ _myParameters.getNumberOfUnitCellsZ() };
	double latticeConstant{ _myParameters.getChosenMaterial().getLatticeConstant() };
	string crystalStructure{ _myParameters.getChosenMaterial().getCrystalStructure() };
	
	
	if (crystalStructure == "fcc")
	{
		for (unsigned int z = 0; z <= nOfUnitCellsZ; z++)
		{
			for (unsigned int y = 0; y <= nOfUnitCellsY; y++)
			{
				for (unsigned int x = 0; x <= _myParameters.getNumberOfUnitCellsX(); x++)
				{
					Atom* a0 = new Atom(atomId++, x*latticeConstant, y*latticeConstant, z*latticeConstant);
					Atom* ax = new Atom(atomId++, x*latticeConstant, (y + 0.5)*latticeConstant, (z + 0.5)*latticeConstant);
					Atom* ay = new Atom(atomId++, (x + 0.5)*latticeConstant, y*latticeConstant, (z + 0.5)*latticeConstant);
					Atom* az = new Atom(atomId++, (x + 0.5)*latticeConstant, (y + 0.5)*latticeConstant, z*latticeConstant);

					addAtomToAtomList(a0);

					if (x != nOfUnitCellsX && y != nOfUnitCellsY && z != nOfUnitCellsZ)
					{
						addAtomToAtomList(ax);
						addAtomToAtomList(ay);
						addAtomToAtomList(az);
					}
					else if (x == nOfUnitCellsX && y != nOfUnitCellsY && z != nOfUnitCellsZ)
					{
						addAtomToAtomList(ax);
					}
					else if (x != nOfUnitCellsX && y == nOfUnitCellsY && z != nOfUnitCellsZ)
					{
						addAtomToAtomList(ay);
					}
					else if (x != nOfUnitCellsX && y != nOfUnitCellsY && z == nOfUnitCellsZ)
					{
						addAtomToAtomList(az);
					}
				}
			}
		}


	}
	else if (crystalStructure == "sc")
	{
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

}

Atom* World::getAtomInAtomList(unsigned int index)
{
	return _atomList.at(index);
}

Results World::getResults()
{
	return _myResults;
}

void World::addAtomToAtomList(Atom* a)
{
	_atomList.push_back(a);
}

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
		for (int j{ 0 }; j < a1->getNeighbourList().size(); j++)
		{
			a2 = a1->getNeighbourList().at(j);
			
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