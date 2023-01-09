#pragma once
#include <string>	//std::string

/*
 

struct float3 {
	float x;
	float y;
	float z;
};

struct Node
{
	std::string name;
	Vector3 offset;
	std::string parent;

};

struct skeleton
{
	std::vector<Node> n;

};


class SearchTree
{
public:
	SearchTree() :root(nullptr) {};
	~SearchTree() {};

	void addNode(std::string name, float3 offset);
	bool searchValue(float3 offset);
	void removeNode(float3 offset);
	void show();
private:
	Node* root;

	void inOrder(Node* current)
	{
		if (current != nullptr)
		{
			//inOrder(current->first);
			//std::cout << current->offset.x << current->offset.y << current->offset.z << std::endl;
			//inOrder(current->second);
			//std::cout << current->offset.x << current->offset.y << current->offset.z << std::endl;
			//inOrder(current->third);
		}
	}

	//Node* searchMaxNode(Node* node)
	//{
	//	if (node == NULL)
	//		return NULL;
	//	while (node->third != NULL)
	//	{
	//		node = node->third;
	//	}
	//	return node;
	//}

	Node* removeSequence(Node* node, float _value);
};

void SearchTree::addNode(std::string name, float3 offset)
{
	Node* node = new Node();
	Node* tmpRoot = nullptr;

	node->name = name;
	node->offset = offset;

	if (root == nullptr)
		root = node;
	else
	{
		Node* ptr = root;
		while (ptr != nullptr)
		{
			tmpRoot = ptr;	//root
			//if (ptr->first == nullptr)
			//{
			//
			//}
			//else if (ptr->second == nullptr)
			//{
			//
			//}
			//else if (ptr->third == nullptr)
			//{
			//
			//}
		}
	}
}

void BVHLoader(std::string path);

*/