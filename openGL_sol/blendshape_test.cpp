#include <string>
#include <direct.h>		//_mkdir
#include <math.h>
#include <fstream>

#define TINYPLY_IMPLEMENTATION		//??

#pragma warning(disable: 4996)
#include "PLY_tinyply.h"
#include "objLoader.h"
//#include "bvhLoader.h"



static double time1 = 0;
static double time4 = 0;

static int pose = 0;

//project window size
static int win_w = 1000;
static int win_h = 1000;

GLfloat r_arm_u = 0;	//오른쪽 어깨
GLfloat r_arm_d = 0;
GLfloat l_arm_u = 0;
GLfloat l_arm_d = 0;


vector<data_seq> sequence;
vector<int> texture_num;

vector<texture_image> tex_list;
std::vector<Json::Value> joint_list;

std::vector<Vector3> point_vec2;

body body_part;
body body_part_draw;

uchar3 rgb_red = { 255.0f, 0.0f, 0.0f };
uchar3 rgb_pink = { 240.0f, 20.0f, 88.0f };
uchar3 rgb_org = { 237.0f, 92.0f, 21.0f };	//오렌지

#define RUN 1

void BVHLoader(std::string path, skeleton skel);

void FindRealValue_aabb_3D(std::vector<Json::Value> joint_list, int frame, vector<Vector3> point_3D, cv::Mat aabb_image_x, cv::Mat aabb_image_z, vector<Vector3>& joint_3D);
//void skeleton_postprocessing(Vector3& target_node, Vector3 parents_node, Vector3 child_node, vector<Vector3>  point_vec, vector<Vector3>& nei, vector<uchar3>& nei_rgb, double Epsilon, int frame_num);
void translate_v_to_draw(std::vector<Vector3> skeleton);
void divide_upper_lower_body(vector<Vector3> v, std::vector<Vector3> skeleton);
void divide_upper_body(std::vector<Vector3> skeleton);
void divide_lower_body(std::vector<Vector3> skeleton);
void project_viewer_glut(int argc, char** argv);


int main(int argc, char** argv)		//blendshape_test
{
	//shape 불러오기
	//OBJLoader_noTex("E:/01_data/12_obj/blendshape/Apple blendshapes51 OBJs/OBJs", );

	return 0;









	//bvh loader생성
	skeleton skel;
	BVHLoader("./bvh/loot_Adult_Male.bvh", skel);

	int data_idx = 3706;
	int data_num = 1;
	std::string obj_path = "./input/" + std::to_string(data_idx) + "/textured_model.obj";								//경로1/4
	string tex_path1 = "./input/" + std::to_string(data_idx) + "/textured_model_material0000_map_Kd.png";				//경로2/4
	string tex_path2 = "./input/" + std::to_string(data_idx) + "/textured_model_material0001_map_Kd.png";				//경로3/4
	string tex_path3 = "./input/" + std::to_string(data_idx) + "/textured_model_material0002_map_Kd.png";				//경로4/4

	OBJLoader(obj_path, sequence, texture_num);

	cv::Mat texture_01 = cv::imread(tex_path1, CV_LOAD_IMAGE_UNCHANGED);
	cv::Mat texture_02;
	cv::Mat texture_03;

	if (texture_num[0] == 3)
	{
		texture_02 = cv::imread(tex_path2, CV_LOAD_IMAGE_UNCHANGED);
		texture_03 = cv::imread(tex_path3, CV_LOAD_IMAGE_UNCHANGED);

	}
	else if (texture_num[0] == 2)
	{
		texture_02 = cv::imread(tex_path2, CV_LOAD_IMAGE_UNCHANGED);

	}
	tex_list.push_back({ texture_01.clone(), texture_02.clone(), texture_03.clone() });


	char mkpath[256];
	char cha[256];

	sprintf_s(cha, "project_sj_t");


	sprintf_s(mkpath, "C:/02_data/05_skeleton/%s/input", cha);
	_mkdir(mkpath);
	sprintf_s(mkpath, "C:/02_data/05_skeleton/%s/2Doutput", cha);
	_mkdir(mkpath);
	sprintf_s(mkpath, "C:/02_data/05_skeleton/%s/3Doutput", cha);
	_mkdir(mkpath);

	OBJViewer(data_num, sequence, tex_list, texture_num, cha);

	char bat_path[256];
	char bat[32] = ".bat";

	strcat(bat_path, cha);
	strcat(bat_path, bat);
	system(cha);	//오픈포즈 배치파일 실행

	char json_path[256];

	joint_list.clear();
	for (int j = 0; j < data_num; j++)
	{
		Json::Value joint_root;

		for (int win_num = 0; win_num < 4; win_num++)
		{
			string str;
			Json::Value root;
			Json::Reader reader;

			sprintf_s(json_path, "C:/02_data/05_skeleton/%s/2Doutput/%d_img%d_keypoints.json", cha, j, win_num);

			ifstream ist(json_path);

			for (char p = 0; ist >> p;)
			{
				str += p;
			}
			bool parsingRet0 = reader.parse(str, root);
			joint_root[win_num] = root["people"][0]["pose_keypoints_2d"];
		}
		joint_list.push_back(joint_root);
	}


	int frame;
	frame = 0;

	while (1)
	{
		std::vector<Vector3> point_vec;

		vector<Vector3> joint2, joint_nei;
		vector<Vector3> joint_3D;

		float confidence = 0.45;

		char imread_path[256];

		sprintf(imread_path, "C:/02_data/05_skeleton/%s/input/0_img%d.png", cha, 0);
		cv::Mat image_z = cv::imread(imread_path);
		sprintf(imread_path, "C:/02_data/05_skeleton/%s/input/0_img%d.png", cha, 1);
		cv::Mat image_x = cv::imread(imread_path);

		vector<vector<Vector3>> scaled_joint_list;

		//4 cam
		FindRealValue_aabb_3D(joint_list, frame, sequence[frame].v, image_x, image_z, joint_3D);

		point_vec = sequence[frame].v;
		point_vec2.clear();

		Vector3 output_joint = { 0,0,0 };
		vector<uchar3> rgb_vec;
		vector<uchar3> nei_rgb_vec;

		//joint2.clear();	rgb_vec.clear();			//nei뽑기1/2
		//skeleton_postprocessing(joint_3D[4], joint_3D[3], joint_3D[4], point_vec, joint2, rgb_vec, 0.03, frame);	//연구실 데이터 0.03
		//joint_nei = joint2;	nei_rgb_vec = rgb_vec;	//nei뽑기2/2

		point_vec2.insert(point_vec2.begin(), joint_3D.begin(), joint_3D.end());

		vector<int> edge1;
		vector<int> edge2;

		edge1.push_back(0);	edge1.push_back(1);	edge1.push_back(2);	edge1.push_back(3);
		edge1.push_back(1);	edge1.push_back(5);	edge1.push_back(6);	edge1.push_back(1);
		edge1.push_back(8);	edge1.push_back(9);	edge1.push_back(10);	edge1.push_back(8);
		edge1.push_back(12);	edge1.push_back(13);	//edge1.push_back(0);	edge1.push_back(0);

		edge2.push_back(1);	edge2.push_back(2);	edge2.push_back(3);	edge2.push_back(4);
		edge2.push_back(5);	edge2.push_back(6);	edge2.push_back(7);	edge2.push_back(8);
		edge2.push_back(9);	edge2.push_back(10);	edge2.push_back(11);	edge2.push_back(12);
		edge2.push_back(13);	edge2.push_back(14);//	edge2.push_back(17);	edge2.push_back(18);

		//sprintf_s(json_path, "C:/02_data/05_skeleton/%s/3Doutput/nei_%d", cha, frame);
		//write_ply_rgb(json_path, joint_nei, nei_rgb_vec);

		sprintf_s(json_path, "C:/02_data/05_skeleton/%s/3Doutput/%d", cha, frame);		//1110 무슨 의미?
		write_ply_edge(json_path, point_vec2, edge1, edge2);
		frame++;

		if (frame == data_num)
		{
			frame = 0;
			break;
		}
	}

	//상체하체 분리
	divide_upper_lower_body(sequence[0].v, point_vec2);
	//divide_upper_body(point_vec2);
	//divide_lower_body(point_vec2);

	printf("head points: %d\n", body_part.head_list.size());
	printf("body points: %d\n", body_part.upper_rest_rest_list.size());

	return 0;



	//draw용 vertex로 변경
	translate_v_to_draw(point_vec2);

	project_viewer(body_part_draw, skel);

	return 0;

}

