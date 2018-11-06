/*
	File name: material.cpp
	Date: 2018-11-06
	This file contains the definition of the class Material
*/

#include "material.h"
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
{}


Material::~Material()
{}

string Material::getCrystalStructure()
{
	return _crystalStructure;
}

void Material::setCrystalStructure(string str)
{
	_crystalStructure = str;
}

double Material::getLatticeConstant()
{
	return _latticeConstant;
}

void Material::setLatticeConstant(double lc)
{
	_latticeConstant = lc;
}

double Material::getEpsilon()
{
	return _epsilon;
}

void Material::setEpsilon(double eps)
{
	_epsilon = eps;
}

double Material::getSigma()
{
	return _sigma;
}

void Material::setSigma(double sig)
{
	_sigma = sig;
}

double Material::getCutOffDistance()
{
	return _cutOffDistance;
}

void Material::setCutOffDistance(double cod)
{
	_cutOffDistance = cod;
}

double Material::getMass()
{
	return _mass;
}

void Material::setMass(double m)
{
	_mass = m;
}