#include "material.h"
#include <string>

using namespace std;

string material::getCrystalStructure()
{
	return crystalStructure;
}

void material::setCrystalStructure(string str)
{
	crystalStructure = str;
}

double material::getLatticeConstant()
{
	return latticeConstant;
}

void material::setLatticeConstant(double lc)
{
	latticeConstant = lc;
}

double material::getEpsilon()
{
	return epsilon;
}

void material::setEpsilon(double eps)
{
	epsilon = eps;
}

double material::getSigma()
{
	return sigma;
}

void material::setSigma(double sig)
{
	sigma = sig;
}

double material::getCutOffDistance()
{
	return cutOffDistance;
}

void material::setCutOffDistance(double cod)
{
	cutOffDistance = cod;
}

double material::getMass()
{
	return mass;
}

void material::setMass(double m)
{
	mass = m;
}