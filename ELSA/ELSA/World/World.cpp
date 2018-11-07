#include "World.h"

using namespace std;

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

void World::calcPotentialAndForce()
{
	double pot, f;
	array<double, 3> force;
	Atom a1, a2;
	array<double, 4> r;
	for (unsigned int i{ 0 }; i < myParameters.getNumberOfAtoms(); i++)
	{
		a1 = atomList.at(i);
		for (int j{ 0 }; j < a1.getNeighbourList().size(); j++)
		{
			a2 = *a1.getNeighbourList().at(j);

			r = mySimulation.calcDistance(a1.getPosX(), a1.getPosY(), a1.getPosZ(), a2.getPosX(), a2.getPosY(), a2.getPosZ());
			f = mySimulation.calcForce(r[0]);

			force = { f*r[1], f*r[2],f*r[3] };
			pot = mySimulation.calcLJPotential(r[0]);

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