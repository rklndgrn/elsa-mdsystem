#pragma once

#include "../Atom/Atom.h"
#include <array>


class Cell
{
private:
	std::array<unsigned int, 3> _cellID = { 0, 0, 0 };

	std::vector<Atom*> _atomsInCellList;

public:
	Cell() = default;
	Cell(unsigned int, unsigned int, unsigned int);
	~Cell() = default;

	void addAtomToCellList(Atom* newAtom);
	void clearAtomsInCellList();

	//getters
	std::array<unsigned int,3> getCellID() const;

	std::vector<Atom*> getAtomsInCellList() ;

	//setters
	void setCellID(unsigned int i, unsigned int j, unsigned int k);


};