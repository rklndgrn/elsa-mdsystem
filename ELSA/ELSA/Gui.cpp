#include "Gui.h"

Gui::Gui()
{
	_name = "ELSA";
	_plotVisible = false;
	_visualVisible = false;
	_mainVisible = true;
	_exitPressed = false;
	_crystalType = "fcc";
	_latticeConstant = 408.53e-12;
	_epsilon = 0.34*(1.6021766208E-19);
	_sigma = 2.65e-10;
	_cutOffDistance = 1.5*408.53e-12;
	_mass = 39.948*(1.660539040e-27);
	_timeStep = 1e-14;
	_simulationTime = 20e-14;
	_numberOfUnitCellsX = 5;
	_numberOfUnitCellsY = 5;
	_numberOfUnitCellsZ = 5;
	_temperature = 10;
	_collisionPercentage = 0.2;
	_thermostat = false;
	_2D = false;
}

void Gui::setupGui(GLFWwindow *window)
{

	const char* glsl_version = "#version 330";

	//Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsLight();

	io.Fonts->AddFontFromFileTTF("imgui-master/misc/fonts/Roboto-Medium.ttf", 16.0f);
}

void Gui::show()
{
	_visible = true;
}

bool Gui::visible() const
{
	return _visible;
}

bool Gui::simulate() const
{
	return _simulate;
}

void Gui::stopSimulate()
{
	_simulate = false;
	_simulationWindow = false;
	_plotVisible = true;
}

void Gui::hide()
{
	_visible = false;
}

void Gui::showHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

std::string Gui::getCrystalType()
{
	return _crystalType;
}

double Gui::getTemperature()
{
	return _temperature;
}

double Gui::getTimeStep()
{
	return _timeStep;
}

double Gui::getSimulationTime()
{
	return _simulationTime;
}

bool Gui::isThermostat()
{
	return _thermostat;
}

bool Gui::is2D()
{
	return _2D;
}

int Gui::getNumberOfUnitCellsX()
{
	return _numberOfUnitCellsX;
}

int Gui::getNumberOfUnitCellsY()
{
	return _numberOfUnitCellsY;
}

int Gui::getNumberOfUnitCellsZ()
{
	return _numberOfUnitCellsZ;
}

double Gui::getLatticeConstant()
{
	return _latticeConstant;
}

double Gui::getEpsilon()
{
	return _epsilon;
}

double Gui::getSigma()
{
	return _sigma;
}

double Gui::getCutOffDistance()
{
	return _cutOffDistance;
}

double Gui::getMass()
{
	return _mass;
}

double Gui::getCollisionPercentage()
{
	return _collisionPercentage;
}

bool Gui::showCrystalSelector(const char* label)
{
	static int style_idx = 1;
	const char* items[] = { "SC", "FCC" };
	if (ImGui::Combo(label, &style_idx, items, IM_ARRAYSIZE(items)))
	{
		switch (style_idx)
		{
		case 0: _crystalType = "sc"; break;
		case 1: _crystalType = "fcc"; break;
		}
		return true;
	}
	return false;
}

bool Gui::showMaterialSelector(const char* label)
{
	static int style_idx = 0;
	const char* items[] = { "Ag: Silver", "Ar: Argon" };
	if (ImGui::Combo(label, &style_idx, items, IM_ARRAYSIZE(items)))
	{
		switch (style_idx)
		{
			// Data for Silver
		case 0: _latticeConstant = 408.53e-12;
				_epsilon = 0.34*(1.6021766208E-19);
				_sigma = 2.65e-10;
				_mass = 107.8682*(1.660539040e-27);
				break;
			// Data for Argon
		case 1: _latticeConstant = 525.6e-12;
				_epsilon = 0.0104*(1.6021766208E-19);
				_sigma = 3.40e-10;
				_mass = 39.948*(1.660539040e-27);
				break;
		}
		return true;
	}
	return false;
}

