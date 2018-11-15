#include "Cell.h"

using namespace std;

Cell::Cell(unsigned int i, unsigned int j, unsigned int k)
{
	_cellID[0] = i;
	_cellID[1] = j;
	_cellID[2] = k;
};

//Append atom to cell list
void Cell::addAtomToCellList(Atom* newAtom)
{
	_atomsInCellList.push_back(newAtom);
}

//Clear all atoms in cell list
void Cell::clearAtomsInCellList()
{
	_atomsInCellList.erase(_atomsInCellList.begin(), _atomsInCellList.end());
}

//Getters.
array<unsigned int,3> Cell::getCellID() const
{
	return _cellID;
};


vector<Atom*> Cell::getAtomsInCellList()
{
	return _atomsInCellList;
}


//Setters.
void Cell::setCellID(unsigned int i,unsigned int j,unsigned int k)
{
	_cellID[0] = i;
	_cellID[1] = j;
	_cellID[2] = k;
}

