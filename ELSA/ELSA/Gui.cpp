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
	_collisionFrequency = 10;
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

double Gui::getCollisionFrequency()
{
	return _collisionFrequency;
}

bool Gui::showCrystalSelector(const char* label)
{
	static int style_idx = -1;
	if (ImGui::Combo(label, &style_idx, "SC\0FCC\0"))
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

void Gui::handleFrame()
{
	//Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Gui::handleMenu()
{
	//Menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem("Exit", NULL))
			{
				_exitPressed = true;
			}
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
		handleCollapsingHeaders();
		simulateButtonHandler();
		if (_simulate)
		{
			ImGui::Text("Simulating...");// , _counter);
		}
		ImGui::End();
	}

}

void Gui::handlePlots(const float* potEn)//, const float* kinEn, const float* totEn, const float* temp)
{
	if (_plotVisible)
	{
	ImGui::Begin("Results", &_plotVisible);
	//if (_simulate)
	//{
		if (ImGui::CollapsingHeader("Potential energy"))
		{
			ImGui::PlotLines("", potEn, (int)(floorf(_simulationTime / _timeStep)), 0, "Potential energy", 1.0f, 100.0f, ImVec2(1700, 480));
		}
		/*if (ImGui::CollapsingHeader("Kinetic energy"))
		{
			ImGui::PlotLines("", kinEn, (int)(floorf(_simulationTime / _timeStep)), 0, "Kinetic energy", 1.0f, 100.0f, ImVec2(1700, 480));
		}
		if (ImGui::CollapsingHeader("Total energy"))
		{
			ImGui::PlotLines("", totEn, (int)(floorf(_simulationTime / _timeStep)), 0, "Total energy", 1.0f, 100.0f, ImVec2(1700, 480));
		}
		if (ImGui::CollapsingHeader("Temperature"))
		{
			ImGui::PlotLines("", temp, (int)(floorf(_simulationTime / _timeStep)), 0, "Temperature", 1.0f, 100.0f, ImVec2(1700, 480));
		}*/
	//}
	ImGui::End();
	}
}

void Gui::handleConfigurationHeader()
{

	if (ImGui::CollapsingHeader("Simulation parameters"))
	{
		ImGuiIO& io = ImGui::GetIO();

		if (showCrystalSelector("CrystalSelector"))
			showCrystalSelector("cr");

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
		ImGui::InputDouble("Collision frequency [Hz]", &_collisionFrequency, 0.0f, 0.0f, "%e");
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
		//ImGui::Text("Lattice constant: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Lattice constant [m]", &_latticeConstant, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Epsilon: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Epsilon [1]", &_epsilon, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Sigma: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Sigma [1]", &_sigma, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Cut off distance: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Cut off distance [m]", &_cutOffDistance, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Mass: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Mass [kg]", &_mass, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");
	}
}

void Gui::simulateButtonHandler()
{
	if (ImGui::Button("Simulate"))
	{
		_simulate = true;
	}
	//ImGui::End();
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