void BVHLoader(std::string path, skeleton skel)
{
	std::ifstream file;
	std::string line;
	std::stack<std::string> parent;

	file.open(path);
	if (!file.good())
		std::cout << "ERROR::failed to open!!!::" << path << std::endl;

	while (std::getline(file, line))
	{
		std::string text;
		file >> text;
		std::cout << text << std::endl;

		if (text == "ROOT")
		{
			Node new_node;
			new_node.parent = "me";

			file >> text;						//root
			std::cout << text << std::endl;		//root
			new_node.name = text;				//root
			parent.push(text);					//root

			std::getline(file, line);	//{

			std::getline(file, line);	//offset
			file >> text;
			std::cout << text << std::endl;//offset

			Vector3 offset;
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

		}

		if (text == "JOINT")
		{
			Node new_node;
			new_node.parent = parent.top();

			file >> text;	//hip, spine, head, ...
			std::cout << text << std::endl;	//hip, spine, head, ...
			new_node.name = text;	//hip, spine, head, ...
			parent.push(text);		//hip, spine, head, ...

			std::getline(file, line);	//{

			std::getline(file, line);	//offset
			file >> text;
			std::cout << text << std::endl;//offset

			Vector3 offset;
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

		if (text == "MOTION")
		{
			std::getline(file, line);
			file >> text;
			if (text == "Frames:")
			{
				file >> skel.frames;
			}

			std::getline(file, line);
			file >> text;
			if (text == "Frame")
			{
				file >> text;
				if (text == "Time:")
				{
					file >> skel.time;
				}
			}

			while (std::getline(file, line))
			{
				std::vector<Motion> one_frame_motion;

				for (int i = 0; i < skel.n.size(); i++)	//88번
				{
					Motion new_motion;	//6channel임

					file >> new_motion.position.x;
					file >> new_motion.position.y;
					file >> new_motion.position.z;

					file >> new_motion.rotation.x;
					file >> new_motion.rotation.y;
					file >> new_motion.rotation.z;

					one_frame_motion.push_back(new_motion);
				}
				skel.m.push_back(one_frame_motion);
			}
		}

	}

	//파일 load 결과 확인
	std::cout << "skeleton joint size: " << skel.n.size() << std::endl;

	//for (int i = 0; i < skel.n.size(); i++)
	//{
	//	std::cout << "node name: " << skel.n[i].name << std::endl;
	//	std::cout << "offset: " << skel.n[i].offset.x << ", " << skel.n[i].offset.y << ", " << skel.n[i].offset.z << std::endl;
	//	std::cout << "parent name: " << skel.n[i].parent << std::endl;
	//	std::cout << std::endl;
	//}

	std::cout << "frames: " << skel.frames << std::endl;
	std::cout << "frame time: " << skel.time << std::endl;
	std::cout << "frame number: " << skel.m.size() << std::endl;
	std::cout << "joint number: " << skel.m[0].size() << std::endl;

	//for (int i = 0; i < skel.m.size(); i++)
	//{
	//	std::cout << "num: " << i << "position: " << skel.m[i].position.x << skel.m[i].position.y << skel.m[i].position.z << std::endl;
	//	std::cout << "num: " << i << "rotation: " << skel.m[i].rotation.x << skel.m[i].rotation.y << skel.m[i].rotation.z << std::endl;
	//	std::cout << std::endl;
	//}

}

void translate_v_to_draw(std::vector<Vector3> skeleton)
{
	//상체(그대로 복사)
	body_part_draw.upper_rest_rest_list.insert(body_part_draw.upper_rest_rest_list.begin(), body_part.upper_rest_rest_list.begin(), body_part.upper_rest_rest_list.end());

	//머리(그대로 복사)
	body_part_draw.head_list.insert(body_part_draw.head_list.begin(), body_part.head_list.begin(), body_part.head_list.end());

	//
	//   
	//오른쪽
	//어깨(는 윗팔이랑 좀 다르게 움직여야 할듯)
	//for (int i = 0; i < body_part.r_shoulder_list.size(); i++)
	//{
	//	body_part_draw.r_shoulder_list[i].x = body_part.r_shoulder_list[i].x - skeleton[].x;
	//	body_part_draw.r_shoulder_list[i].y = body_part.r_shoulder_list[i].y - skeleton[].y;
	//	body_part_draw.r_shoulder_list[i].z = body_part.r_shoulder_list[i].z - skeleton[].z;
	//}

	//윗팔
	for (int i = 0; i < body_part.r_upper_arm_list.size(); i++)
	{
		body_part_draw.r_upper_arm_list.push_back({ body_part.r_upper_arm_list[i].x - skeleton[2].x, body_part.r_upper_arm_list[i].y - skeleton[2].y, body_part.r_upper_arm_list[i].z - skeleton[2].z });
		//pushback으로 변경해야할듯
		//body_part_draw.r_upper_arm_list[i].x = body_part.r_upper_arm_list[i].x - skeleton[2].x;
		//body_part_draw.r_upper_arm_list[i].y = body_part.r_upper_arm_list[i].y - skeleton[2].y;
		//body_part_draw.r_upper_arm_list[i].z = body_part.r_upper_arm_list[i].z - skeleton[2].z;
	}

	//아랫팔
	for (int i = 0; i < body_part.r_lower_arm_list.size(); i++)
	{
		body_part_draw.r_lower_arm_list.push_back({ body_part.r_lower_arm_list[i].x - skeleton[3].x, body_part.r_lower_arm_list[i].y - skeleton[3].y, body_part.r_lower_arm_list[i].z - skeleton[3].z });

		//body_part_draw.r_lower_arm_list[i].x = body_part.r_lower_arm_list[i].x - skeleton[3].x;
		//body_part_draw.r_lower_arm_list[i].y = body_part.r_lower_arm_list[i].y - skeleton[3].y;
		//body_part_draw.r_lower_arm_list[i].z = body_part.r_lower_arm_list[i].z - skeleton[3].z;
	}

	//손목(변경해야함 지금은 아랫팔이랑 동일)
	for (int i = 0; i < body_part.r_hand_list.size(); i++)
	{
		body_part_draw.r_hand_list.push_back({ body_part.r_hand_list[i].x - skeleton[3].x, body_part.r_hand_list[i].y - skeleton[3].y, body_part.r_hand_list[i].z - skeleton[3].z });

		//body_part_draw.r_hand_list[i].x = body_part.r_hand_list[i].x - skeleton[3].x;
		//body_part_draw.r_hand_list[i].y = body_part.r_hand_list[i].y - skeleton[3].y;
		//body_part_draw.r_hand_list[i].z = body_part.r_hand_list[i].z - skeleton[3].z;
	}

	//왼쪽
	//어깨
	//for (int i = 0; i < body_part.l_shoulder_list.size(); i++)
	//{
	//	body_part_draw.l_shoulder_list[i].x = body_part.l_shoulder_list[i].x - skeleton[5].x;
	//	body_part_draw.l_shoulder_list[i].y = body_part.l_shoulder_list[i].y - skeleton[5].y;
	//	body_part_draw.l_shoulder_list[i].z = body_part.l_shoulder_list[i].z - skeleton[5].z;
	//}

	//윗팔
	for (int i = 0; i < body_part.l_upper_arm_list.size(); i++)
	{
		body_part_draw.l_upper_arm_list.push_back({ body_part.l_upper_arm_list[i].x - skeleton[5].x, body_part.l_upper_arm_list[i].y - skeleton[5].y, body_part.l_upper_arm_list[i].z - skeleton[5].z });

		//body_part_draw.l_upper_arm_list[i].x = body_part.l_upper_arm_list[i].x - skeleton[5].x;
		//body_part_draw.l_upper_arm_list[i].y = body_part.l_upper_arm_list[i].y - skeleton[5].y;
		//body_part_draw.l_upper_arm_list[i].z = body_part.l_upper_arm_list[i].z - skeleton[5].z;
	}

	//아랫팔
	for (int i = 0; i < body_part.l_lower_arm_list.size(); i++)
	{
		body_part_draw.l_lower_arm_list.push_back({ body_part.l_lower_arm_list[i].x - skeleton[6].x, body_part.l_lower_arm_list[i].y - skeleton[6].y, body_part.l_lower_arm_list[i].z - skeleton[6].z });

		//body_part_draw.l_lower_arm_list[i].x = body_part.l_lower_arm_list[i].x - skeleton[6].x;
		//body_part_draw.l_lower_arm_list[i].y = body_part.l_lower_arm_list[i].y - skeleton[6].y;
		//body_part_draw.l_lower_arm_list[i].z = body_part.l_lower_arm_list[i].z - skeleton[6].z;
	}

	//손목(추가)
	for (int i = 0; i < body_part.l_hand_list.size(); i++)
	{
		body_part_draw.l_hand_list.push_back({ body_part.l_hand_list[i].x - skeleton[6].x, body_part.l_hand_list[i].y - skeleton[6].y, body_part.l_hand_list[i].z - skeleton[6].z });

		//body_part_draw.l_hand_list[i].x = body_part.l_hand_list[i].x - skeleton[6].x;
		//body_part_draw.l_hand_list[i].y = body_part.l_hand_list[i].y - skeleton[6].y;
		//body_part_draw.l_hand_list[i].z = body_part.l_hand_list[i].z - skeleton[6].z;
	}

	//하반신
	for (int i = 0; i < body_part.lower_list.size(); i++)
	{
		body_part_draw.lower_list.push_back({ body_part.lower_list[i].x - skeleton[8].x, body_part.lower_list[i].y - skeleton[8].y, body_part.lower_list[i].z - skeleton[8].z });

		//body_part_draw.l_hand_list[i].x = body_part.l_hand_list[i].x - skeleton[6].x;
		//body_part_draw.l_hand_list[i].y = body_part.l_hand_list[i].y - skeleton[6].y;
		//body_part_draw.l_hand_list[i].z = body_part.l_hand_list[i].z - skeleton[6].z;
	}
}


void divide_upper_lower_body(vector<Vector3> v, std::vector<Vector3> skeleton)
{
	Vector3 spine;
	//척추 벡터 구하기
	spine.x = skeleton[1].x - skeleton[8].x;
	spine.y = skeleton[1].y - skeleton[8].y;
	spine.z = skeleton[1].z - skeleton[8].z;

	std::vector<uchar3> rgb_list;

	//내적값 구하기
	for (int i = 0; i < v.size(); i++)
	{
		if (((v[i].x - skeleton[8].x) * spine.x + (v[i].y - skeleton[8].y) * spine.y + (v[i].z - skeleton[8].z) * spine.z) > 0)
		{
			rgb_list.push_back({ 255, 0, 0 });
			body_part.upper_list.push_back(v[i]);
			//divide_upper_body

		}
		else
		{
			rgb_list.push_back({ 0, 0, 255 });
			body_part.lower_list.push_back(v[i]);

		}
	}

	string divide_body_path = "./output/divide_body";
	write_ply_rgb(divide_body_path, v, rgb_list);
}

void divide_upper_body(std::vector<Vector3> skeleton)
{
	Vector3 neck_born;
	Vector3 start_p;

	float a = 3;
	float b = 2;
	start_p.x = skeleton[0].x / a + b * skeleton[1].x / a;		//1(정확히는 1과 0의 1/3지점)에서 0으로 감
	start_p.y = skeleton[0].y / a + b * skeleton[1].y / a;
	start_p.z = skeleton[0].z / a + b * skeleton[1].z / a;

	//조정
	neck_born.x = (skeleton[0].x) - (start_p.x);
	neck_born.y = (skeleton[0].y) - (start_p.y);
	neck_born.z = (skeleton[0].z) - (start_p.z);

	std::vector<uchar3> pink_rgb_list;

	//내적값 구하기
	for (int i = 0; i < body_part.upper_list.size(); i++)
	{
		if (((body_part.upper_list[i].x - start_p.x) * neck_born.x + (body_part.upper_list[i].y - start_p.y) * neck_born.y + (body_part.upper_list[i].z - start_p.z) * neck_born.z) > 0)
		{
			pink_rgb_list.push_back({ 240, 20, 88 });	//핑크
			body_part.head_list.push_back(body_part.upper_list[i]);
		}
		else
		{
			//rgb_list.push_back({ 255, 0, 0 });		//이미 빨강
			body_part.no_head_list.push_back(body_part.upper_list[i]);

		}
	}

	string divide_body_path = "./output/divide_head";
	write_ply_rgb(divide_body_path, body_part.head_list, pink_rgb_list);


	Vector3 r_shoulder_born;
	Vector3 start_p1;
	Vector3 finish_p1;

	a = 5;
	b = 1;

	//8과 2의 b/a
	start_p1.x = (a - b) * skeleton[2].x / a + b * skeleton[8].x / a;		//앞 <-- 뒤 향하는 벡터
	start_p1.y = (a - b) * skeleton[2].y / a + b * skeleton[8].y / a;
	start_p1.z = (a - b) * skeleton[2].z / a + b * skeleton[8].z / a;

	//j2와 j3의 중간
	finish_p1.x = skeleton[2].x / 2 + skeleton[3].x / 2;
	finish_p1.y = skeleton[2].y / 2 + skeleton[3].y / 2;
	finish_p1.z = skeleton[2].z / 2 + skeleton[3].z / 2;

	//조정
	r_shoulder_born.x = finish_p1.x - start_p1.x;
	r_shoulder_born.y = finish_p1.y - start_p1.y;
	r_shoulder_born.z = finish_p1.z - start_p1.z;

	std::vector<uchar3> org_rgb_list;

	//내적값 구하기
	for (int i = 0; i < body_part.no_head_list.size(); i++)
	{
		if (((body_part.no_head_list[i].x - start_p1.x) * r_shoulder_born.x + (body_part.no_head_list[i].y - start_p1.y) * r_shoulder_born.y + (body_part.no_head_list[i].z - start_p1.z) * r_shoulder_born.z) > 0)
		{
			org_rgb_list.push_back({ 237, 92, 21 });	//오렌지
			body_part.upper_r_list.push_back(body_part.no_head_list[i]);
		}
		else
		{
			body_part.upper_rest_list.push_back(body_part.no_head_list[i]);
		}
	}

	string divide_upper_r_body_path = "./output/divide_upper_r";
	write_ply_rgb(divide_upper_r_body_path, body_part.upper_r_list, org_rgb_list);

	//string divide_upper_r_body_path = "./output/divide_upper_r";
	//write_ply_rgb(divide_upper_r_body_path, body_part.upper_r_list, rgb_list);


	Vector3 r_up_arm_born;
	Vector3 start_p2;

	a = 1;
	b = 1;

	start_p2.x = (a - b) * skeleton[3].x / a + b * skeleton[2].x / a;		//앞-->뒤 향하는 벡터
	start_p2.y = (a - b) * skeleton[3].y / a + b * skeleton[2].y / a;
	start_p2.z = (a - b) * skeleton[3].z / a + b * skeleton[2].z / a;

	//손은 아래 팔의 방향벡터 그대로 사용
	r_up_arm_born.x = (skeleton[3].x) - (start_p2.x);
	r_up_arm_born.y = (skeleton[3].y) - (start_p2.y);
	r_up_arm_born.z = (skeleton[3].z) - (start_p2.z);

	Vector3 r_d_arm_born;
	Vector3 start_p3;

	a = 1;
	b = 0;

	start_p3.x = (a - b) * skeleton[3].x / a + b * skeleton[4].x / a;		//앞-->뒤 향하는 벡터
	start_p3.y = (a - b) * skeleton[3].y / a + b * skeleton[4].y / a;
	start_p3.z = (a - b) * skeleton[3].z / a + b * skeleton[4].z / a;

	//조정
	r_d_arm_born.x = (skeleton[4].x) - (start_p3.x);
	r_d_arm_born.y = (skeleton[4].y) - (start_p3.y);
	r_d_arm_born.z = (skeleton[4].z) - (start_p3.z);

	Vector3 r_hand_born;
	Vector3 start_p4;

	start_p4.x = (a - b) * skeleton[4].x / a + b * skeleton[4].x / a;		//앞-->뒤 향하는 벡터
	start_p4.y = (a - b) * skeleton[4].y / a + b * skeleton[4].y / a;
	start_p4.z = (a - b) * skeleton[4].z / a + b * skeleton[4].z / a;

	//손은 아래 팔의 방향벡터 그대로 사용
	r_hand_born.x = (skeleton[4].x) - (start_p3.x);
	r_hand_born.y = (skeleton[4].y) - (start_p3.y);
	r_hand_born.z = (skeleton[4].z) - (start_p3.z);

	std::vector<uchar3> gray_yellow_rgb_list;
	std::vector<uchar3> yellow_rgb_list;
	std::vector<uchar3> light_org_rgb_list;
	std::vector<uchar3> rest_org_rgb_list;

	//내적값 구하기
	for (int i = 0; i < body_part.upper_r_list.size(); i++)
	{
		if (((body_part.upper_r_list[i].x - start_p4.x) * r_hand_born.x + (body_part.upper_r_list[i].y - start_p4.y) * r_hand_born.y + (body_part.upper_r_list[i].z - start_p4.z) * r_hand_born.z) > 0)
		{
			gray_yellow_rgb_list.push_back({ 162, 162, 9 });	//
			body_part.r_hand_list.push_back(body_part.upper_r_list[i]);
		}
		else if (((body_part.upper_r_list[i].x - start_p3.x) * r_d_arm_born.x + (body_part.upper_r_list[i].y - start_p3.y) * r_d_arm_born.y + (body_part.upper_r_list[i].z - start_p3.z) * r_d_arm_born.z) > 0)
		{
			yellow_rgb_list.push_back({ 215, 215, 1 });	//노랑
			body_part.r_lower_arm_list.push_back(body_part.upper_r_list[i]);
		}
		else if (((body_part.upper_r_list[i].x - start_p2.x) * r_up_arm_born.x + (body_part.upper_r_list[i].y - start_p2.y) * r_up_arm_born.y + (body_part.upper_r_list[i].z - start_p2.z) * r_up_arm_born.z) > 0)
		{
			light_org_rgb_list.push_back({ 237, 165, 22 });	//연한 주황
			body_part.r_upper_arm_list.push_back(body_part.upper_r_list[i]);

		}
		else
		{
			rest_org_rgb_list.push_back({ 237, 92, 21 });	//오렌지
			body_part.r_shoulder_list.push_back(body_part.upper_r_list[i]);
		}
	}

	string divide_r_hand_path = "./output/divide_r_hand";
	write_ply_rgb(divide_r_hand_path, body_part.r_hand_list, gray_yellow_rgb_list);

	string divide_r_lower_arm_body_path = "./output/divide_r_lower_arm";
	write_ply_rgb(divide_r_lower_arm_body_path, body_part.r_lower_arm_list, yellow_rgb_list);

	string divide_r_upper_arm_body_path = "./output/divide_r_upper_arm";
	write_ply_rgb(divide_r_upper_arm_body_path, body_part.r_upper_arm_list, light_org_rgb_list);

	string divide_r_shoulder_body_path = "./output/divide_r_shoulder";
	write_ply_rgb(divide_r_shoulder_body_path, body_part.r_shoulder_list, rest_org_rgb_list);


	//////////////////////////////왼팔
	Vector3 l_shoulder_born;
	Vector3 start_p1l;
	Vector3 finish_p1l;

	a = 5;
	b = 1;

	//8과 5의 b/a
	start_p1l.x = (a - b) * skeleton[5].x / a + b * skeleton[8].x / a;		//앞 <-- 뒤 향하는 벡터
	start_p1l.y = (a - b) * skeleton[5].y / a + b * skeleton[8].y / a;
	start_p1l.z = (a - b) * skeleton[5].z / a + b * skeleton[8].z / a;

	//j5와 j6의 중간
	finish_p1l.x = skeleton[5].x / 2 + skeleton[6].x / 2;
	finish_p1l.y = skeleton[5].y / 2 + skeleton[6].y / 2;
	finish_p1l.z = skeleton[5].z / 2 + skeleton[6].z / 2;

	//조정
	l_shoulder_born.x = finish_p1l.x - start_p1l.x;
	l_shoulder_born.y = finish_p1l.y - start_p1l.y;
	l_shoulder_born.z = finish_p1l.z - start_p1l.z;

	std::vector<uchar3> ygreen_rgb_list;
	std::vector<uchar3> rgb_list;

	//내적값 구하기
	for (int i = 0; i < body_part.upper_rest_list.size(); i++)
	{
		if (((body_part.upper_rest_list[i].x - start_p1l.x) * l_shoulder_born.x + (body_part.upper_rest_list[i].y - start_p1l.y) * l_shoulder_born.y + (body_part.upper_rest_list[i].z - start_p1l.z) * l_shoulder_born.z) > 0)
		{
			ygreen_rgb_list.push_back({ 165, 235, 20 });	//연두
			body_part.upper_l_list.push_back(body_part.upper_rest_list[i]);
		}
		else
		{
			rgb_list.push_back({ 255, 0, 0 });		//이미 빨강
			body_part.upper_rest_rest_list.push_back(body_part.upper_rest_list[i]);

		}
	}

	string divide_upper_l_body_path = "./output/divide_upper_l";
	write_ply_rgb(divide_upper_l_body_path, body_part.upper_l_list, ygreen_rgb_list);

	string divide_upper_rest_rest_body_path = "./output/divide_upper_rest_rest_body";
	write_ply_rgb(divide_upper_rest_rest_body_path, body_part.upper_rest_rest_list, rgb_list);


	Vector3 l_up_arm_born;
	Vector3 start_p2l;

	a = 1;
	b = 1;

	//j5, j6
	start_p2l.x = (a - b) * skeleton[6].x / a + b * skeleton[5].x / a;		//앞-->뒤 향하는 벡터
	start_p2l.y = (a - b) * skeleton[6].y / a + b * skeleton[5].y / a;
	start_p2l.z = (a - b) * skeleton[6].z / a + b * skeleton[5].z / a;

	//손은 아래 팔의 방향벡터 그대로 사용
	l_up_arm_born.x = (skeleton[6].x) - (start_p2l.x);
	l_up_arm_born.y = (skeleton[6].y) - (start_p2l.y);
	l_up_arm_born.z = (skeleton[6].z) - (start_p2l.z);

	Vector3 l_d_arm_born;
	Vector3 start_p3l;

	a = 1;
	b = 0;

	//j6, j7
	start_p3l.x = (a - b) * skeleton[6].x / a + b * skeleton[7].x / a;		//앞-->뒤 향하는 벡터
	start_p3l.y = (a - b) * skeleton[6].y / a + b * skeleton[7].y / a;
	start_p3l.z = (a - b) * skeleton[6].z / a + b * skeleton[7].z / a;

	//조정
	l_d_arm_born.x = (skeleton[7].x) - (start_p3l.x);
	l_d_arm_born.y = (skeleton[7].y) - (start_p3l.y);
	l_d_arm_born.z = (skeleton[7].z) - (start_p3l.z);

	Vector3 l_hand_born;
	Vector3 start_p4l;

	//j7, j7
	start_p4l.x = (a - b) * skeleton[7].x / a + b * skeleton[7].x / a;		//앞-->뒤 향하는 벡터
	start_p4l.y = (a - b) * skeleton[7].y / a + b * skeleton[7].y / a;
	start_p4l.z = (a - b) * skeleton[7].z / a + b * skeleton[7].z / a;

	//손은 아래 팔의 방향벡터 그대로 사용
	l_hand_born.x = (skeleton[7].x) - (start_p3l.x);
	l_hand_born.y = (skeleton[7].y) - (start_p3l.y);
	l_hand_born.z = (skeleton[7].z) - (start_p3l.z);

	std::vector<uchar3> gray_green_rgb_list;
	std::vector<uchar3> green_rgb_list;
	std::vector<uchar3> light_green_rgb_list;
	std::vector<uchar3> rest_ygreen_rgb_list;

	//내적값 구하기
	for (int i = 0; i < body_part.upper_l_list.size(); i++)
	{
		if (((body_part.upper_l_list[i].x - start_p4l.x) * l_hand_born.x + (body_part.upper_l_list[i].y - start_p4l.y) * l_hand_born.y + (body_part.upper_l_list[i].z - start_p4l.z) * l_hand_born.z) > 0)
		{
			gray_green_rgb_list.push_back({ 9, 161, 9 });	//gray green
			body_part.l_hand_list.push_back(body_part.upper_l_list[i]);
		}
		else if (((body_part.upper_l_list[i].x - start_p3l.x) * l_d_arm_born.x + (body_part.upper_l_list[i].y - start_p3l.y) * l_d_arm_born.y + (body_part.upper_l_list[i].z - start_p3l.z) * l_d_arm_born.z) > 0)
		{
			green_rgb_list.push_back({ 12, 215, 200 });	//초록
			body_part.l_lower_arm_list.push_back(body_part.upper_l_list[i]);
		}
		else if (((body_part.upper_l_list[i].x - start_p2l.x) * l_up_arm_born.x + (body_part.upper_l_list[i].y - start_p2l.y) * l_up_arm_born.y + (body_part.upper_l_list[i].z - start_p2l.z) * l_up_arm_born.z) > 0)
		{
			light_green_rgb_list.push_back({ 200, 226, 12 });	//연한 초록
			body_part.l_upper_arm_list.push_back(body_part.upper_l_list[i]);

		}
		else
		{
			rest_ygreen_rgb_list.push_back({ 165, 235, 20 });
			body_part.l_shoulder_list.push_back(body_part.upper_l_list[i]);
		}
	}

	string divide_l_hand_path = "./output/divide_l_hand";
	write_ply_rgb(divide_l_hand_path, body_part.l_hand_list, gray_green_rgb_list);

	string divide_l_lower_arm_body_path = "./output/divide_l_lower_arm";
	write_ply_rgb(divide_l_lower_arm_body_path, body_part.l_lower_arm_list, green_rgb_list);

	string divide_l_upper_arm_body_path = "./output/divide_l_upper_arm";
	write_ply_rgb(divide_l_upper_arm_body_path, body_part.l_upper_arm_list, light_green_rgb_list);

	string divide_l_shoulder_body_path = "./output/divide_l_shoulder";
	write_ply_rgb(divide_l_shoulder_body_path, body_part.l_shoulder_list, rest_ygreen_rgb_list);


}

void divide_lower_body(std::vector<Vector3> skeleton)
{
	float a = 3;
	float b = 2;

	//오른다리랑 왼다리 분리
	Vector3 start_p;
	Vector3 r_leg_bone;

	//8번 썼더니 결과 안 좋아서 
	start_p.x = skeleton[8].x;
	start_p.y = skeleton[8].y;
	start_p.z = skeleton[8].z;

	//9랑 12 중앙으로 변경했는데도 결과 안 좋음
	//start_p.x = (skeleton[9].x + skeleton[12].x)/2;
	//start_p.y = (skeleton[9].y + skeleton[12].y)/2;
	//start_p.z = (skeleton[9].z + skeleton[12].z)/2;

	r_leg_bone.x = skeleton[9].x - start_p.x;
	r_leg_bone.y = skeleton[9].y - start_p.y;
	r_leg_bone.z = skeleton[9].z - start_p.z;


	std::vector<uchar3> pink_rgb_list;
	std::vector<uchar3> green_rgb_list;

	//내적값 구하기
	for (int i = 0; i < body_part.lower_list.size(); i++)
	{
		if (((body_part.lower_list[i].x - start_p.x) * r_leg_bone.x + (body_part.lower_list[i].y - start_p.y) * r_leg_bone.y + (body_part.lower_list[i].z - start_p.z) * r_leg_bone.z) > 0)
		{
			pink_rgb_list.push_back({ 240, 20, 88 });	//핑크
			body_part.lower_r_list.push_back(body_part.lower_list[i]);
		}
		else
		{
			green_rgb_list.push_back({ 98, 195, 98 });		//연두
			body_part.lower_l_list.push_back(body_part.lower_list[i]);

		}
	}

	string divide_body_path = "./output/lower_r_list";
	write_ply_rgb(divide_body_path, body_part.lower_r_list, pink_rgb_list);

	divide_body_path = "./output/lower_l_list";
	write_ply_rgb(divide_body_path, body_part.lower_l_list, green_rgb_list);



	/*

	Vector3 r_up_leg_bone;
	Vector3 start_p2;

	a = 1;
	b = 1;

	start_p2.x = (a - b) * skeleton[3].x / a + b * skeleton[2].x / a;
	start_p2.y = (a - b) * skeleton[3].y / a + b * skeleton[2].y / a;
	start_p2.z = (a - b) * skeleton[3].z / a + b * skeleton[2].z / a;

	//손은 아래 팔의 방향벡터 그대로 사용
	r_up_leg_bone.x = (skeleton[3].x) - (start_p2.x);
	r_up_leg_bone.y = (skeleton[3].y) - (start_p2.y);
	r_up_leg_bone.z = (skeleton[3].z) - (start_p2.z);




	Vector3 r_d_arm_born;
	Vector3 start_p3;

	a = 1;
	b = 0;

	start_p3.x = (a - b) * skeleton[3].x / a + b * skeleton[4].x / a;
	start_p3.y = (a - b) * skeleton[3].y / a + b * skeleton[4].y / a;
	start_p3.z = (a - b) * skeleton[3].z / a + b * skeleton[4].z / a;

	//조정
	r_d_arm_born.x = (skeleton[4].x) - (start_p3.x);
	r_d_arm_born.y = (skeleton[4].y) - (start_p3.y);
	r_d_arm_born.z = (skeleton[4].z) - (start_p3.z);

	Vector3 r_foot_bone;
	Vector3 start_p4;

	start_p4.x = (a - b) * skeleton[4].x / a + b * skeleton[4].x / a;		//앞-->뒤 향하는 벡터
	start_p4.y = (a - b) * skeleton[4].y / a + b * skeleton[4].y / a;
	start_p4.z = (a - b) * skeleton[4].z / a + b * skeleton[4].z / a;

	//손은 아래 팔의 방향벡터 그대로 사용
	r_foot_bone.x = (skeleton[4].x) - (start_p3.x);
	r_foot_bone.y = (skeleton[4].y) - (start_p3.y);
	r_foot_bone.z = (skeleton[4].z) - (start_p3.z);

	std::vector<uchar3> gray_yellow_rgb_list;
	std::vector<uchar3> yellow_rgb_list;
	std::vector<uchar3> light_org_rgb_list;
	std::vector<uchar3> rest_org_rgb_list;

	//내적값 구하기
	for (int i = 0; i < body_part.lower_r_list.size(); i++)
	{
		if (((body_part.upper_r_list[i].x - start_p4.x) * r_foot_bone.x + (body_part.upper_r_list[i].y - start_p4.y) * r_foot_bone.y + (body_part.upper_r_list[i].z - start_p4.z) * r_foot_bone.z) > 0)
		{
			gray_yellow_rgb_list.push_back({ 162, 162, 9 });	//
			body_part.r_hand_list.push_back(body_part.upper_r_list[i]);
		}
		else if (((body_part.upper_r_list[i].x - start_p3.x) * r_d_arm_born.x + (body_part.upper_r_list[i].y - start_p3.y) * r_d_arm_born.y + (body_part.upper_r_list[i].z - start_p3.z) * r_d_arm_born.z) > 0)
		{
			yellow_rgb_list.push_back({ 215, 215, 1 });	//노랑
			body_part.r_lower_arm_list.push_back(body_part.upper_r_list[i]);
		}
		else if (((body_part.upper_r_list[i].x - start_p2.x) * r_up_leg_born.x + (body_part.upper_r_list[i].y - start_p2.y) * r_up_leg_born.y + (body_part.upper_r_list[i].z - start_p2.z) * r_up_leg_born.z) > 0)
		{
			light_org_rgb_list.push_back({ 237, 165, 22 });	//연한 주황
			body_part.r_upper_arm_list.push_back(body_part.upper_r_list[i]);

		}
		else
		{
			rest_org_rgb_list.push_back({ 237, 92, 21 });	//오렌지
			body_part.r_shoulder_list.push_back(body_part.upper_r_list[i]);
		}
	}

	string divide_r_hand_path = "./output/divide_r_hand";
	write_ply_rgb(divide_r_hand_path, body_part.r_hand_list, gray_yellow_rgb_list);

	string divide_r_lower_arm_body_path = "./output/divide_r_lower_arm";
	write_ply_rgb(divide_r_lower_arm_body_path, body_part.r_lower_arm_list, yellow_rgb_list);

	string divide_r_upper_arm_body_path = "./output/divide_r_upper_arm";
	write_ply_rgb(divide_r_upper_arm_body_path, body_part.r_upper_arm_list, light_org_rgb_list);

	string divide_r_shoulder_body_path = "./output/divide_r_shoulder";
	write_ply_rgb(divide_r_shoulder_body_path, body_part.r_shoulder_list, rest_org_rgb_list);


	*/
}

void glInit()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };		//이건 뭐여?!?

	GLfloat position[] = { 400.0, 300.0, -700.0, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glClearColor(1, 1, 1, 1);

	glMatrixMode(GL_PROJECTION);

	//glViewport(0, 0, win_w, win_h);

	glLoadIdentity();

	float clip_value = 3.3;
	//glOrtho(-clip_value, clip_value, -clip_value, clip_value, -clip_value, clip_value);	//실제 사이즈 2미만
	//glOrtho(-clip_value, clip_value, -clip_value, clip_value, -1, 1);	//실제 사이즈 2미만
	gluPerspective(60, 1, 1, 20);
}

