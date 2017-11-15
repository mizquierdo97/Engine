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

public:
	std::string path_texture;
	float alpha_test = 0.5f;
};