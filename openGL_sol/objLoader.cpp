#include <stdio.h>
#include <fstream>	//ifstream file
#include <iostream>	//cout
#include <string>	//getline()
#include <vector>	//vector
//#include <glfw-imgui/include/imgui.h>

#include "objLoader.h"

using namespace std;

bool normal_bool = true;
bool RGB_bool = true;
bool capture = false;

bool pm = true;

float tr_xyz1[3] = { 0,0.263,0 };
float offset_value1[3] = { -3,0.5,0 };// { -1.8,0.677,0 };
float tr_xyz2[3] = { 0,0,2.0 };
float offset_value2[3] = { -3,0,0 };
float tr_xyz3[3] = { 0,0,2.0 };
float offset_value3[3] = { -3,-0.5,0 }; //{ -2.3,-0.677,0 };
float zvalue[2] = { 0.081,2.8 };
float scale = 0.9;
float		ceiling = -0.94;



vector<string> split(string s, string divid)
{
	vector<string> v;
	char* c = strtok((char*)s.c_str(), divid.c_str());
	while (c) {
		v.push_back(c);
		c = strtok(NULL, divid.c_str());
	}
	return v;
}

void OBJLoader(string path, vector<data_seq>& sequence, vector<int>& texture_num)
{
	ifstream file;

	file.open(path);
	if (!file.good())
	{
		cout << "ERROR::failed to open!!!::" << path << endl;
	}

	string line;
	int temp_count = 0;


	vector<Vector3>v;
	vector<TexCoord> t;

	vector<element_index> fi;
	vector<element_index> ui;
	vector<element_index> ni;

	vector<element_index> fi2;
	vector<element_index> ui2;
	vector<element_index> ni2;

	vector<element_index> fi3;
	vector<element_index> ui3;
	vector<element_index> ni3;

	while (getline(file, line))
	{
		string text;

		file >> text;

		if (text == "v")
		{
			Vector3 vertex;	//??

			file >> vertex.x;
			file >> vertex.y;
			file >> vertex.z;


			v.push_back(vertex);
		}

		if (text == "vt")
		{
			TexCoord texCoord;

			file >> texCoord.u;
			file >> texCoord.v;

			t.push_back(texCoord);
		}

		if (text == "usemtl")
		{
			//printf("break\n");
			temp_count++;
			//break;
		}

		if (temp_count == 1) {
			if (text == "f")
			{
				string value1;
				string value2;
				string value3;

				file >> value1;
				file >> value2;
				file >> value3;

				vector<string> face_index = split(value1, "/");
				vector<string> uv_index = split(value2, "/");
				vector<string> normal_index = split(value3, "/");

				int face_index1 = std::atoi(face_index[0].c_str());
				int uv_index1 = std::atoi(face_index[1].c_str());
				int normal_index1 = std::atoi(face_index[2].c_str());

				int face_index2 = std::atoi(uv_index[0].c_str());
				int uv_index2 = std::atoi(uv_index[1].c_str());
				int normal_index2 = std::atoi(uv_index[2].c_str());

				int face_index3 = std::atoi(normal_index[0].c_str());
				int uv_index3 = std::atoi(normal_index[1].c_str());
				int normal_index3 = std::atoi(normal_index[2].c_str());

				element_index face_value_struct = { face_index1, face_index2, face_index3 };
				element_index uv_value_struct = { uv_index1, uv_index2, uv_index3 };
				element_index normal_value_struct = { normal_index1, normal_index2, normal_index3 };

				//printf("uv_index1 %d %d %d\n", uv_index1, uv_index2, uv_index3);

				fi.push_back(face_value_struct);
				ui.push_back(uv_value_struct);
				ni.push_back(normal_value_struct);

			}
		}

		if (temp_count == 2) {
			if (text == "f")
			{
				string value1;
				string value2;
				string value3;

				file >> value1;
				file >> value2;
				file >> value3;

				vector<string> face_index = split(value1, "/");
				vector<string> uv_index = split(value2, "/");
				vector<string> normal_index = split(value3, "/");

				int face_index1 = std::atoi(face_index[0].c_str());
				int uv_index1 = std::atoi(face_index[1].c_str());
				int normal_index1 = std::atoi(face_index[2].c_str());

				int face_index2 = std::atoi(uv_index[0].c_str());
				int uv_index2 = std::atoi(uv_index[1].c_str());
				int normal_index2 = std::atoi(uv_index[2].c_str());

				int face_index3 = std::atoi(normal_index[0].c_str());
				int uv_index3 = std::atoi(normal_index[1].c_str());
				int normal_index3 = std::atoi(normal_index[2].c_str());

				element_index face_value_struct = { face_index1, face_index2, face_index3 };
				element_index uv_value_struct = { uv_index1, uv_index2, uv_index3 };
				element_index normal_value_struct = { normal_index1, normal_index2, normal_index3 };

				//printf("uv_index2 %d %d %d\n", uv_index1, uv_index2, uv_index3);
				//	printf("%d %d %d\n", face_index1, face_index2, face_index3);

				fi2.push_back(face_value_struct);
				ui2.push_back(uv_value_struct);
				ni2.push_back(normal_value_struct);

				//printf("push_back1()!\n");
			}
		}

		if (temp_count == 3) {
			if (text == "f")
			{
				string value1;
				string value2;
				string value3;

				file >> value1;
				file >> value2;
				file >> value3;

				vector<string> face_index = split(value1, "/");
				vector<string> uv_index = split(value2, "/");
				vector<string> normal_index = split(value3, "/");

				int face_index1 = std::atoi(face_index[0].c_str());
				int uv_index1 = std::atoi(face_index[1].c_str());
				int normal_index1 = std::atoi(face_index[2].c_str());

				int face_index2 = std::atoi(uv_index[0].c_str());
				int uv_index2 = std::atoi(uv_index[1].c_str());
				int normal_index2 = std::atoi(uv_index[2].c_str());

				int face_index3 = std::atoi(normal_index[0].c_str());
				int uv_index3 = std::atoi(normal_index[1].c_str());
				int normal_index3 = std::atoi(normal_index[2].c_str());

				element_index face_value_struct = { face_index1, face_index2, face_index3 };
				element_index uv_value_struct = { uv_index1, uv_index2, uv_index3 };
				element_index normal_value_struct = { normal_index1, normal_index2, normal_index3 };

				//printf("uv_index2 %d %d %d\n", uv_index1, uv_index2, uv_index3);
				//	printf("%d %d %d\n", face_index1, face_index2, face_index3);

				fi3.push_back(face_value_struct);
				ui3.push_back(uv_value_struct);
				ni3.push_back(normal_value_struct);

				//printf("push_back1()!\n");
			}
		}
	}


	sequence.push_back({ v, t, fi, ui, ni, fi2, ui2, ni2, fi3, ui3, ni3 });

	texture_num.push_back(temp_count);
}


