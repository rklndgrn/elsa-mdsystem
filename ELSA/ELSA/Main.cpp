#include "./Atom/Atom.h"
#include <iostream>

using namespace std;

int main()
{
	Atom a{1, 1, 2, 3};
	Atom b{ 2, 87, 4, 2 };

	cout << "a: ID " << a.getID() << ", position (" << a.getPosX() << ", " << a.getPosY() << ", " << a.getPosZ() << ") och hastighet (" << a.getVelocityX() << ", " << a.getVelocityY() << ", " << a.getVelocityZ() << ")!" << endl;
	cout << "b: ID " << b.getID() << ", position (" << b.getPosX() << ", " << b.getPosY() << ", " << b.getPosZ() << ") och hastighet (" << b.getVelocityX() << ", " << b.getVelocityY() << ", " << b.getVelocityZ() << ")!" << endl;


	a.setPosition(6, 2, 3);
	a.setVelocity(5, 3, 2);
	a.addToNeighbourList(b);

	vector<Atom*> anl = a.getNeighbourList();
	Atom q = *anl[0];

	cout << "Fast nu har vi en atom med ID " << a.getID() << ", position (" << a.getPosX() << ", " << a.getPosY() << ", " << a.getPosZ() << ") och hastighet (" << a.getVelocityX() << ", " << a.getVelocityY() << ", " << a.getVelocityZ() << ")!" << endl;
	cout << "I dess lista ligger också ett stycke atom med ID " << q.getID() << ", position (" << q.getPosX() << ", " << q.getPosY() << ", " << q.getPosZ() << ") och hastighet (" << q.getVelocityX() << ", " << q.getVelocityY() << ", " << q.getVelocityZ() << ")!" << endl;
	

	char exit;

	cin >> exit;

	return 0;
}