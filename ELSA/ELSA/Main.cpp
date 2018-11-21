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

	visual myVis;

	Gui myGui;
	myGui.setupGui(myVis.getWindow());

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

		myGui.handleMenu(0,1); //1,1 does nothing
		

		if (myGui.simulate())
		{
			myGui._initializing = true;
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
				myGui.getCollisionPercentage(),
				myGui.isThermostat(),
				myGui.is2D(),
				myMaterial);

			World myWorld(myParameters);

			double deltaT = myParameters.getTimeStep();
			double** potArray = myWorld.getResults().getPotentialEnergy();
			double** kinArray = myWorld.getResults().getKineticEnergy();
			double** totArray = myWorld.getResults().getTotalEnergy();
			double** tempArray = myWorld.getResults().getTemperature();

			int index{ 0 };

			myGui._initializing = false;

			for (double t = deltaT; t < myParameters.getSimulationTime() - 0.5*deltaT; t += deltaT)
			{
				if (myGui.simulate())
				{
					glUseProgram(0);
					ImGui::Render();

					//myGui.handleProgressBar(t,myParameters.getSimulationTime());

					int display_w, display_h;
					glfwMakeContextCurrent(myVis.getWindow());
					glfwGetFramebufferSize(myVis.getWindow(), &display_w, &display_h);
					glViewport(0, 0, display_w, display_h);
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

					glfwPollEvents();
					glfwSwapBuffers(myVis.getWindow());

					//Here's where the magic happens
					myWorld.performSimulation(t, 10);
					//---//

					myGui.handleFrame();
					myGui.handleMenu(t, myParameters.getSimulationTime());
				}

			}
			myGui.stopSimulate();
			myGui._kineticEnergy = *kinArray;
			myGui._totalEnergy = *totArray;
			myGui._potentialEnergy = *potArray;
			myGui._temp = *tempArray;
		}

		//myGui.handlePlots();// , kinenen, totenen, tempen);

		/*bool tesr = false;
		ImGui::ShowDemoWindow(&tesr);*/

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

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(myVis.getWindow());
	glfwTerminate();

	return 0;
}
