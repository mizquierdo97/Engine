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
		};
		~ComponentMaterial() {
			
		};

		void UpdateComponent();
		void SaveComponentScene(Data* data);
		void ShowInspectorComponents();

public:
	std::string path_texture;
};