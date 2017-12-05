
#ifndef _COMPONENT_MATERIAL
#define _COMPONENT_MATERIAL

#include "Component.h"
#include "ShaderProgram.h"
class ComponentMaterial :public Component {
	public:
		ComponentMaterial() {
			shader = App->shaders->default_shader;
		};
		ComponentMaterial(UUID uid, GameObject* obj) {
			res_uuid = uid;			
			SetParent(obj);
			comp_type = ComponentType::material;
			alpha_test = 0.5f;
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
	ShaderProgram shader = App->shaders->default_shader;
private:
	float alpha_test = 0.5f;
};
#endif