void OBJLoader_noTex(string path, vector<data_seq>& sequence)
{
	ifstream file;

	file.open(path);
	if (!file.good())
	{
		cout << "ERROR::failed to open!!!::" << path << endl;
	}

	string line;
	int temp_count = 0;


	vector<Vector3>v;
	vector<TexCoord> t;

	vector<element_index> fi;
	vector<element_index> ui;
	vector<element_index> ni;

	vector<element_index> fi2;
	vector<element_index> ui2;
	vector<element_index> ni2;

	vector<element_index> fi3;
	vector<element_index> ui3;
	vector<element_index> ni3;

	while (getline(file, line))
	{
		string text;

		file >> text;

		if (text == "v")
		{
			Vector3 vertex;	//??

			file >> vertex.x;
			file >> vertex.y;
			file >> vertex.z;


			v.push_back(vertex);
		}

		if (text == "vt")
		{
			TexCoord texCoord;

			file >> texCoord.u;
			file >> texCoord.v;

			t.push_back(texCoord);
		}

		if (text == "usemtl")
		{
			//printf("break\n");
			temp_count++;
			//break;
		}

		if (text == "f")
		{
			string value1;
			string value2;
			string value3;

			file >> value1;
			file >> value2;
			file >> value3;

			vector<string> face_index = split(value1, "/");
			vector<string> uv_index = split(value2, "/");
			vector<string> normal_index = split(value3, "/");

			int face_index1 = std::atoi(face_index[0].c_str());
			int uv_index1 = std::atoi(face_index[1].c_str());
			int normal_index1 = std::atoi(face_index[2].c_str());

			int face_index2 = std::atoi(uv_index[0].c_str());
			int uv_index2 = std::atoi(uv_index[1].c_str());
			int normal_index2 = std::atoi(uv_index[2].c_str());

			int face_index3 = std::atoi(normal_index[0].c_str());
			int uv_index3 = std::atoi(normal_index[1].c_str());
			int normal_index3 = std::atoi(normal_index[2].c_str());

			element_index face_value_struct = { face_index1, face_index2, face_index3 };
			element_index uv_value_struct = { uv_index1, uv_index2, uv_index3 };
			element_index normal_value_struct = { normal_index1, normal_index2, normal_index3 };

			//printf("uv_index1 %d %d %d\n", uv_index1, uv_index2, uv_index3);

			fi.push_back(face_value_struct);
			ui.push_back(uv_value_struct);
			ni.push_back(normal_value_struct);

		}
		

	}


	sequence.push_back({ v, t, fi, ui, ni, fi2, ui2, ni2, fi3, ui3, ni3 });

}


void render_slider(glfw_state& app_state)
{
	static const int flags = ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ 430, 300 });

	ImGui::Begin("slider", nullptr, flags);

	ImGui::Checkbox("normal ver", &normal_bool);
	ImGui::SameLine();
	ImGui::Checkbox("rgb ver", &RGB_bool);
	ImGui::SameLine();
	ImGui::Checkbox("view cap", &capture);
	ImGui::Checkbox("pm", &pm);
	ImGui::SameLine();
	//ImGui::SliderFloat("Masking value", &mask,128,255);

	ImGui::SliderFloat3(" view point ro 1", offset_value1, -3.1, 3.14);
	ImGui::SliderFloat3(" view point tr1", tr_xyz1, -1.0, 1.0);

	//	ImGui::SliderFloat2("  distance", holo_distance, 50.0 cm, 150 cm);
	//	ImGui::SliderFloat2("  z", zvalue, -10, 10);
	//
	//	ImGui::SliderFloat(" SLM pitch x", &SLM_PX, 3.0, 10.4);
	//	ImGui::SliderFloat(" scale y", &scale, -10.0, 10.0);
	//	SLM_PITCH_X = (double)(SLM_PX um);
	//	SLM_PITCH_Y = (double)(SLM_PX um);


	if (ImGui::Button("Reset"))
	{
		// do something
		printf("Reset!!\n");
		tr_xyz1[0] = 0.0f, tr_xyz1[1] = 0.0f, tr_xyz1[2] = 0.0f;
		offset_value1[0] = 0.0f, offset_value1[1] = 0.0f, offset_value1[2] = 0.0f;
		tr_xyz2[0] = 0.0f, tr_xyz2[1] = 0.0f, tr_xyz2[2] = 0.0f;
		offset_value2[0] = 0.0f, offset_value2[1] = 0.0f, offset_value2[2] = 0.0f;
		tr_xyz3[0] = 0.0f, tr_xyz3[1] = 0.0f, tr_xyz3[2] = 0.0f;
		offset_value3[0] = 0.0f, offset_value3[1] = 0.0f, offset_value3[2] = 0.0f;
	}

	if (ImGui::IsAnyItemActive()) {
		app_state.over = 1;
	}
	else {
		app_state.over = 0;
	}

	ImGui::End();
}

void DrawBody(std::vector<Vector3> draw_list, std::vector<std::vector<Motion>> m, int frame)
{
	glPushMatrix();

	//glRotatef(90, 1, 0, 0);
	//root hips_JNT
	glRotatef(m[frame][0].rotation.x, 1, 0, 0);
	glRotatef(m[frame][0].rotation.y, 0, 1, 0);
	glRotatef(m[frame][0].rotation.z, 0, 0, 1);

	glPushMatrix();

	//spine_JNT
	glRotatef(m[frame][1].rotation.x, 1, 0, 0);
	glRotatef(m[frame][1].rotation.y, 0, 1, 0);
	glRotatef(m[frame][1].rotation.z, 0, 0, 1);

	glRotatef(m[frame][2].rotation.x, 1, 0, 0);
	glRotatef(m[frame][2].rotation.y, 0, 1, 0);
	glRotatef(m[frame][2].rotation.z, 0, 0, 1);

	glRotatef(m[frame][3].rotation.x, 1, 0, 0);
	glRotatef(m[frame][3].rotation.y, 0, 1, 0);
	glRotatef(m[frame][3].rotation.z, 0, 0, 1);

	glRotatef(m[frame][4].rotation.x, 1, 0, 0);
	glRotatef(m[frame][4].rotation.y, 0, 1, 0);
	glRotatef(m[frame][4].rotation.z, 0, 0, 1);


	//실제 그리는/생성하는 부분
	glBegin(GL_POINTS);
	for (int i = 0; i < draw_list.size(); i++)
	{
		
		glVertex3f(draw_list[i].x, draw_list[i].y, draw_list[i].z);

	}
	glEnd();

	


	//glPopMatrix();
}

void DrawHead(std::vector<Vector3> draw_list, std::vector<std::vector<Motion>> m, int frame)
{
	glPushMatrix();

	glRotatef(m[frame][5].rotation.x, 1, 0, 0);
	glRotatef(m[frame][5].rotation.y, 0, 1, 0);
	glRotatef(m[frame][5].rotation.z, 0, 0, 1);


	//실제 그리는/생성하는 부분
	glBegin(GL_POINTS);
	for (int i = 0; i < draw_list.size(); i++)
	{

		glVertex3f(draw_list[i].x, draw_list[i].y, draw_list[i].z);

	}
	glEnd();

	glPopMatrix();
}

void DrawL_sh(std::vector<Vector3> draw_list, std::vector<std::vector<Motion>> m, int frame)
{
	glPushMatrix();

	glRotatef(m[frame][6].rotation.x, 1, 0, 0);
	glRotatef(m[frame][6].rotation.y, 0, 1, 0);
	glRotatef(m[frame][6].rotation.z, 0, 0, 1);


	//실제 그리는/생성하는 부분
	glBegin(GL_POINTS);
	for (int i = 0; i < draw_list.size(); i++)
	{

		glVertex3f(draw_list[i].x, draw_list[i].y, draw_list[i].z);

	}
	glEnd();

}

