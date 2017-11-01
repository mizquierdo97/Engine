#pragma once
#include "Module.h"
#include "Globals.h"


#include <list>
#include "Bullet/include/btBulletDynamicsCommon.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 
#define GRID_SIZE 10;
class DebugDrawer;
struct PhysBody3D;


class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D( bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const float3& anchorA, const float3& anchorB);
	void AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const float3& anchorA, const float3& anchorB, const float3& axisS, const float3& axisB, bool disable_collision = false);

	btDiscreteDynamicsWorld*			world;

	void DrawNormals(ComponentMesh* obj);
private:

	bool debug;

	btDefaultCollisionConfiguration*	collision_conf;
	btCollisionDispatcher*				dispatcher;
	btBroadphaseInterface*				broad_phase;
	btSequentialImpulseConstraintSolver* solver;

	DebugDrawer*						debug_draw;

	std::list<btCollisionShape*> shapes;
	std::list<PhysBody3D*> bodies;
	std::list<btDefaultMotionState*> motions;
	std::list<btTypedConstraint*> constraints;
	
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer()
	{
		line.pos = float3(0, 0, 0);
	};

	void drawGrid(int grid_size);
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {};
	void drawLine(float3 from, float3 to, float3 color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	math::Line line;
	

	
};