#include "Application.h"

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	gui = new ModuleGUI();
	
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	physics = new ModulePhysics3D();
	world = new ModuleWorld();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(world);
	AddModule(physics);
	
	
	// Renderer last!
	AddModule(renderer3D);
	//GUI Last, it should print on the top.
	AddModule(gui);


	window->name = "Window";
	camera->name = "Camera";
	input->name = "Input";
	physics->name = "Physics";
	renderer3D->name = "Renderer3D";
	gui->name = "Gui";
	world->name = "World";
}

Application::~Application()
{
	std::list<Module*>::iterator item = list_modules.end();

	while (item != list_modules.begin())
	{
		item--;
		delete (*item);
		
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules

	config = new Config();
	config->Init();
	config->Load();

	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		
		ret = (*item)->Init();
		(*item)->module_timer = new Timer();
		item++;
	}

	
	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		
		ret = (*item)->Start();
		item++;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		//PreUpdate ms
		(*item)->StartTimer();
		
		ret = (*item)->PreUpdate(dt);
	
		//Pause timer
		(*item)->PauseTimer();
		
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		//Update ms
		(*item)->ResumeTimer();

		ret = (*item)->Update(dt);
		
		//Pause timer
		(*item)->PauseTimer();
		
		item++;
		
	}
	item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		
		//PostUpdate ms
		(*item)->ResumeTimer();
		
		ret = (*item)->PostUpdate(dt);
		
		//Pause timer
		(*item)->StopTimer();
		//LOG("%i", (*item)->module_timer->Read());
		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		
		ret = (*item)->CleanUp();
		item++;
		
	}
	config->Save();
	return ret;
}

bool Application::Options()
{
	if (ImGui::CollapsingHeader("Application"))
	{
		std::list<Module*>::iterator item = App->list_modules.begin();

		while (item != App->list_modules.end())
		{

			const char* name = (*item)->name.c_str();

			(*item)->performance[(*item)->performance_offset] = (*item)->module_timer->Read_ms();
			(*item)->performance_offset = ((*item)->performance_offset + 1) % IM_ARRAYSIZE((*item)->performance);

			ImGui::PlotHistogram((char*)name, (*item)->performance, IM_ARRAYSIZE((*item)->performance), 0, name, 0.0f, 30.f, ImVec2(0, 40));
			item++;
		}
	}
	return true;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}