#include "../Atom/Atom.h"
#include <array>

#pragma once


class Cell
{
private:
	std::array<unsigned int, 3> _cellID = { 0, 0, 0 };
	std::vector<Atom*> _atomsInCellList;

public:
	std::array<unsigned int,3> getCellID() const;

	void setCellID(unsigned int i,unsigned int j,unsigned int k);
	std::vector<Atom*> getAtomsInCellList() ;
	void addAtomToCellList(Atom* newAtom);
	void clearAtomsInCellList();

	Cell();
	Cell(unsigned int, unsigned int, unsigned int);
	~Cell();

};