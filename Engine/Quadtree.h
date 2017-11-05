#pragma once
#include "MathGeoLib.h"
#include "Object.h"
#include <list>
class QuadtreeNode {
public:
	QuadtreeNode(const AABB& box);
	virtual ~QuadtreeNode();

	bool IsLeaf() const;
	void Insert(GameObject* go);
	void Erase(GameObject* go);
	void CreateChilds();
	void RedistributeChilds();
	void CollectBoxes(std::vector<const QuadtreeNode*>& nodes) const;
	void CollectObjects(std::vector<GameObject*>& objects) const;
	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const;


public:
	

	QuadtreeNode* parent = nullptr;
	std::list<GameObject*> objects;
	AABB bounds;
	QuadtreeNode* nodes[8];

	
};

class Quadtree
{
public:
	
	Quadtree();
	virtual ~Quadtree();
	void SetBoundaries(const AABB& box);
	void Insert(GameObject* go);
	void Erase(GameObject* go);
	void Clear();
	void CollectBoxes(std::vector<const QuadtreeNode*>& nodes) const;
	void CollectObjects(std::vector<GameObject*>& objects) const;
	

public:
	QuadtreeNode* root = nullptr;
	
};

	template<typename TYPE>
	void QuadtreeNode::CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const
	{
		//if (root != nullptr)
		//{
			if (primitive.Intersects(bounds))
			{
				for (std::list<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
				{
					if (primitive.Intersects((*it)->GetGlobalBBox()))
						objects.push_back(*it);
				}

				for (int i = 0; i < 4; ++i)
					if (nodes[i] != nullptr) nodes[i]->CollectIntersections(objects, primitive);
			}
	//}
}
