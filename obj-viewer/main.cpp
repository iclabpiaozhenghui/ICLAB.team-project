#include <stdio.h>
#include <fstream>	//ifstream file
#include <iostream>	//cout
#include <string>	//getline()
#include <vector>	//vector
//#include <typeinfo>	
//#include <istream>
//#include <ostream>

#include "objLoader.h"

using namespace std;

vector<data_seq> sequence;
vector<int> texture_num;


vector<texture_image> tex_list;

int main()
{	
	int data_min = 3900;
	int data_max = 3901;

	int data_num = data_max - data_min + 1;
	

	for (int i = 0; i < data_num; i++)
	{
		string obj_path = "./Low_Proven1_stt/" + std::to_string(data_min + i) + "/textured_model.obj";							//°æ·Î1/3
		
		OBJLoader(obj_path, i, sequence, texture_num);
		
		printf("seq-number : %d\n", i);
		printf("v - %d\n", sequence[i].v.size());
		printf("t - %d\n", sequence[i].t.size());



		string tex_path = "./Low_Proven1_stt/" + std::to_string(data_min + i) + "/textured_model_material0000_map_Kd.png";				//°æ·Î2/3
		string tex_path2 = "./Low_Proven1_stt/" + std::to_string(data_min + i) + "/textured_model_material0001_map_Kd.png";				//°æ·Î3/3

		cv::Mat texture_01 = cv::imread(tex_path, cv::IMREAD_UNCHANGED);
		cv::Mat texture_02;

		if (texture_num[i] == 2)
		{
			texture_02 = cv::imread(tex_path2, cv::IMREAD_UNCHANGED);
		}
		
		tex_list.push_back({ texture_01.clone() , texture_02.clone() });
	

		//for (int i = 0; i < tex_list.size(); i++) {
		//	cv::Mat temp1 = tex_list[i].tex1.clone();
		//	cv::Mat temp2 = tex_list[i].tex2.clone();
		//	
		//	cv::imshow("window", temp1);
		//	cv::imshow("window2", temp2);
		//
		//	cv::waitKey(0);
		//}

	}


	OBJViewer(data_num, sequence, tex_list, texture_num);


}
