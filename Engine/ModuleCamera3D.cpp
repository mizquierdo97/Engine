#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ImGui\ImGuizmo.h"
#include "MathGeoLib.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{


	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 1.0f, 0.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{

	RELEASE(dummyfrustum);
}

bool ModuleCamera3D::Init() {

	dummyfrustum = new ComponentCamera();
	CalculateViewMatrix();
	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{


	picking = LineSegment(float3::zero, float3::unitY);



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

	if (!App->world->ImFocused())
		return UPDATE_CONTINUE;

	static float3 temp_vec = float3(0, 0, 0);
	static float num = 0;
	float wheel = 0;
	float3 newPos(0, 0, 0);
	float speed = 10.0f * dt;

	//Speed up 
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 20.0f * dt;

	//WASD Movement
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z*speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z*speed;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	//Wheel mouse
	wheel = App->input->GetMousepositionZ();
	if (wheel > 0) newPos -= Z * speed;
	else if (wheel < 0)newPos += Z * speed;


	Reference = temp_vec;

	Position += newPos;
	Reference += newPos;

	float3 max_dist = float3(0, 0, 0);
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && App->world->IsObjectSelected()) {

		GameObject* selected = App->world->GetSelectedObject();

		float4x4 mat = selected->GetTransform()->GetMatrix();
		float3 pos, sca;
		Quat rot;
		mat.Decompose(pos, rot, sca);

		AABB selected_AABB = selected->GetGlobalBBox();
		float3 new_position = float3(0, 0, 0);
		num = 0;
		int i = 0;
		
		temp_vec = pos;

		float3 temp_vec2 = float3(0, 0, 0);
		
			ComponentMesh* temp_mesh = selected->GetMesh();
			if (temp_mesh != nullptr) {
				temp_vec2.x = (selected_AABB.maxPoint.x);
				temp_vec2.y = (selected_AABB.maxPoint.y);
				temp_vec2.z = (selected_AABB.maxPoint.z);

				if (Abs(max_dist.x) < Abs(temp_vec2.x))max_dist.x = temp_vec2.x;
				if (Abs(max_dist.y) < Abs(temp_vec2.y))max_dist.y = temp_vec2.y;
				if (Abs(max_dist.z) < Abs(temp_vec2.z))max_dist.z = temp_vec2.z;
			}
			else {
				max_dist = float3(3, 3, 3);
			}
		
		Reference = temp_vec;
		Position = Reference + Z * (max_dist*1.5f - temp_vec).Length();

	}

	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->world->IsObjectSelected())
	{
		int dx = -App->input->GetMousepositionXMotion();
		int dy = -App->input->GetMousepositionYMotion();

		GameObject* selected = App->world->GetSelectedObject();
		float4x4 mat = selected->GetTransform()->GetMatrix();
		float3 pos, sca;
		Quat rot;
		mat.Decompose(pos, rot, sca);

		float Sensitivity = 0.25f;

		Reference = pos;
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
				Y = Z.Cross(X);
			}
		}
		Position = Reference + Z * (Position).Length();

		GameObject* obj = App->world->GetSelectedObject();
		temp_vec = float3(0, 0, 0);


		temp_vec.x += obj->GetTransform()->translation.x;
		temp_vec.y += obj->GetTransform()->translation.y;
		temp_vec.z += obj->GetTransform()->translation.z;



	}

	else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && !App->world->IsObjectSelected())
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
				Y = Z.Cross(X);
			}

		}

	}

	
	bool test = ImGuizmo::IsOver();
	if (!test) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->world->ImFocused())
		{

			float mx = (float)App->input->GetMousepositionX();
			float my = (float)App->input->GetMousepositionY();
			float width = (float)App->window->GetWidth();
			float height = (float)App->window->GetHeight();

			if (mx > App->world->world_tex_vec.x && mx < (App->world->world_tex_vec.x + App->world->world_tex_vec.z)) {
				if (my > App->world->world_tex_vec.y+40 && my < (App->world->world_tex_vec.y + App->world->world_tex_vec.w)) {
					mouse_pos.x = -(1.0f - ((mx - App->world->world_tex_vec.x) / (App->world->world_tex_vec.z / 2.0f)));
					mouse_pos.y = (1.0f - ((my - App->world->world_tex_vec.y-40) / (App->world->world_tex_vec.w / 2.0f)));

					picking = dummyfrustum->cam_frustum.UnProjectLineSegment(mouse_pos.x, mouse_pos.y);

					float distance;
					App->world->SetSelectedObject(Raycast(picking, distance));
				}

			}
		}
	}


	float color[4] = { 1.0f, 1.0f, 0.7f, 1 };
	dummyfrustum->cam_frustum.Draw(5.0f, color);
		

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
	return dummyfrustum->cam_frustum.ViewProjMatrix().ptr();
	//return (float*)ViewMatrix.v;
}

void ModuleCamera3D::UpdateCamProp()
{
		dummyfrustum->cam_frustum.horizontalFov = (2*  math::Atan(math::Tan(dummyfrustum->cam_frustum.verticalFov / 2) *App->window->GetTextureAspectRatio()));

}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	dummyfrustum->cam_frustum.pos = Position;
	dummyfrustum->cam_frustum.front = -Z;
	dummyfrustum->cam_frustum.up = Y;

	//	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);
	//ViewMatrixInverse = ViewMatrix.Inverted();
}

GameObject * ModuleCamera3D::Raycast(const LineSegment & segment, float  &dist)const
{
	dist = inf;
	GameObject* Closest_object = nullptr;
	Recursivetest(segment, dist, &Closest_object);
	return Closest_object;
}



void ModuleCamera3D::Recursivetest(const LineSegment& segment, float& dist, GameObject** closest_object)const
{
	std::map<float, GameObject*> obj;
	App->world->quadtree.root->CollectIntersections(obj, segment);
	std::vector<ComponentMesh*> meshes;
	GameObject* goTemp;
	for (std::map<float, GameObject*>::const_iterator iterator = obj.begin(); iterator != obj.end(); iterator++)
	{
		goTemp = iterator->second;
		meshes.push_back((goTemp)->GetMesh());

	}

	for (std::list<GameObject*>::const_iterator item = App->world->non_static_list.begin(); item != App->world->non_static_list.end(); item++) {
		if ((*item)->GetGlobalBBox().Intersects(segment))
			meshes.push_back((*item)->GetMesh());
	}

	for (int n = 0; n<meshes.size(); n++) {
		ComponentMesh* oMesh = meshes[n];
		GameObject* go = oMesh->GetParent();
		ComponentTransform* transform = (ComponentTransform*)go->GetTransform();
		Mesh objmesh = ((ResourceMesh*)oMesh->GetResource())->obj_mesh;

		LineSegment local(segment);
		local.Transform(transform->GetMatrix().Inverted());


		for (int i = 0; i < objmesh.num_indices - 9;)
		{
			Triangle tri;
			tri.a.Set(&objmesh.vertexs[objmesh.indices[i++] * 3]);
			tri.b.Set(&objmesh.vertexs[objmesh.indices[i++] * 3]);
			tri.c.Set(&objmesh.vertexs[objmesh.indices[i++] * 3]);
			float localdistance = 0;
			float3 localhitpoint;

			if (local.Intersects(tri, &localdistance, &localhitpoint))
			{
				if (localdistance < dist) {

					dist = localdistance;
					closest_object[0] = go;

				}
			}
		}
	}
}