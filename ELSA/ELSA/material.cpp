/*
	File name: material.cpp
	Date: 2018-11-06
	This file contains the definition of the class Material
*/

#include "material.h"
#include <string>

using namespace std;

Material::Material() : 
	crystalStructure{""},
	latticeConstant{0},
	epsilon{0},
	sigma{0},
	cutOffDistance{0},
	mass{0}
{}

Material::~Material()
{}

string Material::getCrystalStructure()
{
	return crystalStructure;
}

void Material::setCrystalStructure(string str)
{
	crystalStructure = str;
}

double Material::getLatticeConstant()
{
	return latticeConstant;
}

void Material::setLatticeConstant(double lc)
{
	latticeConstant = lc;
}

double Material::getEpsilon()
{
	return epsilon;
}

void Material::setEpsilon(double eps)
{
	epsilon = eps;
}

double Material::getSigma()
{
	return sigma;
}

void Material::setSigma(double sig)
{
	sigma = sig;
}

double Material::getCutOffDistance()
{
	return cutOffDistance;
}

void Material::setCutOffDistance(double cod)
{
	cutOffDistance = cod;
}

double Material::getMass()
{
	return mass;
}

void Material::setMass(double m)
{
	mass = m;
}