void Drawminmax()
{
	Vector3 min;
	Vector3 max;

	min.x = -5.6;
	min.y = -2.6;
	min.z = 1.3;

	max.x = 0;
	max.y = -1.3;
	max.z = 6.5;

	glBegin(GL_POINTS);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(max.x, max.y, max.z);
	glEnd();

}

void DrawUpperBody(int x, int a, int b, int c)
{
	glPushMatrix();

	//glRotatef(90, 1, 0, 0);
	//glRotatef(x, a, b, c);

	glBegin(GL_POINTS);
	for (int i = 0; i < body_part_draw.upper_rest_rest_list.size(); i++)
	{
		glColor3f(rgb_red.red, rgb_red.green, rgb_red.blue);
		glVertex3f(body_part_draw.upper_rest_rest_list[i].x, body_part_draw.upper_rest_rest_list[i].y, body_part_draw.upper_rest_rest_list[i].z);
	}
	glEnd();

	glPopMatrix();
}

void DrawHead(int x, int a, int b, int c)
{
	//printf("head 그리고 있음\n");

	glPushMatrix();		//저장
	//glTranslatef(0, 0.02, 0.0);		//머리 시작점

	glBegin(GL_POINTS);
	for (int i = 0; i < body_part_draw.head_list.size(); i++)
	{
		glColor3f(rgb_red.red, rgb_red.green, rgb_red.blue);
		//glColor3f(rgb_pink.red, rgb_pink.green, rgb_pink.blue);
		glVertex3f(body_part_draw.head_list[i].x, body_part_draw.head_list[i].y, body_part_draw.head_list[i].z);
	}
	glEnd();

	//왼쪽 뿔 그리기

	glPopMatrix();
	//오른쪽 뿔 그리기
	//왼쪽 눈 그리기
	//오른쪽 눈 그리기

}

