#include "../Atom/Atom.h"

#pragma once


class Cell
{
private:
	unsigned int _cellID[3];
	std::vector<Atom*> atomsInCellList;

public:
	unsigned int getCellID() const;
	std::vector<Atom*>* getAtomsInCellList() const;

};