#include "World.h"

World::World(Parameters p)
{
	myParameters = p;
	Results r{(int) p.getSimulationTime(), (int) p.getTimeStep(), (int) p.getNumberOfAtoms()};
	myResults = r;
}

Atom World::getAtomInAtomList(unsigned int index)
{
	return atomList.at(index);
}

void World::addAtomToAtomList(Atom a)
{
	atomList.push_back(a);
}

void World::calcPot()
{
	double pot;// , force;
	Atom a1, a2;
	double r;
	for (unsigned int i{ 0 }; i < myParameters.getNumberOfAtoms(); i++)
	{
		a1 = atomList.at(i);
		for (int j{ 0 }; j < a1.getNeighbourList().size(); j++)
		{
			a2 = *a1.getNeighbourList().at(j);
			r = mySimulation.calcDist(a1.getPosX(), a1.getPosY(), a1.getPosZ(), a2.getPosX(), a2.getPosY(), a2.getPosZ());
			pot = mySimulation.calcLJPot(r);
//			force = mySimulation.calcForce(r);
			a1.setPotential(a1.getPotential() + pot);
			a2.setPotential(a2.getPotential() + pot);
//			a1.setForce(a1.getForce() + force);
//			a2.setForce(a2.getForce() + force);
		}
	}
}