void DrawRShoulder(int x, int a, int b, int c)
{
	glPushMatrix();		//저장

	glTranslatef(point_vec2[2].x, point_vec2[2].y, point_vec2[2].z);		//머리 시작점
	glRotatef(x, a, b, c);

	glBegin(GL_POINTS);
	for (int i = 0; i < body_part_draw.r_shoulder_list.size(); i++)
	{
		glColor3f(rgb_red.red, rgb_red.green, rgb_red.blue);
		//glColor3f(rgb_org.red, rgb_org.green, rgb_org.blue);
		glVertex3f(body_part_draw.r_shoulder_list[i].x, body_part_draw.r_shoulder_list[i].y, body_part_draw.r_shoulder_list[i].z);
	}
	glEnd();

	glPopMatrix();
}

void DrawRUpperArm(int x, int a, int b, int c)
{
	glPushMatrix();		//저장

	glRotatef(-5.0, 1, 0, 0);
	glTranslatef(point_vec2[2].x, point_vec2[2].y, point_vec2[2].z);		//머리 시작점
	glRotatef(x, a, b, c);

	glBegin(GL_POINTS);
	for (int i = 0; i < body_part_draw.r_upper_arm_list.size(); i++)
	{
		glColor3f(rgb_red.red, rgb_red.green, rgb_red.blue);
		//glColor3f(rgb_org.red, rgb_org.green, rgb_org.blue);
		glVertex3f(body_part_draw.r_upper_arm_list[i].x, body_part_draw.r_upper_arm_list[i].y, body_part_draw.r_upper_arm_list[i].z);
	}
	glEnd();

	glPopMatrix();
}

