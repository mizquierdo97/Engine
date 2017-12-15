
#ifndef _COMPONENT_MATERIAL
#define _COMPONENT_MATERIAL


#define MAX_TEXTURES 5

#include "Component.h"
#include "ShaderProgram.h"
#include "ResourceTexture.h"
class ComponentMaterial :public Component {
	public:
		ComponentMaterial() {
			shader = &App->shaders->default_shader;
			diffuse_tex = IID_NULL;
			normal_tex = IID_NULL;

			for (int i = 0; i < MAX_TEXTURES; i++) {
				extra_textures[i] = IID_NULL;
			}
		};
		ComponentMaterial(UUID uid, GameObject* obj) {
			res_uuid = uid;			
			SetParent(obj);
			comp_type = ComponentType::material;
			alpha_test = 0.5f;
			diffuse_tex = IID_NULL;
			normal_tex = IID_NULL;
			for (int i = 0; i < MAX_TEXTURES; i++) {
				extra_textures[i] = IID_NULL;
			}

		};
		~ComponentMaterial() {
			
		};

		void UpdateComponent();
		void SaveComponentScene(Data* data);
		void ShowInspectorComponents();

		float GetAlphaTest()const {
			return alpha_test;
		}
		void SetAlphaTest(float val) {
			alpha_test = val;
		}


public:
	std::string path_texture;
	ShaderProgram* shader = &App->shaders->default_shader;
	UUID diffuse_tex = IID_NULL;
	UUID normal_tex = IID_NULL;

	UUID extra_textures[MAX_TEXTURES];
	

private:
	float alpha_test = 0.5f;
};
#endif