#pragma once
#include "MathGeoLib.h"
#include "Object.h"
#include <list>
class QuadtreeNode {
public:
	QuadtreeNode(const AABB& box);
	//virtual ~QuadtreeNode();

	bool IsLeaf() const;
	void Insert(GameObject* go);
	void Erase(GameObject* go);
	void CreateChilds();
	void RedistributeChilds();
	void CollectBoxes(std::vector<const QuadtreeNode*>& nodes) const;
	void CollectObjects(std::vector<GameObject*>& objects) const;
	/*//void CollectObjects(std::map<float, GameObject*>& objects, const float3& origin) const;
	//template<typename TYPE>
//	void CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& primitive) const;
	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const;
	*/


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
	/*//void CollectObjects(std::map<float, GameObject*>& objects, const float3& origin) const;
	//template<typename TYPE>
	//void CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& primitive) const;
	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const;*/

public:
	QuadtreeNode* root = nullptr;
	
};