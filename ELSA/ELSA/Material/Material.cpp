/*
	File name: material.cpp
	Date: 2018-11-06
	This file contains the definition of the class Material
*/

#include "Material.h"
#include <string>

using namespace std;

Material::Material() : 
	_crystalStructure{""},
	_latticeConstant{0},
	_epsilon{0},
	_sigma{0},
	_cutOffDistance{0},
	_mass{0}
{}

Material::Material(
	std::string crystalStructure,
	double latticeConstant,
	double epsilon,
	double sigma,
	double cutOffDistance,
	double mass) : 
	_crystalStructure{ crystalStructure },
	_latticeConstant{ latticeConstant },
	_epsilon{ epsilon },
	_sigma{ sigma },
	_cutOffDistance{ cutOffDistance },
	_mass{ mass }
{
	//Calculate cell size from cut off distance
	_cellSize = ceil(cutOffDistance / latticeConstant)*latticeConstant;
}


//getters
double Material::getCellSize() const
{
	return _cellSize;
}

double Material::getCutOffDistance() const
{
	return _cutOffDistance;
}

double Material::getEpsilon() const
{
	return _epsilon;
}

double Material::getLatticeConstant() const
{
	return _latticeConstant;
}

double Material::getMass() const
{
	return _mass;
}

double Material::getSigma() const
{
	return _sigma;
}

string Material::getCrystalStructure() const
{
	return _crystalStructure;
}

//setters
void Material::setCellSize(double cellSize)
{
	_cellSize = cellSize;
}

void Material::setCrystalStructure(string str)
{
	_crystalStructure = str;
}

void Material::setCutOffDistance(double cod)
{
	_cutOffDistance = cod;
}

void Material::setEpsilon(double eps)
{
	_epsilon = eps;
}

void Material::setLatticeConstant(double lc)
{
	_latticeConstant = lc;
}

void Material::setSigma(double sig)
{
	_sigma = sig;
}

void Material::setMass(double m)
{
	_mass = m;
}