void DrawL_uparm(std::vector<Vector3> draw_list, std::vector<std::vector<Motion>> m, int frame)
{

	glRotatef(m[frame][7].rotation.x, 1, 0, 0);
	glRotatef(m[frame][7].rotation.y, 0, 1, 0);
	glRotatef(m[frame][7].rotation.z, 0, 0, 1);


	//실제 그리는/생성하는 부분
	glBegin(GL_POINTS);
	for (int i = 0; i < draw_list.size(); i++)
	{

		glVertex3f(draw_list[i].x, draw_list[i].y, draw_list[i].z);

	}
	glEnd();

}

void DrawL_lowarm(std::vector<Vector3> draw_list, std::vector<std::vector<Motion>> m, int frame)
{
	glRotatef(m[frame][8].rotation.x, 1, 0, 0);
	glRotatef(m[frame][8].rotation.y, 0, 1, 0);
	glRotatef(m[frame][8].rotation.z, 0, 0, 1);


	//실제 그리는/생성하는 부분
	glBegin(GL_POINTS);
	for (int i = 0; i < draw_list.size(); i++)
	{

		glVertex3f(draw_list[i].x, draw_list[i].y, draw_list[i].z);

	}
	glEnd();

}

void DrawL_hand(std::vector<Vector3> draw_list, std::vector<std::vector<Motion>> m, int frame)
{
	glRotatef(m[frame][9].rotation.x, 1, 0, 0);
	glRotatef(m[frame][9].rotation.y, 0, 1, 0);
	glRotatef(m[frame][9].rotation.z, 0, 0, 1);


	//실제 그리는/생성하는 부분
	glBegin(GL_POINTS);
	for (int i = 0; i < draw_list.size(); i++)
	{

		glVertex3f(draw_list[i].x, draw_list[i].y, draw_list[i].z);

	}
	glEnd();

	glPopMatrix();
}

void DrawR_sh(std::vector<Vector3> draw_list, std::vector<std::vector<Motion>> m, int frame)
{
	glPushMatrix();

	glRotatef(m[frame][6].rotation.x, 1, 0, 0);
	glRotatef(m[frame][6].rotation.y, 0, 1, 0);
	glRotatef(m[frame][6].rotation.z, 0, 0, 1);


	//실제 그리는/생성하는 부분
	glBegin(GL_POINTS);
	for (int i = 0; i < draw_list.size(); i++)
	{

		glVertex3f(draw_list[i].x, draw_list[i].y, draw_list[i].z);

	}
	glEnd();

}

void DrawR_uparm(std::vector<Vector3> draw_list, std::vector<std::vector<Motion>> m, int frame)
{

	glRotatef(m[frame][7].rotation.x, 1, 0, 0);
	glRotatef(m[frame][7].rotation.y, 0, 1, 0);
	glRotatef(m[frame][7].rotation.z, 0, 0, 1);


	//실제 그리는/생성하는 부분
	glBegin(GL_POINTS);
	for (int i = 0; i < draw_list.size(); i++)
	{

		glVertex3f(draw_list[i].x, draw_list[i].y, draw_list[i].z);

	}
	glEnd();

}

void DrawR_lowarm(std::vector<Vector3> draw_list, std::vector<std::vector<Motion>> m, int frame)
{
	glRotatef(m[frame][8].rotation.x, 1, 0, 0);
	glRotatef(m[frame][8].rotation.y, 0, 1, 0);
	glRotatef(m[frame][8].rotation.z, 0, 0, 1);


	//실제 그리는/생성하는 부분
	glBegin(GL_POINTS);
	for (int i = 0; i < draw_list.size(); i++)
	{

		glVertex3f(draw_list[i].x, draw_list[i].y, draw_list[i].z);

	}
	glEnd();

}

void DrawR_hand(std::vector<Vector3> draw_list, std::vector<std::vector<Motion>> m, int frame)
{
	glRotatef(m[frame][9].rotation.x, 1, 0, 0);
	glRotatef(m[frame][9].rotation.y, 0, 1, 0);
	glRotatef(m[frame][9].rotation.z, 0, 0, 1);


	//실제 그리는/생성하는 부분
	glBegin(GL_POINTS);
	for (int i = 0; i < draw_list.size(); i++)
	{

		glVertex3f(draw_list[i].x, draw_list[i].y, draw_list[i].z);

	}
	glEnd();

	glPopMatrix();
}


void render_no_textu(glfw_state app_state, body body_part_draw, skeleton skel)
{
	for (int frame = 0; frame < skel.frames; frame++)
	{
		glColor3f(1, 1, 1);

		//원점 중심이면 translation도 해줘야함 각자 joint 위치로
		DrawBody(body_part_draw.upper_rest_rest_list, skel.m, frame);

		DrawHead(body_part_draw.head_list, skel.m, frame);

		DrawL_sh(body_part_draw.l_shoulder_list, skel.m, frame);
		DrawL_uparm(body_part_draw.l_upper_arm_list, skel.m, frame);
		DrawL_lowarm(body_part_draw.l_lower_arm_list, skel.m, frame);
		DrawL_hand(body_part_draw.l_hand_list, skel.m, frame);

		DrawR_sh(body_part_draw.r_shoulder_list, skel.m, frame);
		DrawR_uparm(body_part_draw.r_upper_arm_list, skel.m, frame);
		DrawR_lowarm(body_part_draw.r_lower_arm_list, skel.m, frame);
		DrawR_hand(body_part_draw.r_hand_list, skel.m, frame);


	}
	
	//DrawNeck();
	//DrawHead();
	//DrawR_arm(r_arm_u, 1, 0, 0);
	//DrawR_hand(r_arm_d, 1, 0, 0);
	//DrawL_arm(l_arm_u, 1, 0, 0);
	//DrawL_hand(l_arm_d, 1, 0, 0);
	//DrawL_leg(l_leg_u, 1, 0, 0);
	//DrawL_foot(l_leg_d, 1, 0, 0);
	//DrawR_leg(r_leg_u, 1, 0, 0);
	//DrawR_foot(r_leg_d, 1, 0, 0);


}

