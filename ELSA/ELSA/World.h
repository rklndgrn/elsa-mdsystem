#ifndef WORLD_H
#define WORLD_H

#include "../Atom/Atom.h"
#include "../Parameters/Parameters.h"
#include "../Results/Results.h"

class World
{
private:
	vector<Atom> atomList;
	vector<Cell> cellList;
	Parameters myParameters;
	Results myResults;

public:
	World(Parameters) = default;
	~World() = default;

	Atom getAtomInAtomList(unsigned int);

	void addAtomToAtomList(Atom);
};

#endif