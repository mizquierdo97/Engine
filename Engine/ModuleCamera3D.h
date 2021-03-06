#ifndef _MODULECAMERA
#define _MODULECAMERA
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D( bool start_enabled = true);
	~ModuleCamera3D();

	update_status Update(float dt);
	
	bool Init();
	bool Start();
	bool CleanUp();
	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	float* GetViewMatrix();
	void UpdateCamProp();

	void ModuleCamera3D::Follow(PhysBody3D* body, float min, float max, float height)
	{
		min_following_dist = min;
		max_following_dist = max;
		following_height = height;
		following = body;
	}

	GameObject * Raycast(const LineSegment & segment, float  &dist)const;
	void Recursivetest(const LineSegment& segment, float& dist, GameObject** closest_object)const;

private:

	void CalculateViewMatrix();

public:


	float3 X, Y, Z, Position, Reference;
	LineSegment picking;
	ComponentCamera* dummyfrustum;
private:
	
	
	float3* last_hit;
	PhysBody3D* following = NULL;
	float min_following_dist;
	float max_following_dist;
	float following_height;
	float4x4 ViewMatrix, ViewMatrixInverse;
	float2 mouse_pos = float2(0, 0);
};

#endif