void render(glfw_state app_state, int frame, vector<data_seq> sequence, int type)
{
	glEnable(GL_TEXTURE_2D);
	if (type == 1) {
		glBindTexture(GL_TEXTURE_2D, app_state.tex.get_gl_handle());
	}
	else if (type == 2) {
		glBindTexture(GL_TEXTURE_2D, app_state.tex2.get_gl_handle());
	}
	else if (type == 3)
	{
		glBindTexture(GL_TEXTURE_2D, app_state.tex3.get_gl_handle());

	}
	//glBindTexture(GL_TEXTURE_2D, app_state.tex2.get_gl_handle());
	float tex_border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, tex_border_color);


	//	printf("%d\n", type);
	//	printf("%d\n", fi.size());
	//	printf("%d\n", fi2.size());

		//printf("type %d\n", type);
	if (type == 3)
	{

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < sequence[frame].fi3.size(); i++)
		{
			//texture3
			int a = (int)sequence[frame].fi3[i].x - 1;
			int a2 = (int)sequence[frame].ui3[i].x - 1;
			int a3 = (int)sequence[frame].ni3[i].x - 1;

			int b = (int)sequence[frame].fi3[i].y - 1;
			int b2 = (int)sequence[frame].ui3[i].y - 1;
			int b3 = (int)sequence[frame].ni3[i].y - 1;


			int c = (int)sequence[frame].fi3[i].z - 1;
			int c2 = (int)sequence[frame].ui3[i].z - 1;
			int c3 = (int)sequence[frame].ni3[i].z - 1;

			//printf("%d - %f %f %f\n", a, vertices[a].x, vertices[a].y, vertices[a].z);
			//printf("%d - %f %f %f\n", b, vertices[b].x, vertices[b].y, vertices[b].z);
			//printf("%d - %f %f %f\n", c, vertices[c].x, vertices[c].y, vertices[c].z);

			glTexCoord2f(sequence[frame].t[a2].u, sequence[frame].t[a2].v);
			glVertex3f(sequence[frame].v[a].x, sequence[frame].v[a].y, sequence[frame].v[a].z);	//0대신 0.5f

			glTexCoord2f(sequence[frame].t[b2].u, sequence[frame].t[b2].v);
			glVertex3f(sequence[frame].v[b].x, sequence[frame].v[b].y, sequence[frame].v[b].z);

			glTexCoord2f(sequence[frame].t[c2].u, sequence[frame].t[c2].v);
			glVertex3f(sequence[frame].v[c].x, sequence[frame].v[c].y, sequence[frame].v[c].z);

			//glVertex3f(points[i].x, points[i].y, points[i].z );

			//glColor3ub(rgb[i].x, rgb[i].y, rgb[i].z);
			//printf("%i\n",rgb[i].x);
		}
		glEnd();
	}
	else if (type == 2)
	{

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < sequence[frame].fi2.size(); i++)
		{

			int a = (int)sequence[frame].fi2[i].x - 1;
			int a2 = (int)sequence[frame].ui2[i].x - 1;
			int a3 = (int)sequence[frame].ni2[i].x - 1;

			int b = (int)sequence[frame].fi2[i].y - 1;
			int b2 = (int)sequence[frame].ui2[i].y - 1;
			int b3 = (int)sequence[frame].ni2[i].y - 1;


			int c = (int)sequence[frame].fi2[i].z - 1;
			int c2 = (int)sequence[frame].ui2[i].z - 1;
			int c3 = (int)sequence[frame].ni2[i].z - 1;

			//printf("%d - %f %f %f\n", a, vertices[a].x, vertices[a].y, vertices[a].z);
			//printf("%d - %f %f %f\n", b, vertices[b].x, vertices[b].y, vertices[b].z);
			//printf("%d - %f %f %f\n", c, vertices[c].x, vertices[c].y, vertices[c].z);

			glTexCoord2f(sequence[frame].t[a2].u, sequence[frame].t[a2].v);
			glVertex3f(sequence[frame].v[a].x, sequence[frame].v[a].y, sequence[frame].v[a].z);	//0대신 0.5f

			glTexCoord2f(sequence[frame].t[b2].u, sequence[frame].t[b2].v);
			glVertex3f(sequence[frame].v[b].x, sequence[frame].v[b].y, sequence[frame].v[b].z);

			glTexCoord2f(sequence[frame].t[c2].u, sequence[frame].t[c2].v);
			glVertex3f(sequence[frame].v[c].x, sequence[frame].v[c].y, sequence[frame].v[c].z);

			//glVertex3f(points[i].x, points[i].y, points[i].z );

			//glColor3ub(rgb[i].x, rgb[i].y, rgb[i].z);
			//printf("%i\n",rgb[i].x);
		}
		glEnd();
	}
	else
	{

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < sequence[frame].fi.size(); i++)
		{
			int a = (int)sequence[frame].fi[i].x - 1;
			int a2 = (int)sequence[frame].ui[i].x - 1;
			int a3 = (int)sequence[frame].ni[i].x - 1;

			int b = (int)sequence[frame].fi[i].y - 1;
			int b2 = (int)sequence[frame].ui[i].y - 1;
			int b3 = (int)sequence[frame].ni[i].y - 1;


			int c = (int)sequence[frame].fi[i].z - 1;
			int c2 = (int)sequence[frame].ui[i].z - 1;
			int c3 = (int)sequence[frame].ni[i].z - 1;

			//printf("%d - %f %f %f\n", a, vertices[a].x, vertices[a].y, vertices[a].z);
			//printf("%d - %f %f %f\n", b, vertices[b].x, vertices[b].y, vertices[b].z);
			//printf("%d - %f %f %f\n", c, vertices[c].x, vertices[c].y, vertices[c].z);

			glTexCoord2f(sequence[frame].t[a2].u, sequence[frame].t[a2].v);
			glVertex3f(sequence[frame].v[a].x, sequence[frame].v[a].y, sequence[frame].v[a].z);

			glTexCoord2f(sequence[frame].t[b2].u, sequence[frame].t[b2].v);
			glVertex3f(sequence[frame].v[b].x, sequence[frame].v[b].y, sequence[frame].v[b].z);

			glTexCoord2f(sequence[frame].t[c2].u, sequence[frame].t[c2].v);
			glVertex3f(sequence[frame].v[c].x, sequence[frame].v[c].y, sequence[frame].v[c].z);

			//glVertex3f(points[i].x, points[i].y, points[i].z );

			//glColor3ub(rgb[i].x, rgb[i].y, rgb[i].z);
			//printf("%i\n",rgb[i].x);
		}
		glEnd();
	}

}

