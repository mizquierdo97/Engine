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
			parent = obj;
			comp_type = ComponentType::material;
		};
		~ComponentMaterial() {
			delete obj_tex;
		};

		void UpdateComponent();

public:
	Texture* obj_tex;
};