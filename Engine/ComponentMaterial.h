#pragma once
#include "Component.h"

class ComponentMaterial :public Component {
	public:
		ComponentMaterial() {
		};
		ComponentMaterial(Texture* tex, Object* obj) {
			obj_tex = tex;
			parent = obj;
			comp_type = ComponentType::material;
		};
		~ComponentMaterial() {

		};

		void UpdateComponent();

public:
	Texture* obj_tex;
};