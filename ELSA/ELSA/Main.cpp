#include "./Atom/Atom.h"
#include "./Material/Material.h"
#include "./Parameters/Parameters.h"
#include "./Cell/Cell.h"
#include "./Simulation/Simulation.h"
#include "./World/World.h"
#include "visual.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main()
{
	//Material anotherMaterial("fcc", 408.53e-12, 0.34*(1.6021766208E-19), 2.65e-10, 1.5*408.53e-12, 39.948*(1.660539040e-27));
	//Parameters myParameters(1e-14, 20e-14, 5, 5, 5, 10, 10, false, false, anotherMaterial);
	//World myWorld(myParameters);
	visual myVis;

	Gui myGui;
	myGui.setupGui(myVis.getWindow());

	ofstream myFilePos;
	myFilePos.open("BengtPos.txt");
	while ((glfwGetKey(myVis.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(myVis.getWindow()) == 0))// || myGui.exitPressed() )
	{
		myGui.handleFrame();

		if (myGui.VisualVisible())
			myVis.setAtomsVisible(true);
		else
			myVis.setAtomsVisible(false);
		myVis.mainLoopVisual();

		// -------------------------------- MENU -----------------------------------------

		myGui.handleMenu();
		

		if (myGui.simulate())
		{
			Material myMaterial(
				myGui.getCrystalType(),
				myGui.getLatticeConstant(),
				myGui.getEpsilon(),
				myGui.getSigma(),
				myGui.getCutOffDistance(),
				myGui.getMass());

			Parameters myParameters(
				myGui.getTimeStep(),
				myGui.getSimulationTime(),
				myGui.getNumberOfUnitCellsX(),
				myGui.getNumberOfUnitCellsY(),
				myGui.getNumberOfUnitCellsZ(),
				myGui.getTemperature(),
				myGui.getCollisionFrequency(),
				myGui.isThermostat(),
				myGui.is2D(),
				myMaterial);

			World myWorld(myParameters);

			double deltaT = myParameters.getTimeStep();
			double** potArray = myWorld.getResults().getTotalEnergy();
			double** kinArray = myWorld.getResults().getKineticEnergy();
			double** tempArray = myWorld.getResults().getTemperature();
			//double* U = *potArray;
			double* K = *kinArray;
			//double* T = *tempArray;
			int index{ 0 };

			for (double t = deltaT; t < myParameters.getSimulationTime() - 0.5*deltaT; t += deltaT)
			{
				if (deltaT > 0)
				{
					myWorld.updateCells();
					myWorld.updateNeighbourList();
				}

				myWorld.calcPotentialAndForce(t);
				myWorld.solveEquationsOfMotion(t);
				myWorld.calcPressure(t);
				//U = *potArray;
				K = *kinArray;
				
				//T = *tempArray;
				index = (int)round(t / deltaT);
				myFilePos << K[index] << " ";
			}
			myGui.stopSimulate();

			myGui._kinArray = reinterpret_cast<float*>(*kinArray);
		}

		myGui.handlePlots(myGui._kinArray);// , kinenen, totenen, tempen);

		bool tesr = true;
		ImGui::ShowDemoWindow(&tesr);

		// Rendering
		glUseProgram(0);
		ImGui::Render();

		int display_w, display_h;
		glfwMakeContextCurrent(myVis.getWindow());
		glfwGetFramebufferSize(myVis.getWindow(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(myVis.getWindow());
	}

	/*double** potArray = myWorld.getResults().getPotentialEnergy();
	double** kinArray = myWorld.getResults().getKineticEnergy();
	double** tempArray = myWorld.getResults().getTemperature();
	double* U = *potArray;
	double* K = *kinArray;
	double* T = *tempArray;
	int index{ 0 };
	

	
	ofstream myFilePos;
	myFilePos.open("BengtPos.txt");
	


	cout << "Time 0: " << endl;
	cout << "   Potential energy: " << U[index] << endl;
	cout << "   Kinetic energy: " << K[index] << endl;
	cout << "   Total energy: " << U[index] + K[index] << endl;
	cout << "   Temperature: " << T[index] << endl;
	
	
	Atom* a = myWorld.getAtomInAtomList(62);
	Atom* b = myWorld.getAtomInAtomList(63);
	myFilePos << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " " << b->getPositionX() << " " << b->getPositionY() << " " << b->getPositionZ() << " ";

	
	for (double t = deltaT; t < myParameters.getSimulationTime() - 0.5*deltaT; t += deltaT)
	{
		if (deltaT > 0)
		{
			myWorld.updateCells();
			myWorld.updateNeighbourList();
		}

		myWorld.calcPotentialAndForce(t);
		myWorld.solveEquationsOfMotion(t);
		myWorld.calcPressure(t);
		U = *potArray;
		K = *kinArray;
		T = *tempArray;
		index = (int)round(t / deltaT);


		myFilePos << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " " << b->getPositionX() << " " << b->getPositionY() << " " << b->getPositionZ() << " ";
		
		
		cout << "Time " << t << ": " << endl;
		cout << "   Potential energy: " << U[index] << endl;
		cout << "   Kinetic energy: " << K[index] << endl;
		cout << "   Total energy: " << U[index] + K[index] << endl;
		cout << "   Temperature: " << T[index] << endl << endl;
	}
	*/
	myFilePos.close();
	//cout << "Hello there!" << endl;

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(myVis.getWindow());
	glfwTerminate();

	return 0;
}
