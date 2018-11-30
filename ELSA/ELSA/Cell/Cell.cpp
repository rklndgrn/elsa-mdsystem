#include "Cell.h"

using namespace std;

//Constructor.
Cell::Cell(unsigned int i, unsigned int j, unsigned int k)
{
	_cellID[0] = i;
	_cellID[1] = j;
	_cellID[2] = k;
};

//Copy constructor, e.g. r2{r].
Cell::Cell(Cell const& other)
{
	_cellID[0] = other.getCellID()[0];
	_cellID[1] = other.getCellID()[1];
	_cellID[2] = other.getCellID()[2];
}

//Copy allocation, t.ex. r = r2{other}.
Cell & Cell::operator = (Cell const& other)
{
	_cellID[0] = other.getCellID()[0];
	_cellID[1] = other.getCellID()[1];
	_cellID[2] = other.getCellID()[2];

	return *this;
}

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

