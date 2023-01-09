#include <stdio.h>
#include <string>	//std::string
#include <fstream>	//ifstream file
#include <iostream>	//cout
#include <string>	//getline()
#include <vector>	//vector
#include <stack>	//stack

#include "bvhLoader.h"


void BVHLoader(std::string path)
{
	std::ifstream file;
	skeleton skel;
	std::string line;
	std::stack<std::string> parent;

	file.open(path);
	if (!file.good())
		std::cout << "ERROR::failed to open!!!::" << path << std::endl;

	while (std::getline(file, line))
	{
		std::string text;
		file >> text;
		if (text == "HIERARCHY")
		{
			while(std::getline(file, line))
			{
				file >> text;
				if (text == "ROOT")
				{
					Node new_node;
					new_node.parent = "me";

					file >> text;	//root
					std::cout << text << std::endl;	//root
					new_node.name = text;
					parent.push(text);					//root

					while (std::getline(file, line))
					{
						file >> text;
						float3 offset;
						if (text == "OFFSET")
						{
							file >> new_node.offset.x;
							file >> new_node.offset.y;
							file >> new_node.offset.z;
						}
						else if (text == "CHANNELS")
						{
						}

						skel.n.push_back(new_node);
						break;
					}
				}

				if (text == "JOINT")
				{
					Node new_node;
					new_node.parent = parent.top();

					file >> text;	//hip, spine, head, ...
					std::cout << text << std::endl;	//hip, spine, head, ...
					new_node.name = text;	//hip, spine, head, ...
					parent.push(text);		//hip, spine, head, ...

					while (std::getline(file, line))
					{
						file >> text;
						float3 offset;
						if (text == "OFFSET")
						{
							file >> new_node.offset.x;
							file >> new_node.offset.y;
							file >> new_node.offset.z;

							skel.n.push_back(new_node);

						}
						else if (text == "CHANNELS")
						{
						}
						
						break;
					}
				}

				if (text == "End")
				{
					parent.push(text);
					std::getline(file, line);	//{
					std::getline(file, line);	//OFFSET

				}
				
				if (text == "}")
				{
					parent.pop();
				}

			}
		}
		
		if (text == "MOTION")
		{
		}

	}

	//파일 load 결과 확인
	for (int i = 0; i < skel.n.size(); i++)
	{
		std::cout << "node name: " << skel.n[i].name << std::endl;
		std::cout << "offset: " << skel.n[i].offset.x << ", " << skel.n[i].offset.y << ", " << skel.n[i].offset.z << std::endl;
		std::cout << "parent name: " << skel.n[i].parent << std::endl;
		std::cout <<  std::endl;
	}

}