void Gui::handleFrame()
{
	//Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Gui::handleMenu(double elapsedTime, double totalTime)
{
	//Menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Open result file", NULL, &_loadResultWindow);
			ImGui::MenuItem("Save result file", NULL, &_saveResultWindow);
	
			if (ImGui::MenuItem("Exit", NULL)) { _exitPressed = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Main window", NULL, &_mainVisible);
			//ImGui::MenuItem("Visualization window", NULL, &show_visual_window);
			ImGui::MenuItem("Plot window", NULL, &_plotVisible);
			ImGui::MenuItem("Visualization", NULL, &_visualVisible);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::BeginMenu("Style"))
			{
				if (ImGui::MenuItem("Light", "Ctrl+L"))
				{
					ImGui::StyleColorsLight();
					glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
				}
				if (ImGui::MenuItem("Dark", "Ctrl+D"))
				{
					ImGui::StyleColorsDark();
					glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
				}
				if (ImGui::MenuItem("Classic", "Ctrl+C"))
				{
					ImGui::StyleColorsClassic();
					glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	if (_mainVisible)
	{
		ImGui::Begin("ELSA MD-system", &_mainVisible, window_flags);
		ImGui::SetWindowSize(ImVec2(560, 640));
		ImGui::SetWindowPos(ImVec2(60, 140));
		handleCollapsingHeaders();
		simulateButtonHandler();
		stopButtonHandler();

		//if (_simulate)
		//{
			//ImGui::Text("Simulating...");// , _counter);
			//handleProgressBar(elapsedTime, totalTime);
			
		//}
		ImGui::End();
	}
	handlePlots();
	saveResultsWindow();
	loadResultsWindow();
	visualWindow();
	simulationWindow(elapsedTime, totalTime);
}

void Gui::visualWindow()
{
	bool auto_resize = false;
	//ImGuiWindowFlags window_flags = 0;
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	flags |= ImGuiWindowFlags_NoResize;
	if (_visualVisible)
	{
		ImGui::Begin("Visualization", &_visualVisible, flags);
		ImGui::SetWindowSize(ImVec2(400, 50));
		ImGui::Text("Enable moving camera by holding space");
		ImGui::Text("Strafe by using w,a,s & d");
		ImGui::Text("Rotate by using up,left,down & right");
		ImGui::End();
	}


}

void Gui::simulationWindow(double elapsedTime, double totalTime)
{
	bool auto_resize = false;
	//ImGuiWindowFlags window_flags = 0;
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	flags |= ImGuiWindowFlags_NoResize;
	if (_simulationWindow)
	{
		ImGui::Begin("Simulation running...", &_simulationWindow, flags);
		ImGui::SetWindowSize(ImVec2(900, 60));
		handleProgressBar(elapsedTime, totalTime);
		stopButtonHandler();
		//if (!_simulate) { _simulationWindow = false; _simulate = false; }
		ImGui::End();
	}
}

void Gui::loadResultsWindow()
{
	bool auto_resize = false;
	//ImGuiWindowFlags window_flags = 0;
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	flags |= ImGuiWindowFlags_NoResize;
	if (_loadResultWindow)
	{
		ImGui::Begin("Load results file...", &_loadResultWindow, flags);
		ImGui::SetWindowSize(ImVec2(500, 110));
		ImGui::Text("Enter the name of the file you want to open:");
		static char buf1[64] = "./SaveData/"; ImGui::InputText(".txt", buf1, 64);
		if (ImGui::Button("Load")) {
			strcat(buf1, ".txt");
			std::ifstream myFile;
			myFile.open(buf1); 
			if (!myFile.is_open()) { _unableToOpenFile = true; }
			else {
				std::string dummy;
				int sizeOfFile{ 0 }, i{ 0 };
				while (!myFile.eof()) { getline(myFile, dummy); sizeOfFile++; }
				myFile.close(); myFile.open(buf1);
				if (_temp == NULL) { _temp = new double[sizeOfFile]; }
				if (_totalEnergy == NULL) { _totalEnergy = new double[sizeOfFile]; }
				if (_potentialEnergy == NULL) { _potentialEnergy = new double[sizeOfFile]; }
				if (_kineticEnergy == NULL) { _kineticEnergy = new double[sizeOfFile]; }
				std::getline(myFile, dummy);
				while (!myFile.eof())
				{
					myFile >> _temp[i];
					myFile >> _totalEnergy[i];
					myFile >> _potentialEnergy[i];
					myFile >> _kineticEnergy[i];
					i++;
				}
				myFile.close();
				_loadResultWindow = false;
				_unableToOpenFile = false;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) { _loadResultWindow = false; };
		if(_unableToOpenFile) { ImGui::Text("Unable to open file..."); }
		ImGui::End();
	}
}

void Gui::saveResultsWindow()
{
	bool auto_resize = false;
	//ImGuiWindowFlags window_flags = 0;
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	flags |= ImGuiWindowFlags_NoResize;
	if (_saveResultWindow)
	{
		ImGui::Begin("Save results file...", &_saveResultWindow, flags);
		ImGui::SetWindowSize(ImVec2(500, 110));
		ImGui::Text("Enter the name of the file you want to save to:");
		static char buf1[64] = "./SaveData/"; ImGui::InputText(".txt", buf1, 64);
		if (ImGui::Button("Save")) {
			strcat(buf1, ".txt");
			std::ofstream myFile;
			myFile.open(buf1);

			myFile << "Temperature [K] " << "Total energy [J] " << "Potential energy [J] " << "Kinetic energy [J] " << std::endl;
				for (int i = 0; i < (int)(floor(_simulationTime / _timeStep)); i++)
				{
					myFile << _temp[i] << " " << _totalEnergy[i] << " " << _potentialEnergy[i] << " " << _kineticEnergy[i] << std::endl;
				}
				myFile.close();
				_saveResultWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) { _saveResultWindow = false; };
		ImGui::End();
	}
}

void Gui::handlePlots()
{
	bool auto_resize = false;
	//ImGuiWindowFlags window_flags = 0;
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	flags |= ImGuiWindowFlags_NoResize;
	
	if (_plotVisible)
	{
	ImGui::Begin("Results", &_plotVisible, flags);
	ImGui::SetWindowSize(ImVec2(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height - 22));
	ImGui::SetWindowPos(ImVec2(0, 22));


	ImGui::Text("Potential energy: \n %E", _potentialEnergy[(int)(floor(_simulationTime / _timeStep)) - 1]);
	ImGui::SameLine();
	ImGui::Text("Kinetic energy: \n %E", _kineticEnergy[(int)(floor(_simulationTime / _timeStep)) - 1]);
	ImGui::SameLine();
	ImGui::Text("Total energy: \n %E", _totalEnergy[(int)(floor(_simulationTime / _timeStep)) - 1]);
	ImGui::SameLine();
	ImGui::Text("Temperature: \n %E", _temp[(int)(floor(_simulationTime / _timeStep)) - 1]);
		if (ImGui::CollapsingHeader("Potential energy"))
		{
			float potEnD[5000];
			float max = -1E15;
			float min = 1E15;

			for (int i = 0; i < (int)(floor(_simulationTime / _timeStep) - 1); i++)
			{
				potEnD[i] = static_cast<float>(_potentialEnergy[i+1]);
				if (potEnD[i] > max) { max = potEnD[i]; }
				else if (potEnD[i] < min) { min = potEnD[i]; }
			}

			ImGui::PlotLines("", potEnD, (int)(floor(_simulationTime / _timeStep)) - 1, 0, "Potential energy [J]", min, max, ImVec2(1700, 480));

			
		}
		if (ImGui::CollapsingHeader("Kinetic energy"))
		{
			float kinEnD[5000];
			float max = -1E15;
			float min = 1E15;

			for (int i = 0; i < (int)(floor(_simulationTime / _timeStep) - 1); i++)
			{
				kinEnD[i] = static_cast<float>(_kineticEnergy[i+1]);
				if (kinEnD[i] > max) { max = kinEnD[i]; }
				else if (kinEnD[i] < min) { min = kinEnD[i]; }
			}
			ImGui::PlotLines("", kinEnD, (int)(floor(_simulationTime / _timeStep)) - 1, 0, "Kinetic energy [J]", min, max, ImVec2(1700, 480));
		}
		if (ImGui::CollapsingHeader("Total energy"))
		{
			float totEn[5000];
			float max = -1E5;
			float min = 1E5;

			for (int i = 0; i < (int)(floor(_simulationTime / _timeStep) - 1); i++)
			{
				totEn[i] = static_cast<float>(_totalEnergy[i+1]);
				if (totEn[i] > max) { max = totEn[i]; printf("Vi kom in! nuvarande max: %f tot en: %f \n", max, totEn[i]); }
				else if (totEn[i] < min) { min = totEn[i]; }
			}
			//printf("Min: %f Max: %f", min, max);

			ImGui::PlotLines("", totEn, (int)(floor(_simulationTime / _timeStep)) - 1, 0, "Total energy [J]", min, max, ImVec2(1700, 480));
		}
		if (ImGui::CollapsingHeader("Temperature"))
		{
			float temp[5000];
			float max = -1E15;
			float min = 1E15;

			for (int i = 0; i < (int)(floor(_simulationTime / _timeStep) - 1); i++)
			{
				temp[i] = static_cast<float>(_temp[i+1]);
				if (temp[i] > max) { max = temp[i]; }
				else if (temp[i] < min) { min = temp[i]; }
			}
			ImGui::PlotLines("", temp, (int)(floor(_simulationTime / _timeStep)) - 1, 0, "Temperature [K]", min, max, ImVec2(1700, 480));
		}

	ImGui::End();
	}
}

void Gui::handleConfigurationHeader()
{

	if (ImGui::CollapsingHeader("Simulation parameters"))
	{
		/*ImGuiIO& io = ImGui::GetIO();

		if (showCrystalSelector("CrystalSelector"))
			showCrystalSelector("cr");*/

		//ImGui::Text("Temperature: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Temperature [K]", &_temperature, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Time step length: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Time step length [s]", &_timeStep, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Simulation time: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Simulation time [s]", &_simulationTime, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Collision frequency: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Collision frequency [Hz]", &_collisionPercentage, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//Cut off distance
		ImGui::InputDouble("Cut off distance [m]", &_cutOffDistance, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Number of unit cells x: ");
		//ImGui::SameLine();
		ImGui::InputInt("Number of unit cells x [1]", &_numberOfUnitCellsX);
		/*ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");*/

		//ImGui::Text("Number of unit cells y: ");
		//ImGui::SameLine();
		ImGui::InputInt("Number of unit cells y [1]", &_numberOfUnitCellsY);
		/*ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");*/

		//ImGui::Text("Number of unit cells z: ");
		//ImGui::SameLine();
		ImGui::InputInt("Number of unit cells z [1]", &_numberOfUnitCellsZ);
		/*ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");*/

		ImGui::Checkbox("Visualization", &_visualization);      // Edit bools storing our window open/close state
											
		ImGui::Checkbox("Anderson thermostat", &_thermostat);
		ImGui::Checkbox("2D simulation", &_2D);
	}

}
void Gui::handleSettingsHeader()
{
	if (ImGui::CollapsingHeader("Material parameters"))
	{
		ImGuiIO& io = ImGui::GetIO();

		if (showMaterialSelector("MaterialSelector"))
			showMaterialSelector("mt");

		if (showCrystalSelector("CrystalSelector"))
			showCrystalSelector("cr");

		//ImGui::Text("Lattice constant: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Lattice constant [m]", &_latticeConstant, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Epsilon: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Epsilon [m]", &_epsilon, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Sigma: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Sigma [J]", &_sigma, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Cut off distance: ");
		//ImGui::SameLine();
		/*ImGui::InputDouble("Cut off distance [m]", &_cutOffDistance, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");*/

		//ImGui::Text("Mass: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Mass [kg]", &_mass, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");
	}
}

void Gui::handleProgressBar(double elapsedTime, double totalTime)
{
	// Animate a simple progress bar
	static float progress = 0.0f, progress_dir = 1.0f;

	float quota = static_cast<float>(elapsedTime / totalTime);
	//printf("Progress: %f \n", quota);

	progress = progress_dir * quota;// *ImGui::GetIO().DeltaTime;

	// Typically we would use ImVec2(-1.0f,0.0f) to use all available width, or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
	ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	if (_simulate && !_initializing) { ImGui::Text("Simulating..."); }
	else { ImGui::Text("Initializing..."); }

}

void Gui::simulateButtonHandler()
{
	
	if (ImGui::Button("Simulate"))
	{
		_simulationWindow = true;
		_simulate = true;
	}

}

void Gui::stopButtonHandler()
{
	ImGui::SameLine();
	ImGui::PushID(1);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV( 0.0f, 1.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 1.0f, 0.9f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 1.0f, 0.8f));
	if (ImGui::Button("Stop Simulation")) { _simulate = false; }
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

bool Gui::VisualVisible() const
{
	return _visualVisible;
}

bool Gui::exitPressed() const
{
	return _exitPressed;
}

void Gui::handleCollapsingHeaders()
{
	handleConfigurationHeader();
	handleSettingsHeader();
}

