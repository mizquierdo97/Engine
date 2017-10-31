#pragma once
#include "Component.h"

class ComponentMaterial :public Component {
	public:
		ComponentMaterial() {
			obj_tex = new Texture();
		};
		ComponentMaterial(Texture* tex, GameObject* obj) {
			obj_tex = new Texture();
			obj_tex = tex;
			SetParent(obj);
			comp_type = ComponentType::material;
		};
		~ComponentMaterial() {
			RELEASE(obj_tex);
		};

		void UpdateComponent();
		void SaveComponentScene(Data* data) {};
		void ShowInspectorComponents();

public:
	Texture* obj_tex;
};