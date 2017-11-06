#include "Quadtree.h"
#define QUADTREE_MAX_ITEMS 4
#define QUADTREE_MIN_SIZE 1.0f 

#define  U_NE 0
#define  U_SE 1
#define  U_SW 2
#define  U_NW 3
#define  D_NE 4
#define  D_SE 5
#define  D_SW 6
#define  D_NW 7

QuadtreeNode::QuadtreeNode(const AABB& box) : bounds(box)
{
	parent = nodes[U_NE] = nodes[U_SE] = nodes[U_SW] = nodes[U_NW] = nodes[D_NE] = nodes[D_SE] = nodes[D_SW] = nodes[D_NW] = nullptr;
}
QuadtreeNode::~QuadtreeNode()
{
		for (int i = 0; i < 8; ++i)
			if (nodes[i] != nullptr) RELEASE(nodes[i]);

}
bool QuadtreeNode::IsLeaf() const
{
	return nodes[0] == nullptr;
}

void QuadtreeNode::Insert(GameObject* go)
{
	if (IsLeaf() == true &&	(objects.size() < QUADTREE_MAX_ITEMS ||(bounds.HalfSize().LengthSq() <= QUADTREE_MIN_SIZE * QUADTREE_MIN_SIZE)))
		objects.push_back(go);
	else
	{
		if (IsLeaf() == true)
			CreateChilds();

		objects.push_back(go);
		RedistributeChilds();
	}
}

void QuadtreeNode::Erase(GameObject * go)
{
	std::list<GameObject*>::iterator it = std::find(objects.begin(), objects.end(), go);
	if (it != objects.end())
		objects.erase(it);

	if (IsLeaf() == false)
	{
		for (int i = 0; i < 8; ++i)
			nodes[i]->Erase(go);
	}
}

void QuadtreeNode::CreateChilds()
{
	// We need to subdivide this node ...
	float3 size(bounds.Size());
	float3 new_size(size.x*0.5f, size.y*0.5, size.z*0.5f); // Octree would subdivide y too

	float3 center(bounds.CenterPoint());
	float3 new_center(center);
	AABB new_box;

	// NorthEast
	for (int i = 0; i < 2; i++) {
		int mult = 1;
		if (i == 1) mult = -1;
		new_center.x = center.x + size.x * 0.25f;
		new_center.y = center.y + mult*size.y * 0.25f;
		new_center.z = center.z + size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		nodes[i*4 + U_NE] = new QuadtreeNode(new_box);

		// SouthEast
		new_center.x = center.x + size.x * 0.25f;
		new_center.y = center.y + mult*size.y * 0.25f;
		new_center.z = center.z - size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		nodes[i * 4 + U_SE] = new QuadtreeNode(new_box);

		// SouthWest
		new_center.x = center.x - size.x * 0.25f;
		new_center.y = center.y + mult*size.y * 0.25f;
		new_center.z = center.z - size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		nodes[i * 4 + U_SW] = new QuadtreeNode(new_box);

		// NorthWest
		new_center.x = center.x - size.x * 0.25f;
		new_center.y = center.y + mult*size.y * 0.25f;
		new_center.z = center.z + size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		nodes[i * 4 + U_NW] = new QuadtreeNode(new_box);
	}
}

void QuadtreeNode::RedistributeChilds()
{
	// Now redistribute ALL objects
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end();)
	{
		GameObject* go = *it;

		AABB new_box(go->GetGlobalBBox());

		// Now distribute this new gameobject onto the childs
		bool intersects[8];
		for (int i = 0; i < 8; ++i)
			intersects[i] = nodes[i]->bounds.Intersects(new_box);

		if (intersects[0] && intersects[1] && intersects[2] && intersects[3]&& intersects[4] && intersects[5] && intersects[6] && intersects[7])
			++it; // if it hits all childs, better to just keep it here
		else
		{
			it = objects.erase(it);
			for (int i = 0; i < 8; ++i)
				if (intersects[i]) nodes[i]->Insert(go);
		}
	}
}

void QuadtreeNode::CollectObjects(std::vector<GameObject*>& objects) const
{
	for (std::list<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		objects.push_back(*it);

	for (int i = 0; i < 8; ++i)
		if (nodes[i] != nullptr) nodes[i]->CollectObjects(objects);
}

void QuadtreeNode::CollectObjects(std::map<float, GameObject*>& objects, const float3 & origin) const
{
	for (std::list<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
	{
		float dist = origin.DistanceSq((*it)->GetTransform()->translation);
		objects[dist] = *it;
	}

	for (int i = 0; i < 8; ++i)
		if (nodes[i] != nullptr) nodes[i]->CollectObjects(objects, origin);

}


void QuadtreeNode::CollectBoxes(std::vector<const QuadtreeNode*>& vec_nodes) const
{
	vec_nodes.push_back(this);

	for (int i = 0; i < 8; ++i)
		if (nodes[i] != nullptr) nodes[i]->CollectBoxes(vec_nodes);
}

//---------------------------------


Quadtree::Quadtree()
{}

Quadtree::~Quadtree()
{
	Clear();
}

void Quadtree::SetBoundaries(const AABB& box)
{
	Clear();
	root = new QuadtreeNode(box);
}

void Quadtree::Insert(GameObject* go)
{
	if (root != nullptr)
	{
		
		if (go->GetGlobalBBox().Intersects(root->bounds))
			root->Insert(go);
	}
}

void Quadtree::Erase(GameObject * go)
{
	if (root != nullptr)
		root->Erase(go);
}

void Quadtree::Clear()
{
	RELEASE(root);
}

void Quadtree::CollectBoxes(std::vector<const QuadtreeNode*>& nodes) const
{
	if (root != nullptr)
		root->CollectBoxes(nodes);
}

void Quadtree::CollectObjects(std::vector<GameObject*>& objects) const
{
	if (root != nullptr)
		root->CollectObjects(objects);
}

void Quadtree::CollectObjects(std::map<float, GameObject*>& objects, const float3& origin) const
{
	if (root != nullptr)
		root->CollectObjects(objects, origin);
}