void DrawRLowerArm(int x, int a, int b, int c)
{
	glPushMatrix();		//저장

	glTranslatef(point_vec2[3].x, point_vec2[3].y, point_vec2[3].z);		//머리 시작점

	glRotatef(x, a, b, c);

	glBegin(GL_POINTS);
	for (int i = 0; i < body_part_draw.r_lower_arm_list.size(); i++)
	{
		glColor3f(rgb_red.red, rgb_red.green, rgb_red.blue);
		//glColor3f(rgb_org.red, rgb_org.green, rgb_org.blue);
		glVertex3f(body_part_draw.r_lower_arm_list[i].x, body_part_draw.r_lower_arm_list[i].y, body_part_draw.r_lower_arm_list[i].z);
	}
	glEnd();

	glPopMatrix();
}

void DrawRHand(int x, int a, int b, int c)
{
	glPushMatrix();		//저장

	glTranslatef(point_vec2[3].x, point_vec2[3].y, point_vec2[3].z);		//머리 시작점

	glRotatef(x, a, b, c);

	glBegin(GL_POINTS);
	for (int i = 0; i < body_part_draw.r_hand_list.size(); i++)
	{
		glColor3f(rgb_red.red, rgb_red.green, rgb_red.blue);
		//glColor3f(rgb_org.red, rgb_org.green, rgb_org.blue);
		glVertex3f(body_part_draw.r_hand_list[i].x, body_part_draw.r_hand_list[i].y, body_part_draw.r_hand_list[i].z);
	}
	glEnd();

	glPopMatrix();
}