void render_part(glfw_state app_state, int frame, vector<data_seq> sequence, int type, int part, vector<int> bb)
{
	glEnable(GL_TEXTURE_2D);
	if (type == 1) {
		glBindTexture(GL_TEXTURE_2D, app_state.tex.get_gl_handle());
	}
	else if (type == 2) {
		glBindTexture(GL_TEXTURE_2D, app_state.tex2.get_gl_handle());
	}
	else if (type == 3)
	{
		glBindTexture(GL_TEXTURE_2D, app_state.tex3.get_gl_handle());

	}
	//glBindTexture(GL_TEXTURE_2D, app_state.tex2.get_gl_handle());
	float tex_border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, tex_border_color);


	//	printf("%d\n", type);
	//	printf("%d\n", fi.size());
	//	printf("%d\n", fi2.size());

		//printf("type %d\n", type);
	if (type == 3)
	{

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < sequence[frame].fi3.size(); i++)
		{
			//texture3
			int a = (int)sequence[frame].fi3[i].x - 1;
			int a2 = (int)sequence[frame].ui3[i].x - 1;
			int a3 = (int)sequence[frame].ni3[i].x - 1;

			int b = (int)sequence[frame].fi3[i].y - 1;
			int b2 = (int)sequence[frame].ui3[i].y - 1;
			int b3 = (int)sequence[frame].ni3[i].y - 1;


			int c = (int)sequence[frame].fi3[i].z - 1;
			int c2 = (int)sequence[frame].ui3[i].z - 1;
			int c3 = (int)sequence[frame].ni3[i].z - 1;
			
			//printf("%d - %f %f %f\n", a, vertices[a].x, vertices[a].y, vertices[a].z);
			//printf("%d - %f %f %f\n", b, vertices[b].x, vertices[b].y, vertices[b].z);
			//printf("%d - %f %f %f\n", c, vertices[c].x, vertices[c].y, vertices[c].z);

			if (bb[a] == part)
			{
				glTexCoord2f(sequence[frame].t[a2].u, sequence[frame].t[a2].v);
				glVertex3f(sequence[frame].v[a].x, sequence[frame].v[a].y, sequence[frame].v[a].z);	//0대신 0.5f

				glTexCoord2f(sequence[frame].t[b2].u, sequence[frame].t[b2].v);
				glVertex3f(sequence[frame].v[b].x, sequence[frame].v[b].y, sequence[frame].v[b].z);

				glTexCoord2f(sequence[frame].t[c2].u, sequence[frame].t[c2].v);
				glVertex3f(sequence[frame].v[c].x, sequence[frame].v[c].y, sequence[frame].v[c].z);
			}
			

		}
		glEnd();
	}
	else if (type == 2)
	{

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < sequence[frame].fi2.size(); i++)
		{

			int a = (int)sequence[frame].fi2[i].x - 1;
			int a2 = (int)sequence[frame].ui2[i].x - 1;
			int a3 = (int)sequence[frame].ni2[i].x - 1;

			int b = (int)sequence[frame].fi2[i].y - 1;
			int b2 = (int)sequence[frame].ui2[i].y - 1;
			int b3 = (int)sequence[frame].ni2[i].y - 1;


			int c = (int)sequence[frame].fi2[i].z - 1;
			int c2 = (int)sequence[frame].ui2[i].z - 1;
			int c3 = (int)sequence[frame].ni2[i].z - 1;

			//printf("%d - %f %f %f\n", a, vertices[a].x, vertices[a].y, vertices[a].z);
			//printf("%d - %f %f %f\n", b, vertices[b].x, vertices[b].y, vertices[b].z);
			//printf("%d - %f %f %f\n", c, vertices[c].x, vertices[c].y, vertices[c].z);

			//v가 원하는 part면 그리기
			if (bb[a] == part)
			{
				glTexCoord2f(sequence[frame].t[a2].u, sequence[frame].t[a2].v);
				glVertex3f(sequence[frame].v[a].x, sequence[frame].v[a].y, sequence[frame].v[a].z);	//0대신 0.5f

				glTexCoord2f(sequence[frame].t[b2].u, sequence[frame].t[b2].v);
				glVertex3f(sequence[frame].v[b].x, sequence[frame].v[b].y, sequence[frame].v[b].z);

				glTexCoord2f(sequence[frame].t[c2].u, sequence[frame].t[c2].v);
				glVertex3f(sequence[frame].v[c].x, sequence[frame].v[c].y, sequence[frame].v[c].z);

			}

			
		}
		glEnd();
	}
	else
	{

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < sequence[frame].fi.size(); i++)
		{
			
			int a = (int)sequence[frame].fi[i].x - 1;
			int a2 = (int)sequence[frame].ui[i].x - 1;
			int a3 = (int)sequence[frame].ni[i].x - 1;

			int b = (int)sequence[frame].fi[i].y - 1;
			int b2 = (int)sequence[frame].ui[i].y - 1;
			int b3 = (int)sequence[frame].ni[i].y - 1;

			int c = (int)sequence[frame].fi[i].z - 1;
			int c2 = (int)sequence[frame].ui[i].z - 1;
			int c3 = (int)sequence[frame].ni[i].z - 1;

			//printf("%d - %f %f %f\n", a, vertices[a].x, vertices[a].y, vertices[a].z);
			//printf("%d - %f %f %f\n", b, vertices[b].x, vertices[b].y, vertices[b].z);
			//printf("%d - %f %f %f\n", c, vertices[c].x, vertices[c].y, vertices[c].z);

			if (bb[a] == part)
			{
				glTexCoord2f(sequence[frame].t[a2].u, sequence[frame].t[a2].v);
				glVertex3f(sequence[frame].v[a].x, sequence[frame].v[a].y, sequence[frame].v[a].z);

				glTexCoord2f(sequence[frame].t[b2].u, sequence[frame].t[b2].v);
				glVertex3f(sequence[frame].v[b].x, sequence[frame].v[b].y, sequence[frame].v[b].z);

				glTexCoord2f(sequence[frame].t[c2].u, sequence[frame].t[c2].v);
				glVertex3f(sequence[frame].v[c].x, sequence[frame].v[c].y, sequence[frame].v[c].z);
			}

			

		}
		glEnd();
	}

}


void Capture_OpenGLViewPort(cv::Mat& copy)
{

	GLubyte* bits; //RGB bits
	GLint viewport[4]; //current viewport
	//get current viewport
	glGetIntegerv(GL_VIEWPORT, viewport);

	int rows = viewport[3] * 1;	//행
	int cols = viewport[2] * 4;	//렬

	bits = new GLubyte[cols * 3 * rows];

	//read pixel from frame buffer
	glFinish(); //finish all commands of OpenGL
	glPixelStorei(GL_PACK_ALIGNMENT, 1); //or glPixelStorei(GL_PACK_ALIGNMENT,4);	//왜 1일까 0부터가 아니고...
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	glReadPixels(0, 0, cols, rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, bits);

	for (int i = 0; i < copy.rows; i++)
	{
		for (int j = 0; j < copy.cols; j++)
		{
			copy.at<cv::Vec3b>(i, j)[0] = (uchar)(bits[(rows - i - 1) * 3 * cols + j * 3 + 0]);
			copy.at<cv::Vec3b>(i, j)[1] = (uchar)(bits[(rows - i - 1) * 3 * cols + j * 3 + 1]);
			copy.at<cv::Vec3b>(i, j)[2] = (uchar)(bits[(rows - i - 1) * 3 * cols + j * 3 + 2]);
		}
	}

	delete[] bits;

}

void draw_with_cam_no_textu(glfw_state& app_state, body& body_part_draw, skeleton& skel)
{
	minmax_3D upper_body_minmax;
	minmax_3Dvalue(body_part_draw.upper_list, upper_body_minmax);

	float center_x = (upper_body_minmax.max_3D_x + upper_body_minmax.min_3D_x)/2;
	float center_y = (upper_body_minmax.max_3D_y + upper_body_minmax.min_3D_y)/2;
	float center_z = (upper_body_minmax.max_3D_z + upper_body_minmax.min_3D_z)/2;

	int obj_w = 1000;
	int obj_h = 1000;

	//함수 밖에서 아래 값들 넣어주기
	// sj_t

	int x_up = 1;
	int y_up = 0;
	int z_up = 0;

	float clip_value = 3.3;	//minmax값 사용하기

	glViewport(0, 0, obj_w, obj_h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-clip_value, clip_value, -clip_value, clip_value, -clip_value, clip_value);	//실제 사이즈 2미만
	//glOrtho(center_z - clip_value, center_z + clip_value, center_x - clip_value, center_x + clip_value, center_y - clip_value, center_y + clip_value);	//실제 사이즈 2미만
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glClear(GL_DEPTH_BUFFER_BIT);

	//	glTranslatef(0, 0, +0.5f + app_state.offset_y * 0.05f);	//확인해보기
	glTranslatef(0, 0, app_state.offset_y);	//확인해보기
	//glTranslatef(-1.5f, 0, 0);
	glRotated(app_state.pitch, 1, 0, 0);
	glRotated(app_state.yaw, 0, 1, 0);
	glPointSize(1.9f);
	glEnable(GL_DEPTH_TEST);


	gluLookAt(center_x, center_y + 0.5, center_z + 0.5, center_x, center_y, center_z, x_up, y_up, z_up);
	//gluLookAt(center_x, center_y + distance, center_z, center_x, center_y, center_z, x_up, y_up, z_up);

	render_no_textu(app_state, body_part_draw, skel);
}

