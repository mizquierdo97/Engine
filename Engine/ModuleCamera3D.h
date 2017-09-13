#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	void ModuleCamera3D::Follow(PhysBody3D* body, float min, float max, float height)
	{
		min_following_dist = min;
		max_following_dist = max;
		following_height = height;
		following = body;
	}

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;

private:

	PhysBody3D* following = NULL;
	float min_following_dist;
	float max_following_dist;
	float following_height;
	mat4x4 ViewMatrix, ViewMatrixInverse;
};