void DrawLShoulder(int x, int a, int b, int c)
{
	glPushMatrix();		//저장

	glTranslatef(point_vec2[5].x, point_vec2[5].y, point_vec2[5].z);		//머리 시작점?


	glBegin(GL_POINTS);
	for (int i = 0; i < body_part_draw.l_shoulder_list.size(); i++)
	{
		glColor3f(rgb_red.red, rgb_red.green, rgb_red.blue);
		//glColor3f(rgb_org.red, rgb_org.green, rgb_org.blue);
		glVertex3f(body_part_draw.l_shoulder_list[i].x, body_part_draw.l_shoulder_list[i].y, body_part_draw.l_shoulder_list[i].z);
	}
	glEnd();

	glPopMatrix();
}

void DrawLUpperArm(int x, int a, int b, int c)
{
	glPushMatrix();		//저장

	glTranslatef(point_vec2[5].x, point_vec2[5].y, point_vec2[5].z);		//머리 시작점?

	glRotatef(x, a, b, c);

	glBegin(GL_POINTS);
	for (int i = 0; i < body_part_draw.l_upper_arm_list.size(); i++)
	{
		glColor3f(rgb_red.red, rgb_red.green, rgb_red.blue);
		//glColor3f(rgb_org.red, rgb_org.green, rgb_org.blue);
		glVertex3f(body_part_draw.l_upper_arm_list[i].x, body_part_draw.l_upper_arm_list[i].y, body_part_draw.l_upper_arm_list[i].z);
	}
	glEnd();

	glPopMatrix();
}

void DrawLLowerArm(int x, int a, int b, int c)
{
	glPushMatrix();		//저장

	glTranslatef(point_vec2[6].x, point_vec2[6].y, point_vec2[6].z);		//머리 시작점?

	glRotatef(x, a, b, c);

	glBegin(GL_POINTS);
	for (int i = 0; i < body_part_draw.l_lower_arm_list.size(); i++)
	{
		glColor3f(rgb_red.red, rgb_red.green, rgb_red.blue);
		//glColor3f(rgb_org.red, rgb_org.green, rgb_org.blue);
		glVertex3f(body_part_draw.l_lower_arm_list[i].x, body_part_draw.l_lower_arm_list[i].y, body_part_draw.l_lower_arm_list[i].z);
	}
	glEnd();

	glPopMatrix();
}

void DrawLHand(int x, int a, int b, int c)
{
	glPushMatrix();		//저장

	glTranslatef(point_vec2[6].x, point_vec2[6].y, point_vec2[6].z);		//머리 시작점?

	glRotatef(x, a, b, c);

	glBegin(GL_POINTS);
	for (int i = 0; i < body_part_draw.l_hand_list.size(); i++)
	{
		glColor3f(rgb_red.red, rgb_red.green, rgb_red.blue);
		//glColor3f(rgb_org.red, rgb_org.green, rgb_org.blue);
		glVertex3f(body_part_draw.l_hand_list[i].x, body_part_draw.l_hand_list[i].y, body_part_draw.l_hand_list[i].z);
	}
	glEnd();

	glPopMatrix();
}

void DrawLowerBody(int x, int a, int b, int c)
{
	glPushMatrix();		//저장

	glTranslatef(point_vec2[8].x, point_vec2[8].y, point_vec2[8].z);		//머리 시작점?

	glRotatef(x, a, b, c);

	glBegin(GL_POINTS);
	for (int i = 0; i < body_part_draw.lower_list.size(); i++)
	{
		glColor3f(rgb_red.red, rgb_red.green, rgb_red.blue);
		//glColor3f(rgb_org.red, rgb_org.green, rgb_org.blue);
		glVertex3f(body_part_draw.lower_list[i].x, body_part_draw.lower_list[i].y, body_part_draw.lower_list[i].z);
	}
	glEnd();

	glPopMatrix();
}

void DrawVolumetricHuman()
{
	glPointSize(3.0);

	//Drawminmax();		//상관없는듯

	DrawHead(0, 0, 0, 0);
	DrawUpperBody(0, 0, 0, 0);	//이것만 그렸을 때 위치 변경 가능?

	DrawRShoulder(0, 0, 0, 0);
	DrawRUpperArm(r_arm_u, 0, 0, 1);
	DrawRLowerArm(r_arm_d, 0, 0, 1);
	DrawRHand(0, 0, 0, 0);

	DrawLShoulder(0, 0, 0, 0);
	DrawLUpperArm(r_arm_u, 0, 0, 1);
	DrawLLowerArm(l_arm_d, 0, 0, 1);
	DrawLHand(0, 0, 0, 0);

	DrawLowerBody(0, 0, 0, 0);
	//glFlush();
}