void draw_with_cam(int cam, int direction, glfw_state& app_state, int frame, std::vector<data_seq>& sequence, vector<int> texture_num)
{
	int obj_w = 1000;
	int obj_h = 1000;

	//함수 밖에서 아래 값들 넣어주기
	// sj_t
	float size_x = 5.5912;
	float size_y = 1.2624;
	float size_z = 5.2063;

	float center_x = -2.7956;
	float center_y = -1.9064;
	float center_z = 3.8493;
	
	
	//salute 변경해야함
	//float size_x = 5.5912;
	//float size_y = 1.2624;
	//float size_z = 5.2063;
	//
	//float center_x = -2.7956;
	//float center_y = -1.9064;
	//float center_z = 3.8493;

	//direction 4260
	//float size_x = 5.5933;
	//float size_y = 1.3589;
	//float size_z = 2.6880;
	//
	//float center_x = -2.7967;
	//float center_y = -1.9343;
	//float center_z = 4.4032;

	//sol_salute
	//float size_x = 5.2437;
	//float size_y = 1.1824;
	//float size_z = 1.8799;
	//
	//float center_x = -2.6219;
	//float center_y = -2.2148;
	//float center_z = 3.7956;

	float x_2D_min = center_x - (size_x / 2);
	float x_2D_max = center_x + (size_x / 2);
	float y_2D_min = center_y - (size_y / 2);
	float y_2D_max = center_y + (size_y / 2);
	float z_2D_min = center_z - (size_z / 2);
	float z_2D_max = center_z + (size_z / 2);

	int x_up = -1;
	int y_up = 0;
	int z_up = 0;

	int distance = 3;	//r 반지름

	float clip_value = 3.3;


	glViewport(obj_w * direction, 0, obj_w, obj_h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-clip_value, clip_value, -clip_value, clip_value, -clip_value, clip_value);	//실제 사이즈 2미만
	//glOrtho(center_z - clip_value, center_z + clip_value, center_x - clip_value, center_x + clip_value, center_y - clip_value, center_y + clip_value);	//실제 사이즈 2미만
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glClear(GL_DEPTH_BUFFER_BIT);

	//	glTranslatef(0, 0, +0.5f + app_state.offset_y * 0.05f);	//확인해보기
	glTranslatef(0, 0, app_state.offset_y);	//확인해보기
	//glTranslatef(-1.5f, 0, 0);
	glRotated(app_state.pitch, 1, 0, 0);
	glRotated(app_state.yaw, 0, 1, 0);
	glPointSize(1.9f);
	glEnable(GL_DEPTH_TEST);


	float degree_interval = 360 / cam;	//cam;카메라 수
	float degree = direction * degree_interval;
	float radian = degree * PI / 180;

	float cam_y = distance * cos(radian);
	float cam_z = distance * sin(radian);

	gluLookAt(center_x, center_y + cam_y, center_z + cam_z, center_x, center_y, center_z, x_up, y_up, z_up);
	//gluLookAt(center_x, center_y + distance, center_z, center_x, center_y, center_z, x_up, y_up, z_up);

	if (texture_num[frame] == 3)
	{

		render(app_state, frame, sequence, 3);
		render(app_state, frame, sequence, 2);
	}
	else if (texture_num[frame] == 2)
	{
		render(app_state, frame, sequence, 2);
	}
	render(app_state, frame, sequence, 1);


}

void draw_with_cam_part(int cam, int direction, glfw_state& app_state, int frame, std::vector<data_seq>& sequence, vector<int> texture_num, int part, vector<int> bb)
{
	int obj_w = 1000;
	int obj_h = 1000;

	//함수 밖에서 아래 값들 넣어주기
	// sj_t
	float size_x = 5.5912;
	float size_y = 1.2624;
	float size_z = 5.2063;

	float center_x = -2.7956;
	float center_y = -1.9064;
	float center_z = 3.8493;


	//salute 변경해야함
	//float size_x = 5.5912;
	//float size_y = 1.2624;
	//float size_z = 5.2063;
	//
	//float center_x = -2.7956;
	//float center_y = -1.9064;
	//float center_z = 3.8493;

	//direction 4260
	//float size_x = 5.5933;
	//float size_y = 1.3589;
	//float size_z = 2.6880;
	//
	//float center_x = -2.7967;
	//float center_y = -1.9343;
	//float center_z = 4.4032;

	//sol_salute
	//float size_x = 5.2437;
	//float size_y = 1.1824;
	//float size_z = 1.8799;
	//
	//float center_x = -2.6219;
	//float center_y = -2.2148;
	//float center_z = 3.7956;

	float x_2D_min = center_x - (size_x / 2);
	float x_2D_max = center_x + (size_x / 2);
	float y_2D_min = center_y - (size_y / 2);
	float y_2D_max = center_y + (size_y / 2);
	float z_2D_min = center_z - (size_z / 2);
	float z_2D_max = center_z + (size_z / 2);

	int x_up = -1;
	int y_up = 0;
	int z_up = 0;

	int distance = 3;	//r 반지름

	float clip_value = 3.3;


	glViewport(obj_w * direction, 0, obj_w, obj_h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-clip_value, clip_value, -clip_value, clip_value, -clip_value, clip_value);	//실제 사이즈 2미만
	//glOrtho(center_z - clip_value, center_z + clip_value, center_x - clip_value, center_x + clip_value, center_y - clip_value, center_y + clip_value);	//실제 사이즈 2미만
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glClear(GL_DEPTH_BUFFER_BIT);

	//	glTranslatef(0, 0, +0.5f + app_state.offset_y * 0.05f);	//확인해보기
	glTranslatef(0, 0, app_state.offset_y);	//확인해보기
	//glTranslatef(-1.5f, 0, 0);
	glRotated(app_state.pitch, 1, 0, 0);
	glRotated(app_state.yaw, 0, 1, 0);
	glPointSize(1.9f);
	glEnable(GL_DEPTH_TEST);


	float degree_interval = 360 / cam;	//cam;카메라 수
	float degree = direction * degree_interval;
	float radian = degree * PI / 180;

	float cam_y = distance * cos(radian);
	float cam_z = distance * sin(radian);

	gluLookAt(center_x, center_y + cam_y, center_z + cam_z, center_x, center_y, center_z, x_up, y_up, z_up);
	//gluLookAt(center_x, center_y + distance, center_z, center_x, center_y, center_z, x_up, y_up, z_up);

	if (texture_num[frame] == 3)
	{

		render_part(app_state, frame, sequence, 3, part, bb);
		render_part(app_state, frame, sequence, 2, part, bb);
	}
	else if (texture_num[frame] == 2)
	{
		render_part(app_state, frame, sequence, 2, part, bb);
	}
	render_part(app_state, frame, sequence, 1, part, bb);


}


