#ifndef WORLD_H
#define WORLD_H

#include "../Atom/Atom.h"
#include "../Cell/Cell.h"
#include "../Parameters/Parameters.h"
#include "../Results/Results.h"
#include "../Simulation/Simulation.h"

class World
{
private:
	std::vector<Atom> atomList;
	std::vector<Cell> cellList;
	Parameters myParameters;
	Results myResults;
	Simulation mySimulation;
	
public:
	World(Parameters);
	~World() = default;

	Atom getAtomInAtomList(unsigned int);

	void addAtomToAtomList(Atom);

	void calcPot();
};

#endif