void Run()
{
	glLoadIdentity();	//CTM 초기화

//수정할 파트
	l_arm_u = sin(time1) * 80;	//왼팔은 80도까지 움직이는데 sin으로 주기적인 움직임 설정
	r_arm_u = -l_arm_u;			//오른팔은 왼팔 반대로 움직임

	r_arm_d = -fabs(sin(time1) * 60 + 50);
	l_arm_d = -fabs(-sin(time1) * 60 + 50);
	//
	//	r_leg_d = fabs(-sin(time) * 30 - 30);
	//	l_leg_d = fabs(sin(time) * 30 - 30);
	//
	//	r_leg_u = sin(time) * 60;
	//	l_leg_u = -r_leg_u;
	//
	//	cyl = gluNewQuadric();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//초기화

	glMatrixMode(GL_MODELVIEW);

	//	DrawGround();

	glLoadIdentity();	//CTM 초기화
	//로봇 달리면서 뒤뚱거리는 부분
	//glRotatef();

	//로봇 달리면서 상하로 움직이는 부분
	//float i = 0;

	glPushMatrix();		//CTM 저장
	//glTranslatef(0,i,0);
	//glTranslatef(0, 0.5, 0.0);

	//변경
	float center_x = -2.8;
	float center_y = -1.9;
	float center_z = 3.8;

	int x_up = -1;
	int y_up = 0;
	int z_up = 0;

	gluLookAt(center_x - 1, center_y + 0.7, center_z, center_x, center_y, center_z, x_up, y_up, z_up);
	//gluLookAt(0, 1, 0, 0, 0, 0, 1, 0, 0);

	DrawVolumetricHuman();

	glutSwapBuffers();
}

void MyMainMenu(int entryID)
{
	pose = entryID;
}

void MyTimer(int Value)
{
	time1 = time1 + 0.1;
	time4 = time4 + 1;

	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);
}

void MyDisplay()
{
	if (pose == RUN)
	{
		Run();
		glPopMatrix();
	}
	//else if (key == EXIT)
	//{
	//	ex();
	//	glPopMatrix();
	//}
}