void project_viewer(body& body_part_draw, skeleton& skel)
{
	int w_w = 1000;
	int w_h = 1000;

	//glfw 초기화
	//glfwInit();
	//GLFWwindow* window = glfwCreateWindow(w_w, w_h, "OBJ", 0, 0);
	//glfwMakeContextCurrent(window);

	window app(w_w, w_h, "3D Mesh");
	//	window app(OBJ_WIDTH + 430, OBJ_HEIGHT, "3D Mesh");		////+430 부분에 슬라이드 바 있음
	ImGui_ImplGlfw_Init(app, false);

	glfw_state app_state;

	register_glfw_callbacks(app, app_state);

	int frame = 0;
	printf("frame %d\n", frame);
	while (app)
	{
		int cam = 4;	//카메라 개수

		for (int direction = 0; direction < cam; direction++)
		{
			draw_with_cam_no_textu(app_state, body_part_draw, skel);
		}

		//cv::Mat rendering_capture(obj_h, obj_w * 4, CV_8UC3);
		//Capture_OpenGLViewPort(rendering_capture);

		//char imwrite_path[256];
		//for (int direction = 0; direction < cam; direction++)
		//{
		//	cv::Mat projection_image0(obj_w, obj_h, CV_8UC3, cv::Scalar(0));
		//	projection_image0 = rendering_capture(cv::Rect(obj_w * direction, 0, obj_w, obj_h));				//오른쪽에서 본
		//
		//	sprintf(imwrite_path, "C:/02_data/05_skeleton/%s/input/%d_img%d.png", cha, frame, direction);
		//	imwrite(imwrite_path, projection_image0);
		//
		//}


		//	ImGui_ImplGlfw_NewFrame(1);
		//	render_slider(app_state);
		//	ImGui::Render();

		frame++;			//한 frame만 가지고 실험하는 거라 주석처리함
		//glfwPollEvents();

		//if (frame == data_num)	//99
		//{
		//	frame = 0;
		//	break;
		//}


		cv::Mat projection_m;
		cv::Mat model_m;

		GLfloat model[16], projection[16];
		glGetFloatv(GL_PROJECTION_MATRIX, projection);
		glGetFloatv(GL_MODELVIEW_MATRIX, model);

		//	for (int i = 0; i < 4; i++)
		//	{
		//		for (int j = 0; j < 4; j++)
		//		{
		//			projection_m.at<float>(j, i) = (float)projection[i * 4 + j];
		//			model_m.at<float>(j, i) = (float)model[i * 4 + j];
		//		}
		//	}

			// OpenGL cleanup
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopAttrib();
		glFlush();
		//glDeleteTextures(2, 0);
		//glDeleteTextures(2, 0);

		//_sleep(20);
		//Sleep(20);
	}

	glfwDestroyWindow(app);
	glfwTerminate();


}

