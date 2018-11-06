#include "./Atom/Atom.h"
#include <iostream>

using namespace std;

int main()
{
	Atom a{1, 1, 2, 3};

	cout << "Hej, vi har en atom med ID " << a.getID() << ", position (" << a.getPosX() << ", " << a.getPosY() << ", " << a.getPosZ() << ") och hastighet (" << a.getVelocityX() << ", " << a.getVelocityY() << ", " << a.getVelocityZ() << ")!" << endl;


	a.setPosition(6, 2, 3);
	a.setVelocity(5, 3, 2);

	cout << "Fast nu har vi en atom med ID " << a.getID() << ", position (" << a.getPosX() << ", " << a.getPosY() << ", " << a.getPosZ() << ") och hastighet (" << a.getVelocityX() << ", " << a.getVelocityY() << ", " << a.getVelocityZ() << ")!" << endl;
	

	char exit;

	cin >> exit;

	return 0;
}