#pragma once
#include "Component.h"

class ComponentMaterial :public Component {
	public:
		ComponentMaterial() {
		
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
	
private:
	float alpha_test = 0.5f;
};