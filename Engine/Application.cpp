#include "Application.h"
#include "imgui_dock.h"


Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	gui = new ModuleGUI();
	
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	physics = new ModulePhysics3D();
	world = new ModuleWorld();
	assimp = new ModuleAssimp();
	resources = new ModuleResourceManager();
	filesystem = new FileSystem();
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	
	AddModule(camera);
	AddModule(input);
	AddModule(physics);
	AddModule(assimp);
	AddModule(world);
	AddModule(resources);
	
	
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
	assimp->name = "Assimp";
	resources->name = "Resource Manager";

}

Application::~Application()
{
	std::list<Module*>::iterator item = list_modules.end();

	while (item != list_modules.begin())
	{
		item--;
		RELEASE(*item);
		
	}
}



void Application::SetGameMode( Gamestatus st)
{
	previousGameStatus = gameStatus;
	gameStatus = st;

	switch(gameStatus)
	{
	case PLAY:

		rtime.frame_time = 1.0f;
		if (previousGameStatus == STOP) {
			world->SaveScene("Saveeditor");
			gtime.gtimer.Start();
		}
	
		else if(previousGameStatus == PAUSE)
		{
			gtime.gtimer.Resume();
		}

		break;

	case PAUSE:

		gtime.gtimer.Pause();

		if (previousGameStatus == PLAY)
		{
			rtime.frame_time = 0.0f;
			
		}
		else 
		{
			gameStatus = STOP;
		}
		break;

	case STOP:
		rtime.frame_time = 1.0f;
		gtime.gtimer.Stop();
		
		if (previousGameStatus == PLAY)
		{
			world->LoadScene("Saveeditor");
			
		}
		break;

	case NEXT_FRAME:
		rtime.frame_time = 0.0f;
		break;
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
	gtime.gtimer.Stop();
	gtime.gtimer.actual_ms = 0;
	filesystem->InitFileSystem();
	ms_timer.Start();
	rtime.startTime.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = ((float)ms_timer.Read() / 1000.0f)* rtime.frame_time;
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
		RELEASE((*item)->module_timer);
		
		item++;
		
	}

	config->Save();	
	RELEASE(config);
	RELEASE(filesystem);
	return ret;
}

bool Application::Options()
{
	if (win_active) {
		ImGui::SetNextWindowSize(ImVec2(350,600), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Application", &win_active)) {

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
		ImGui::Separator();
		ImGui::Text(" Real time: %i", rtime.startTime.Read() / 1000);
		ImGui::Text(" Game time: %i", gtime.gtimer.Read() / 1000);
		ImGui::Text(" mstimer: %i", ms_timer.Read());

		ImGui::End();
	}
	return true;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

