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