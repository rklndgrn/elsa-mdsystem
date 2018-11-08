#include "../Atom/Atom.h"
#include <array>

#pragma once


class Cell
{
private:
	std::array<unsigned int, 3> _cellID = { 0, 0, 0 };
	std::vector<Atom*> _atomsInCellList;

public:
	Cell();
	~Cell();

	std::array<unsigned int,3> getCellID() const;
	std::vector<Atom*> getAtomsInCellList();

	void addAtomToCellList(Atom newAtom);
	void clearAtomsInCellList();
	void setCellID(unsigned int i, unsigned int j, unsigned int k);
};