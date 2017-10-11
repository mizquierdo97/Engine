#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"


ModuleCamera3D::ModuleCamera3D( bool start_enabled) : Module( start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 1.0f, 0.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
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

	static vec3 temp_vec = vec3(0, 0, 0);
	static float num = 0;

	vec3 newPos(0, 0, 0);
	float speed = 3.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	/*if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;
*/
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	if (num > 0) {
		temp_vec /= num;
		num = 0;
	}
	Reference = temp_vec;

	Position += newPos;
	Reference += newPos;
	vec3 max_dist = vec3(0, 0, 0);
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {

	
	
		vec3 new_position = vec3(0, 0, 0);
		num = 0;
		int i = 0;
		/*std::vector<Object*>::iterator item = App->world->obj_vector.begin();
		while (item != App->world->obj_vector.end()) {

			if ((*item)->is_mesh) {
				temp_vec.x += (*item)->obj_mesh.bounding_box.CenterPoint().x;
				temp_vec.y += (*item)->obj_mesh.bounding_box.CenterPoint().y;
				temp_vec.z += (*item)->obj_mesh.bounding_box.CenterPoint().z;
				
			}

			item++;
		}
		if (num > 0)
			temp_vec /= num;
			*/
		std::vector<Object*>::iterator item = App->world->obj_vector.begin();

		 vec3 temp_vec2 = vec3(0, 0, 0);
		 while (item != App->world->obj_vector.end()) {
			 if ((*item)->is_mesh) {
				 temp_vec2.x += (*item)->obj_mesh.bounding_box.maxPoint.x;
				 temp_vec2.y += (*item)->obj_mesh.bounding_box.maxPoint.y;
				 temp_vec2.z += (*item)->obj_mesh.bounding_box.maxPoint.z;

				 if (max_dist.x < temp_vec2.x)max_dist.x = temp_vec2.x;
				 if (max_dist.y < temp_vec2.y)max_dist.y = temp_vec2.y;
				 if (max_dist.z < temp_vec2.z)max_dist.z = temp_vec2.z;


			 }
			 item++;
		 }
		 Reference = temp_vec;
		Position = Reference + Z * length(max_dist - temp_vec);
		

	}

	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT)== KEY_IDLE)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		
		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}
		Position = Reference + Z * length(Position);

		if (App->world->obj_vector.size() > 0) {

			std::vector<Object*>::iterator item = App->world->obj_vector.begin();
			temp_vec = vec3(0, 0, 0);
			num = 0;
			int i = 0;
			while (item != App->world->obj_vector.end()) {

				if ((*item)->is_mesh) {
					temp_vec.x += (*item)->obj_mesh.bounding_box.CenterPoint().x;
					temp_vec.y += (*item)->obj_mesh.bounding_box.CenterPoint().y;
					temp_vec.z += (*item)->obj_mesh.bounding_box.CenterPoint().z;
					num++;
				}
				
				item++;
			}
		
		}



	
		
	}

	

	else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT  && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		
		float Sensitivity = 0.25f;

		

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

		
			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);
			

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}

		}
	

	}
	
	float wheel = 0;

	
	static float angle = 90;

	wheel = App->input->GetMouseZ();

	if (wheel > 0) angle -= 1;
	else if(wheel < 0)angle += 1;
	uint w = App->window->width;
	uint h = App->window->height;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

								 //Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(angle,                  //The camera angle
		(double)w / (double)h, //The width-to-height ratio
		.1,                   //The near z clipping coordinate
		600.0);                //The far z clipping coordinate

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;
		
	// Recalculate matrix -------------
	CalculateViewMatrix();
	
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}