void OBJViewer(int data_num, vector<data_seq>& sequence, vector<texture_image> tex_list, vector<int> texture_num, char cha[256])
{
	int obj_w = 1000;
	int obj_h = 1000;

	int w_w = obj_w * 4;
	int w_h = obj_h;

	//glfw 초기화
	//glfwInit();
	//GLFWwindow* window = glfwCreateWindow(w_w, w_h, "OBJ", 0, 0);
	//glfwMakeContextCurrent(window);

	window app(w_w, w_h, "3D Mesh");
	//	window app(OBJ_WIDTH + 430, OBJ_HEIGHT, "3D Mesh");		////+430 부분에 슬라이드 바 있음
	ImGui_ImplGlfw_Init(app, false);

	glfw_state app_state;

	register_glfw_callbacks(app, app_state);

	glClearColor(0.7, 0.7, 0.7, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	int frame = 0;

	//std::vector<cv::Mat> image_list;

	//cv::imshow("test",texture_02);
	//cv::waitKey(0);

	//printf("tex size %d\n", tex_list.size());

	while (app)
	{
		if (texture_num[frame] > 2)
		{
			printf("texture_count = %d !!!!\n", texture_num[frame]);
			//Sleep(500);
		}

		//printf("tex count %d\n", texture_num[frame]);

		app_state.tex.upload_mat(tex_list[frame].tex1.clone());

		if (texture_num[frame] == 2)
		{
			app_state.tex2.upload_mat2(tex_list[frame].tex2.clone());
		}
		else if (texture_num[frame] == 3)
		{
			app_state.tex2.upload_mat2(tex_list[frame].tex2.clone());
			app_state.tex3.upload_mat3(tex_list[frame].tex3.clone());
		}

		//함수 밖에서 아래 값들 넣어주기
		//salute
		//float size_x = 5.5912;
		//float size_y = 1.2624;
		//float size_z = 5.2063;
		//
		//float center_x = -2.7956;
		//float center_y = -1.9064;
		//float center_z = 3.8493;

		//direction 4260
		//float size_x = 5.5933;
		//float size_y = 1.3589;
		//float size_z = 2.6880;
		//
		//float center_x = -2.7967;
		//float center_y = -1.9343;
		//float center_z = 4.4032;

		//sol_dir
		float size_x = 5.5541;
		float size_y = 1.7495;
		float size_z = 1.9050;

		float center_x = -2.7770;
		float center_y = -1.9651;
		float center_z = 3.7965;

		float x_2D_min = center_x - (size_x / 2);
		float x_2D_max = center_x + (size_x / 2);
		float y_2D_min = center_y - (size_y / 2);
		float y_2D_max = center_y + (size_y / 2);
		float z_2D_min = center_z - (size_z / 2);
		float z_2D_max = center_z + (size_z / 2);

		printf("frame %d\n", frame);

		int cam = 4;	//카메라 개수

		for (int direction = 0; direction < cam; direction++)
		{
			draw_with_cam(cam, direction, app_state, frame, sequence, texture_num);
		}

		cv::Mat rendering_capture(obj_h, obj_w * 4, CV_8UC3);
		Capture_OpenGLViewPort(rendering_capture);

		char imwrite_path[256];
		for (int direction = 0; direction < cam; direction++)
		{
			cv::Mat projection_image0(obj_w, obj_h, CV_8UC3, cv::Scalar(0));
			projection_image0 = rendering_capture(cv::Rect(obj_w * direction, 0, obj_w, obj_h));				//오른쪽에서 본

			sprintf(imwrite_path, "C:/02_data/05_skeleton/%s/input/%d_img%d.png", cha, frame, direction);
			imwrite(imwrite_path, projection_image0);

		}


		//	ImGui_ImplGlfw_NewFrame(1);
		//	render_slider(app_state);
		//	ImGui::Render();

		frame++;			//한 frame만 가지고 실험하는 거라 주석처리함
		//glfwPollEvents();

		if (frame == data_num)	//99
		{
			frame = 0;
			break;
		}


		cv::Mat projection_m;
		cv::Mat model_m;

		GLfloat model[16], projection[16];
		glGetFloatv(GL_PROJECTION_MATRIX, projection);
		glGetFloatv(GL_MODELVIEW_MATRIX, model);

		//	for (int i = 0; i < 4; i++)
		//	{
		//		for (int j = 0; j < 4; j++)
		//		{
		//			projection_m.at<float>(j, i) = (float)projection[i * 4 + j];
		//			model_m.at<float>(j, i) = (float)model[i * 4 + j];
		//		}
		//	}

			// OpenGL cleanup
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopAttrib();
		glFlush();
		//glDeleteTextures(2, 0);
		//glDeleteTextures(2, 0);

		//_sleep(20);
		//Sleep(20);
	}

	glfwDestroyWindow(app);
	glfwTerminate();

}

void OBJViewer_part(int data_num, vector<data_seq>& sequence, vector<texture_image> tex_list, vector<int> texture_num, char cha[256],int part, vector<int> bb)
{
	int obj_w = 1000;
	int obj_h = 1000;

	int w_w = obj_w * 4;
	int w_h = obj_h;

	//glfw 초기화
	//glfwInit();
	//GLFWwindow* window = glfwCreateWindow(w_w, w_h, "OBJ", 0, 0);
	//glfwMakeContextCurrent(window);

	window app(w_w, w_h, "3D Mesh");
	//	window app(OBJ_WIDTH + 430, OBJ_HEIGHT, "3D Mesh");		////+430 부분에 슬라이드 바 있음
	ImGui_ImplGlfw_Init(app, false);

	glfw_state app_state;

	register_glfw_callbacks(app, app_state);

	//glClearColor(0.7, 0.7, 0.7, 1);	//grey
	glClearColor(0, 0, 1, 1);		//blue
	glClear(GL_COLOR_BUFFER_BIT);

	int frame = 0;

	//std::vector<cv::Mat> image_list;

	//cv::imshow("test",texture_02);
	//cv::waitKey(0);

	//printf("tex size %d\n", tex_list.size());

	while (app)
	{
		if (texture_num[frame] > 2)
		{
			printf("texture_count = %d !!!!\n", texture_num[frame]);
			//Sleep(500);
		}

		//printf("tex count %d\n", texture_num[frame]);

		app_state.tex.upload_mat(tex_list[frame].tex1.clone());

		if (texture_num[frame] == 2)
		{
			app_state.tex2.upload_mat2(tex_list[frame].tex2.clone());
		}
		else if (texture_num[frame] == 3)
		{
			app_state.tex2.upload_mat2(tex_list[frame].tex2.clone());
			app_state.tex3.upload_mat3(tex_list[frame].tex3.clone());
		}

		//함수 밖에서 아래 값들 넣어주기
		//salute
		//float size_x = 5.5912;
		//float size_y = 1.2624;
		//float size_z = 5.2063;
		//
		//float center_x = -2.7956;
		//float center_y = -1.9064;
		//float center_z = 3.8493;

		//direction 4260
		//float size_x = 5.5933;
		//float size_y = 1.3589;
		//float size_z = 2.6880;
		//
		//float center_x = -2.7967;
		//float center_y = -1.9343;
		//float center_z = 4.4032;

		//sol_dir
		float size_x = 5.5541;
		float size_y = 1.7495;
		float size_z = 1.9050;

		float center_x = -2.7770;
		float center_y = -1.9651;
		float center_z = 3.7965;

		float x_2D_min = center_x - (size_x / 2);
		float x_2D_max = center_x + (size_x / 2);
		float y_2D_min = center_y - (size_y / 2);
		float y_2D_max = center_y + (size_y / 2);
		float z_2D_min = center_z - (size_z / 2);
		float z_2D_max = center_z + (size_z / 2);

		printf("frame %d\n", frame);

		int cam = 4;	//카메라 개수

		for (int direction = 0; direction < cam; direction++)
		{
			draw_with_cam_part(cam, direction, app_state, frame, sequence, texture_num, part, bb);
		}

		cv::Mat rendering_capture(obj_h, obj_w * 4, CV_8UC3);
		Capture_OpenGLViewPort(rendering_capture);

		char imwrite_path[256];
		for (int direction = 0; direction < cam; direction++)
		{
			cv::Mat projection_image0(obj_w, obj_h, CV_8UC3, cv::Scalar(0));
			projection_image0 = rendering_capture(cv::Rect(obj_w * direction, 0, obj_w, obj_h));				//오른쪽에서 본

			sprintf(imwrite_path, "C:/02_data/05_skeleton/%s/part/%d_img%d.png", cha, frame, direction);
			imwrite(imwrite_path, projection_image0);

		}


		//	ImGui_ImplGlfw_NewFrame(1);
		//	render_slider(app_state);
		//	ImGui::Render();

		frame++;			//한 frame만 가지고 실험하는 거라 주석처리함
		//glfwPollEvents();

		if (frame == data_num)	//99
		{
			frame = 0;
			break;
		}


		cv::Mat projection_m;
		cv::Mat model_m;

		GLfloat model[16], projection[16];
		glGetFloatv(GL_PROJECTION_MATRIX, projection);
		glGetFloatv(GL_MODELVIEW_MATRIX, model);

		//	for (int i = 0; i < 4; i++)
		//	{
		//		for (int j = 0; j < 4; j++)
		//		{
		//			projection_m.at<float>(j, i) = (float)projection[i * 4 + j];
		//			model_m.at<float>(j, i) = (float)model[i * 4 + j];
		//		}
		//	}

			// OpenGL cleanup
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopAttrib();
		glFlush();
		//glDeleteTextures(2, 0);
		//glDeleteTextures(2, 0);

		//_sleep(20);
		//Sleep(20);
	}

	glfwDestroyWindow(app);
	glfwTerminate();

}


// Registers the state variable and callbacks to allow mouse control of the pointcloud
void register_glfw_callbacks(window& app, glfw_state& app_state)
{
	app.on_left_mouse = [&](bool pressed)
	{
		app_state.ml = pressed;
	};


	app.on_mouse_scroll = [&](double xoffset, double yoffset)
	{
		if (app_state.over == 0) {
			app_state.offset_x -= static_cast<float>(xoffset);
			app_state.offset_y -= static_cast<float>(yoffset);
		}
	};

	app.on_mouse_move = [&](double x, double y)
	{
		if (app_state.over == 0) {
			if (app_state.ml)
			{
				app_state.yaw -= (x - app_state.last_x);
				app_state.yaw = std::max(app_state.yaw, -180.0);
				app_state.yaw = std::min(app_state.yaw, +180.0);
				app_state.pitch += (y - app_state.last_y);
				app_state.pitch = std::max(app_state.pitch, -180.0);
				app_state.pitch = std::min(app_state.pitch, +180.0);
			}
			app_state.last_x = x;
			app_state.last_y = y;
		}
	};

	app.on_key_release = [&](int key)
	{
		if (key == 32) // Escape
		{
			app_state.yaw = app_state.pitch = 0; app_state.offset_x = app_state.offset_y = 0.0;
		}
	};
}