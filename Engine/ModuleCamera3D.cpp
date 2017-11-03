#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "Component.h"
#include "ComponentMesh.h"

ModuleCamera3D::ModuleCamera3D( bool start_enabled) : Module( start_enabled)
{
	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 1.0f, 0.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	static float3 temp_vec = float3(0, 0, 0);
	static float num = 0;
	float wheel = 0;
	float3 newPos(0, 0, 0);
	float speed = 3.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;


	/*if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;
*/
	
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z*speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z*speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;
	
	
	wheel = App->input->GetMousepositionZ();
	if (wheel > 0) newPos -= Z * speed;
	else if (wheel < 0)newPos += Z * speed;

	
	Reference = temp_vec;

	Position += newPos;
	Reference += newPos;
	
	float3 max_dist = float3(0, 0, 0);
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && App->world->IsObjectSelected()) {

		GameObject* selected = App->world->GetSelectedObject();
		AABB selected_AABB = selected->GetGlobalBBox();
		float3 new_position = float3(0, 0, 0);
		num = 0;
		int i = 0;
		std::vector<GameObject*>::iterator item = App->world->obj_vector.begin();
		temp_vec = float3(0, 0, 0);
			
		while (item != App->world->obj_vector.end()) {
			ComponentMesh* temp_mesh = (*item)->GetMesh();

			
			temp_vec.x += selected_AABB.CenterPoint().x;
			temp_vec.y += selected_AABB.CenterPoint().y;
			temp_vec.z += selected_AABB.CenterPoint().z;
			num++;
			item++;
		}
		if (num > 0) {
			temp_vec /= num;
			num = 0;
		}
				
	 item = App->world->obj_vector.begin();

		 vec3 temp_vec2 = vec3(0, 0, 0);
		 while (item != App->world->obj_vector.end()) {
			 ComponentMesh* temp_mesh = (*item)->GetMesh();

			 temp_vec2.x = (selected_AABB.maxPoint.x);
			 temp_vec2.y = (selected_AABB.maxPoint.y);
			 temp_vec2.z = (selected_AABB.maxPoint.z);

			 if (Abs(max_dist.x) < Abs(temp_vec2.x))max_dist.x = temp_vec2.x;
			 if (Abs(max_dist.y) < Abs(temp_vec2.y))max_dist.y = temp_vec2.y;
			 if (Abs(max_dist.z) < Abs(temp_vec2.z))max_dist.z = temp_vec2.z;
				
			 item++;
		 }
		 Reference = temp_vec;

		
		Position = Reference + Z * (max_dist*1.5f - temp_vec).Length();
		

	}

	// Mouse motion ----------------
	
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT)== KEY_IDLE && App->world->IsObjectSelected())
	{
		int dx = -App->input->GetMousepositionXMotion();
		int dy = -App->input->GetMousepositionYMotion();

		GameObject* selected = App->world->GetSelectedObject();
		AABB selected_AABB = selected->GetGlobalBBox();

		float Sensitivity = 0.25f;

		
		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;
			Quat rotation;
			rotation.SetFromAxisAngle(float3(0.0f, 1.0f, 0.0f), DeltaX*DEGTORAD);
			
			X = rotation.Transform(X);// rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotation.Transform(Y);
			Z = rotation.Transform(Z);
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;
			Quat rotation;
			rotation.SetFromAxisAngle(X, DeltaY*DEGTORAD);
		

			Y = rotation.Transform(Y);
			Z = rotation.Transform(Z);

			if (Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = X.Cross(Z);
			}
		}
		Position = Reference + Z * (Position).Length();

		if (App->world->obj_vector.size() > 0) {

			std::vector<GameObject*>::iterator item = App->world->obj_vector.begin();
			temp_vec = float3(0, 0, 0);
			num = 0;
			int i = 0;
			while (item != App->world->obj_vector.end()) {
				ComponentMesh* temp_mesh = (*item)->GetMesh();
				temp_vec.x += selected_AABB.CenterPoint().x;
				temp_vec.y += selected_AABB.CenterPoint().y;
				temp_vec.z += selected_AABB.CenterPoint().z;
				num++;
				item++;
			}
			if (num > 0) {
				temp_vec /= num;
				num = 0;
			}
		}	
		
	}

	

	else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT  && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMousepositionXMotion();
		int dy = -App->input->GetMousepositionYMotion();

		
		float Sensitivity = 0.25f;

		

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			Quat rotation;
			rotation.SetFromAxisAngle(float3(0.0f, 1.0f, 0.0f), DeltaX*DEGTORAD);

			X = rotation.Transform(X);// rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotation.Transform(Y);
			Z = rotation.Transform(Z);
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;
			Quat rotation;
			rotation.SetFromAxisAngle(X, DeltaY*DEGTORAD);


			Y = rotation.Transform(Y);
			Z = rotation.Transform(Z);

			if (Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = X.Cross(Z);
			}

		}
	

	}


	
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;
		
	// Recalculate matrix -------------
	CalculateViewMatrix();
	
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}
	
	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3 &Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	
	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return (float*)ViewMatrix.v;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}