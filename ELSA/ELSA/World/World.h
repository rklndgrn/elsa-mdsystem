#ifndef WORLD_H
#define WORLD_H

#include "../Atom/Atom.h"
#include "../Cell/Cell.h"
#include "../Parameters/Parameters.h"
#include "../Results/Results.h"
#include "../Simulation/Simulation.h"
#include <vector>

class World
{
private:
	std::vector<Atom*> _atomList;
	std::vector<Cell> _cellList;
	Parameters _myParameters;
	Results _myResults;
	Simulation _mySimulation;
	
	void setupSystem(Parameters);
public:
	World(Parameters);
	~World() = default;

	Results getResults();

	Atom* getAtomInAtomList(unsigned int);

	void addAtomToAtomList(Atom*);

	void calcPotentialAndForce();

	void calcPotentialEnergy();
	void calcKineticEnergy();
};

#endif