void project_viewer_glut(int argc, char** argv)
{
	minmax_3D upper_body_minmax;
	minmax_3Dvalue(body_part_draw.upper_list, upper_body_minmax);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(win_w, win_h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Project::Robot Motion");

	glInit();

	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddMenuEntry("Run", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutTimerFunc(40, MyTimer, 1);
	glutDisplayFunc(MyDisplay);

	glutMainLoop();
}



void minmax_2Dvalue(cv::Mat projection_image, minmax_2D& aabb)
{

	aabb.max_2D_x = -1000000.0;
	aabb.min_2D_x = 1000000.0;

	aabb.max_2D_y = -1000000.0;
	aabb.min_2D_y = 1000000.0;

	for (int x = 0; x < projection_image.cols; x++)
	{
		for (int y = 0; y < projection_image.cols; y++)
		{
			if (projection_image.at<cv::Vec3b>(cv::Point(x, y)) != cv::Vec3b(178, 178, 178)) {	//배경 아니면
				if (x > aabb.max_2D_x) {
					aabb.max_2D_x = x;
				}
				if (x < aabb.min_2D_x) {
					aabb.min_2D_x = x;
				}
				if (y > aabb.max_2D_y) {
					aabb.max_2D_y = y;
				}
				if (y < aabb.min_2D_y) {
					aabb.min_2D_y = y;
				}

			}

		}
	}

	//값 확인
	printf("max {x, y}={%f, %f}\n", aabb.max_2D_x, aabb.max_2D_y);
	printf("min {x, y}={%f, %f}\n", aabb.min_2D_x, aabb.min_2D_y);

	//	float3 xy_min = { min_x,min_y };
	//	float3 xy_max = { max_x ,max_y };

}

void minmax_3Dvalue(vector<Vector3> point, minmax_3D& aabb)
{

	aabb.max_3D_x = -1000000.0;
	aabb.min_3D_x = 1000000.0;

	aabb.max_3D_y = -1000000.0;
	aabb.min_3D_y = 1000000.0;

	aabb.max_3D_z = -1000000.0;
	aabb.min_3D_z = 1000000.0;

	for (int i = 0; i < point.size(); i++)
	{
		if ((point[i].x > aabb.max_3D_x)) {
			aabb.max_3D_x = point[i].x;
		}
		if (point[i].x < aabb.min_3D_x) {
			aabb.min_3D_x = point[i].x;
		}

		if ((point[i].y > aabb.max_3D_y)) {
			aabb.max_3D_y = point[i].y;
		}
		if ((point[i].y < aabb.min_3D_y)) {
			aabb.min_3D_y = point[i].y;
		}

		if ((point[i].z > aabb.max_3D_z)) {
			aabb.max_3D_z = point[i].z;
		}
		if ((point[i].z < aabb.min_3D_z)) {
			aabb.min_3D_z = point[i].z;
		}

	}

	//	printf("max {x, y, z}={%f, %f, %f}\n", aabb.max_3D_x, aabb.max_3D_y, aabb.max_3D_z);
	//	printf("min {x, y, z}={%f, %f, %f}\n", aabb.min_3D_x, aabb.min_3D_y, aabb.min_3D_z);

		//	float3 xyz_min = { min_x,min_y ,min_z };
		//	float3 xyz_max = { max_x ,max_y  ,max_z };

}

void write_ply_rgb(const std::string& filename, std::vector<Vector3>& vertices, std::vector<uchar3>& rgb)
{
	std::filebuf fb_ascii;
	fb_ascii.open(filename + ".ply", std::ios::out);
	std::ostream outstream_ascii(&fb_ascii);

	if (outstream_ascii.fail()) throw std::runtime_error("failed to open " + filename);

	PlyFile object_file;
	object_file.add_properties_to_element("vertex", { "x", "y", "z" }, Type::FLOAT32, vertices.size(), reinterpret_cast<uint8_t*>(vertices.data()), Type::INVALID, 0);
	object_file.add_properties_to_element("vertex", { "red", "green", "blue" }, Type::UINT8, rgb.size(), reinterpret_cast<uint8_t*>(rgb.data()), Type::INVALID, 0);
	object_file.get_comments().push_back("generated by tinyply 2.2");
	object_file.write(outstream_ascii, false);

}

void write_ply_edge(const std::string& filename, std::vector<Vector3>& vertices, std::vector<int>& edge1, std::vector<int>& edge2)						//, std::vector<uchar3>& edge_color)
{
	std::filebuf fb_ascii;
	fb_ascii.open(filename + ".ply", std::ios::out);

	cout << filename << endl;

	std::ostream outstream_ascii(&fb_ascii);

	if (outstream_ascii.fail()) throw std::runtime_error("failed to open " + filename);

	PlyFile object_file;
	object_file.add_properties_to_element("vertex", { "x", "y", "z" }, Type::FLOAT32, vertices.size(), reinterpret_cast<uint8_t*>(vertices.data()), Type::INVALID, 0);
	//object_file.add_properties_to_element("vertex", { "red", "green", "blue" }, Type::UINT8, rgb.size(), reinterpret_cast<uint8_t*>(rgb.data()), Type::INVALID, 0);
	object_file.get_comments().push_back("generated by tinyply 2.2");
	object_file.add_properties_to_element("edge", { "vertex1" }, Type::INT32, edge1.size(), reinterpret_cast<uint8_t*>(edge1.data()), Type::INVALID, 0);
	object_file.add_properties_to_element("edge", { "vertex2" }, Type::INT32, edge2.size(), reinterpret_cast<uint8_t*>(edge2.data()), Type::INVALID, 0);
	//	object_file.add_properties_to_element("edge", { "red", "green", "blue" }, Type::UINT8, edge_color.size(), reinterpret_cast<uint8_t*>(edge_color.data()), Type::INVALID, 0);
	object_file.write(outstream_ascii, false);

}

void FindRealValue_aabb_3D(std::vector<Json::Value> joint_list, int frame, vector<Vector3> point_3D, cv::Mat aabb_image_x, cv::Mat aabb_image_z, vector<Vector3>& joint_3D)
{
	int print = 0;	//flag

	int direction = 4;
	float confidence = 0.45;

	float img_size = 1000;

	//3D
	float X[4][19];
	float Y[4][19];
	float Z[4][19];

	float C[4][19];

	//1. 2D to 3D
	for (int direction_num = 0; direction_num < direction; direction_num++)
	{
		//2D
		float x;
		float y;

		float c;


		for (int nn = 0; nn < 19; nn++)
		{
			if (print)	//값 들어왔는지
			{
				printf_s("joint_list[%d][%d][%d].x = %f\n", frame, direction_num, nn, joint_list[frame][direction_num][nn * 3].asFloat());
				printf_s("joint_list[%d][%d][%d].y = %f\n", frame, direction_num, nn, joint_list[frame][direction_num][(nn * 3) + 1].asFloat());
				printf_s("joint_list[%d][%d][%d].c = %f\n", frame, direction_num, nn, joint_list[frame][direction_num][(nn * 3) + 2].asFloat());
			}

			x = joint_list[frame][direction_num][nn * 3].asFloat();			//
			y = joint_list[frame][direction_num][(nn * 3) + 1].asFloat();	//
			c = joint_list[frame][direction_num][(nn * 3) + 2].asFloat();

			if (direction_num == 0)	//앞면
			{
				Z[direction_num][nn] = img_size - x;
				X[direction_num][nn] = y;
				C[direction_num][nn] = c;

			}
			else if (direction_num == 2)	//뒷면
			{
				Z[direction_num][nn] = x;
				X[direction_num][nn] = y;
				C[direction_num][nn] = c;
			}
			else if (direction_num == 1)	// 옆1
			{
				Y[direction_num][nn] = x;
				X[direction_num][nn] = y;
				C[direction_num][nn] = c;
			}
			else if (direction_num == 3)	//옆2
			{
				Y[direction_num][nn] = (img_size - x);
				X[direction_num][nn] = y;
				C[direction_num][nn] = c;
			}
			else
			{
				printf("그럴리 없음...\n");
			}

		}
	}


	//1.5. 스케일 값 계산
	float scale_x;	//원래는 두개 같아야함
	float scale_y;
	float scale_z;

	float center_2D_x;
	float center_2D_y;
	float center_2D_z;

	float center_3D_x;
	float center_3D_y;
	float center_3D_z;

	minmax_2D aabb_2D_x;
	minmax_2D aabb_2D_z;
	minmax_3D aabb_3D;


	minmax_2Dvalue(aabb_image_x, aabb_2D_x);	//비율 일정하면 다 볼 필요도 없음? center 맞춰줘야해서 다 확인해야할듯
	minmax_2Dvalue(aabb_image_z, aabb_2D_z);	//비율 일정하면 다 볼 필요도 없음? center 맞춰줘야해서 다 확인해야할듯

	minmax_3Dvalue(point_3D, aabb_3D);

	//	scale = (aabb_2D.max_2D_x - aabb_2D.min_2D_x) / (aabb_3D.max_3D_x - aabb_3D.min_3D_x);
	scale_x = (aabb_3D.max_3D_x - aabb_3D.min_3D_x) / (aabb_2D_x.max_2D_y - aabb_2D_x.min_2D_y);
	scale_y = (aabb_3D.max_3D_y - aabb_3D.min_3D_y) / (aabb_2D_x.max_2D_x - aabb_2D_x.min_2D_x);
	scale_z = (aabb_3D.max_3D_z - aabb_3D.min_3D_z) / (aabb_2D_z.max_2D_x - aabb_2D_z.min_2D_x);

	center_2D_x = (aabb_2D_x.max_2D_y + aabb_2D_x.min_2D_y) / 2;	//위 스케일이랑 순서 같아야함(2Dxyz)
	center_2D_y = (aabb_2D_x.max_2D_x + aabb_2D_x.min_2D_x) / 2;
	center_2D_z = (aabb_2D_z.max_2D_x + aabb_2D_z.min_2D_x) / 2;

	center_3D_x = (aabb_3D.max_3D_x + aabb_3D.min_3D_x) / 2;
	center_3D_y = (aabb_3D.max_3D_y + aabb_3D.min_3D_y) / 2;
	center_3D_z = (aabb_3D.max_3D_z + aabb_3D.min_3D_z) / 2;




	//2. intersection calculation
	for (int nn = 0; nn < 15; nn++)
	{
		float inter_X = 0;
		float inter_Y = 0;
		float inter_Z = 0;

		int count_X = 0;
		int count_Y = 0;
		int count_Z = 0;


		for (int direction_num = 0; direction_num < direction; direction_num++)
		{
			if (direction_num == 0 && C[direction_num][nn] > 0.45)		//앞면
			{
				inter_Z += Z[direction_num][nn];
				count_Z++;

				inter_X += X[direction_num][nn];
				count_X++;

			}
			else if (direction_num == 2 && C[direction_num][nn] > 0.45)	//뒷면
			{
				inter_Z += Z[direction_num][nn];
				count_Z++;

				inter_X += X[direction_num][nn];
				count_X++;
			}
			else if (direction_num == 1 && C[direction_num][nn] > 0.45)	//옆면1
			{
				inter_Y += Y[direction_num][nn];
				count_Y++;

				inter_X += X[direction_num][nn];
				count_X++;
			}
			else if (direction_num == 1 && C[direction_num][nn] > 0.45)	//옆면2
			{
				inter_Y += Y[direction_num][nn];
				count_Y++;

				inter_X += X[direction_num][nn];
				count_X++;
			}

			//nan값 방지
			if (count_Z == 0)
			{
				printf_s("count_Z = 0 !!! joint[%d]\n", nn);
				if (C[0][nn] > C[2][nn])
				{
					inter_Z += Z[0][nn];
					count_Z++;

					inter_X += X[0][nn];
					count_X++;

				}
				else if (C[0][nn] < C[2][nn])
				{
					inter_Z += Z[2][nn];
					count_Z++;

					inter_X += X[2][nn];
					count_X++;

				}
				else
				{
					printf_s("confidence[direction=0]과 confidence[direction=2]이 같음!!!\n");
				}
			}
			else if (count_Y == 0)	//Y값 못 찾으면
			{
				printf_s("신뢰도가 0.45 이상인 count_Y = 0 !!! joint[%d]\n", nn);
				if (C[1][nn] > C[3][nn])	//옆면 1, 2 비교해서 더 높은 신뢰도 가진 y값 사용하는 방법
				{
					inter_Y += Y[1][nn];
					count_Y++;

					inter_X += X[1][nn];
					count_X++;
				}
				else if (C[1][nn] < C[3][nn])
				{
					inter_Y += Y[3][nn];
					count_Y++;

					inter_X += X[3][nn];
					count_X++;
				}
				else
				{
					printf("confidence[direction=1]과 confidence[direction=3]이 같음!!!\n");
				}

			}
			else if (count_X == 0)
			{
				printf("3D X값이 없을 수도 있구나,,, 사방에서 4개나 나오는데,,,,\n");
			}
			else
			{
				//
			}
		}




		inter_X /= count_X;
		inter_Y /= count_Y;
		inter_Z /= count_Z;

		//centering 2D
		inter_X -= (center_2D_x);		//center_2D 값이 500에 가까운지 확인
		inter_Y -= (center_2D_y);
		inter_Z -= (center_2D_z);

		//	inter_X -= (500);		//center_2D 값이 500에 가까운지 확인
		//	inter_Y -= (500);
		//	inter_Z -= (500);



			//3. scaling
		inter_X *= scale_x;	//스케일 값 다 같은지 확인
		inter_Y *= scale_y;
		inter_Z *= scale_z;

		//printf("center 3D xyz : %f %f %f\n", center_3D_x, center_3D_y, center_3D_z);

		//4. centering 3D
		inter_X += (center_3D_x);		//center_2D 값이 500에 가까운지 확인
		inter_Y += (center_3D_y);
		inter_Z += (center_3D_z);

		float center_y;
		//	if (count_Y == 0)	//Y값 없으면
		//	{
		//		center_y = 0;
		//		printf_s("신뢰도가 0.45 이상인 count_Y = 0 !!! joint[%d]\n", nn);
		//		//앞면에서 해당 joint의 xz범위 +-5센치 정도면 y값의 최대/최소 값 구해서 center 값을 이용하는 방법
		//		float dist = 0.5;
		//		float y_min = 99999;
		//		float y_max = -99999;
		//		for (int i = 0; i < point_3D.size(); i++)
		//		{
		//			if ((inter_X / count_X) - dist < point_3D[i].x && point_3D[i].x < (inter_X / count_X) + dist)
		//			{
		//				if ((inter_Z / count_Z) - dist < point_3D[i].z && point_3D[i].z < (inter_Z / count_Z) + dist)
		//				{
		//					printf("여기에 들어오긴 하나?\n");
		//					if (y_min > point_3D[i].y)
		//					{
		//						y_min = point_3D[i].y;
		//					}
		//					if (y_max < point_3D[i].y)
		//					{
		//						y_max = point_3D[i].y;
		//					}
		//				}
		//			}
		//		}
		//		printf("y_min, max %f %f\n", y_min, y_max);
		//		center_y = (y_max + y_min) / 2;
		//	}

		if (count_Y == 0)
		{
			joint_3D.push_back({ inter_X, inter_Y, inter_Z });		//center_y

		}
		else
		{
			joint_3D.push_back({ inter_X, inter_Y, inter_Z });

		}


	}

	//다 끝나고 나서 골반 중앙은 골반 양쪽 평균 내기 ==> 제외해봄
	//joint_3D[8].x = (joint_3D[9].x + joint_3D[12].x) / 2;
	//joint_3D[8].y = (joint_3D[9].y + joint_3D[12].y) / 2;
	//joint_3D[8].z = (joint_3D[9].z + joint_3D[12].z) / 2;


}