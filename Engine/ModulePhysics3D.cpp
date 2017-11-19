#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "MathGeoLib.h"

#include "ComponentMesh.h"
#ifdef _DEBUG
	#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
	#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif

ModulePhysics3D::ModulePhysics3D( bool start_enabled) : Module(start_enabled)
{
	debug = false;

	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	RELEASE( debug_draw);
	RELEASE(solver);
	RELEASE(broad_phase);
	RELEASE(dispatcher);
	RELEASE(collision_conf);
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	LOG("Creating 3D Physics simulation");
	bool ret = true;

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);
	

	// Big plane as ground
	{
		btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	world->stepSimulation(dt, 15);

	int numManifolds = world->getDispatcher()->getNumManifolds();
	for(int i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if(numContacts > 0)
		{
			PhysBody3D* pbodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pbodyB = (PhysBody3D*)obB->getUserPointer();

			if(pbodyA && pbodyB)
			{
				std::list<Module*>::iterator item = pbodyA->collision_listeners.begin();
				while(item != pbodyA->collision_listeners.end())
				{
					(*item)->OnCollision(pbodyA, pbodyB);
					
					(*item)++;
				}

				item = pbodyB->collision_listeners.begin();
				while (item != pbodyB->collision_listeners.end())
				{
					(*item)->OnCollision(pbodyB, pbodyA);

					(*item)++;
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	debug_draw->drawGrid(10);
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		world->debugDrawWorld();
	
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// Remove from the world all collision bodies
	for(int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}


	if(constraints.size() >0)
	for(std::list<btTypedConstraint*>::iterator item = constraints.begin(); item != constraints.end(); item = item++)
	{
		world->removeConstraint((*item));
		RELEASE(*item);
	}
	
	constraints.clear();

	if (motions.size() >0)
	for(std::list<btDefaultMotionState*>::iterator item = motions.begin(); item != motions.end(); item++)
		RELEASE(*item);

	motions.clear();

	if (shapes.size() >0)
	for(std::list<btCollisionShape*>::iterator item = shapes.begin(); item != shapes.end(); item++)
		RELEASE(*item);

	shapes.clear();

	if (bodies.size() >0)
	for(std::list<PhysBody3D*>::iterator item = bodies.begin(); item != bodies.end(); item++)
		RELEASE(*item);

	bodies.clear();
	
	RELEASE(world);

	return true;
}

// ---------------------------------------------------------


// ---------------------------------------------------------


// ---------------------------------------------------------
void ModulePhysics3D::AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const float3& anchorA, const float3& anchorB)
{
	btTypedConstraint* p2p = new btPoint2PointConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z), 
		btVector3(anchorB.x, anchorB.y, anchorB.z));
	world->addConstraint(p2p);
	constraints.push_back(p2p);
	p2p->setDbgDrawSize(2.0f);
}

void ModulePhysics3D::AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const float3& anchorA, const float3& anchorB, const float3& axisA, const float3& axisB, bool disable_collision)
{
	btHingeConstraint* hinge = new btHingeConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z),
		btVector3(axisA.x, axisA.y, axisA.z), 
		btVector3(axisB.x, axisB.y, axisB.z));

	world->addConstraint(hinge, disable_collision);
	constraints.push_back(hinge);
	hinge->setDbgDrawSize(2.0f);
}

void ModulePhysics3D::DrawNormals(ComponentMesh * obj)
{
	float3 color = { 1, 1 ,1 };
	Mesh mesh = ((ResourceMesh*)obj->GetResource())->obj_mesh;

	for (int i = 0; i < mesh.num_vertexs; i++) {
		float3 from = { mesh.vertexs[3 * i],  mesh.vertexs[3 * i + 1], mesh.vertexs[3 * i + 2] };
		
		float3 to = {from.x + mesh.norms[3 * i],from.y + mesh.norms[3 * i + 1], from.z + mesh.norms[3 * i + 2] };
		glPushMatrix();
	
		glMultMatrixf(obj->GetParent()->GetTransform()->GetMatrix().Transposed().ptr());
		debug_draw->drawLine(from, to,color);
		glPopMatrix();
	}
}

void DebugDrawer::drawGrid(int grid_size)
{
	
	float3 line_color =float3(1, 1 ,1);
	for (int i = -grid_size; i <= grid_size; i++)
	{
		float3 vect =float3((float)i, 0 , (float)-grid_size );
		float3 vect2 = float3((float)i, 0 , (float)grid_size);
		drawLine(vect, vect2, line_color);

		vect = { (float)-grid_size, 0 , (float)i };
		vect2 = { (float)grid_size, 0 , (float)i };
		drawLine(vect, vect2, line_color);

	}
}


// =============================================
void DebugDrawer::drawLine(float3 from, float3 to, float3 color)
{
	float3 start = float3(from.x, from.y, from.z);
	float3 end = float3(to.x, to.y, to.z);

	glBegin(GL_LINES);

	glColor4f(color.x, color.y, color.z,1.0f);
	glVertex3fv(&start[0]);
	glVertex3fv(&end[0]);
	glEnd();
	glColor4f(1, 1,1, 1.0f);
	
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	/*point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();*/
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
