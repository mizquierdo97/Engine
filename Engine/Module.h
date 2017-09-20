#pragma once

class Application;
struct PhysBody3D;

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
#include <string>

class Module
{
private :
	bool enabled;
	
public:
	Application* App;

	Module(Application* parent, bool start_enabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	virtual void StartTimer() {
		this->module_timer->Start();
	};
	virtual void PauseTimer() {
		this->module_timer->Pause();
	};
	virtual void ResumeTimer() {
		this->module_timer->Resume();
	};
	virtual void StopTimer() {
		this->module_timer->Stop();
	}
	
public:
	Timer* module_timer;
	std::string name = "";
	float performance[90] = { 0 };
	int performance_offset = 0;
};