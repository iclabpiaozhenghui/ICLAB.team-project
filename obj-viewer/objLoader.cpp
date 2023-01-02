#include <stdio.h>
#include <fstream>	//ifstream file
#include <iostream>	//cout
#include <string>	//getline()
#include <vector>	//vector

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
void Capture_OpenGLViewPort(cv::Mat& copy)
{

	GLubyte* bits; //RGB bits
	GLint viewport[4]; //current viewport
	//get current viewport
	glGetIntegerv(GL_VIEWPORT, viewport);

	int rows = viewport[3] * 1;   //행
	int cols = viewport[2] * 1;   //렬

	bits = new GLubyte[cols * 3 * rows];

	//read pixel from frame buffer
	glFinish(); //finish all commands of OpenGL
	glPixelStorei(GL_PACK_ALIGNMENT, 1); //or glPixelStorei(GL_PACK_ALIGNMENT,4);   //왜 1일까 0부터가 아니고...
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
void OBJLoader(string path, int i, vector<data_seq>& sequence, vector<int>& texture_num)
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

		if (temp_count == 1){
			if (text == "f")
			{
				string value1;
				string value2;
				string value3;

				file >> value1; // 변수 업데이트 입출력 연산자
				file >> value2;
				file >> value3;

				vector<string> face_index = split(value1, "/");
				vector<string> uv_index = split(value2, "/");
				vector<string> normal_index = split(value3, "/");

				int face_index1 = std::atoi(face_index[0].c_str()); // C++의 Atoi는 문자열 값을 정수 값으로 변환하는 데 사용되는 cstdlib 헤더 파일의 미리 정의된 함수입니다.
				int uv_index1 = std::atoi(face_index[1].c_str()); // c_str() 개체의 현재 값을 나타내는 null로 끝나는 문자 시퀀스를 포함하는 배열에 대한 포인터를 반환하는 C++의 내장 함수입니다.
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

		if (temp_count == 2){
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
	}


	sequence.push_back({ v, t, fi, ui, ni, fi2, ui2, ni2 });

	texture_num.push_back(temp_count);
}


void render_slider(glfw_state& app_state) // GLFW는 데스크탑에서 OpenGL, OpenGL ES 및 Vulkan 개발을 위한 오픈 소스, 다중 플랫폼 라이브러리입니다. 창, 컨텍스트 및 표면을 만들고 입력 및 이벤트를 수신하기 위한 간단한 API를 제공합니다. GLFW는 C로 작성되었으며 Windows, macOS, X11 및 Wayland를 지원합니다. GLFW는 zlib/libpng 라이선스에 따라 라이선스가 부여됩니다.
{
	//“static const” is basically a combination of static(a storage specifier) and const(a type qualifier).
	//ImGui는 빠른 반복을 가능하게 하고 프로그래머가 콘텐츠 생성 도구 및 시각화/디버그 도구를 만들 수 있도록 설계되었습니다(일반 최종 사용자를 위한 UI(사용자 인터페이스(UI)는 장치에서 인간-컴퓨터 상호 작용 및 통신의 지점입니다.)와 반대).
	static const int flags = ImGuiWindowFlags_NoCollapse  // Disable user collapsing window by double-clicking on it
		| ImGuiWindowFlags_NoSavedSettings // Never load/save settings in .ini file(ini 파일은 고려되는 항목이 이름=값 공식을 사용하는 문자열 목록을 저장하는 데 사용되는 파일입니다.)
		| ImGuiWindowFlags_NoTitleBar // Disable title-bar
		| ImGuiWindowFlags_NoResize; // Disable user resizing with the lower-right grip

	ImGui::SetNextWindowPos({ 0, 0 }); // set next window position. call before Begin()
	ImGui::SetNextWindowSize({ 430, 300 }); // set next window size. set axis to 0.0f to force an auto-fit on this axis. call before Begin()

	ImGui::Begin("slider", nullptr, flags); // push window to the stack and start appending to it. see .cpp for details. return false when window is collapsed, so you can early out in your code. 'bool* p_open' creates a widget on the upper-right to close the window (which sets your bool to false).
	// nullptr 키워드는 null 포인터 값을 나타냅니다. null 포인터 값을 사용하여 개체 핸들, 내부 포인터 또는 기본 포인터 유형이 개체를 가리키지 않음을 나타냅니다.
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


void render(glfw_state app_state, int frame, vector<data_seq> sequence, int type)
{

	

	glEnable(GL_TEXTURE_2D);
	if(type == 1){
		glBindTexture(GL_TEXTURE_2D, app_state.tex.get_gl_handle());
	}
	else {
		glBindTexture(GL_TEXTURE_2D, app_state.tex2.get_gl_handle());
	}
	//glBindTexture(GL_TEXTURE_2D, app_state.tex2.get_gl_handle());
	float tex_border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, tex_border_color);
	
//	printf("%d\n", type);
//	printf("%d\n", fi.size());
//	printf("%d\n", fi2.size());
	
	printf("type %d\n", type);

	if (type == 2)
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
			glVertex3f(sequence[frame].v[a].x, sequence[frame].v[a].y + 0.5f, sequence[frame].v[a].z - 0.5f);

			glTexCoord2f(sequence[frame].t[b2].u, sequence[frame].t[b2].v);
			glVertex3f(sequence[frame].v[b].x, sequence[frame].v[b].y + 0.5f, sequence[frame].v[b].z - 0.5f);

			glTexCoord2f(sequence[frame].t[c2].u, sequence[frame].t[c2].v);
			glVertex3f(sequence[frame].v[c].x, sequence[frame].v[c].y + 0.5f, sequence[frame].v[c].z - 0.5f);

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
			glVertex3f(sequence[frame].v[a].x, sequence[frame].v[a].y + 0.5f, sequence[frame].v[a].z - 0.5f);

			glTexCoord2f(sequence[frame].t[b2].u, sequence[frame].t[b2].v);
			glVertex3f(sequence[frame].v[b].x, sequence[frame].v[b].y + 0.5f, sequence[frame].v[b].z - 0.5f);

			glTexCoord2f(sequence[frame].t[c2].u, sequence[frame].t[c2].v);
			glVertex3f(sequence[frame].v[c].x, sequence[frame].v[c].y + 0.5f, sequence[frame].v[c].z - 0.5f);

			//glVertex3f(points[i].x, points[i].y, points[i].z );

			//glColor3ub(rgb[i].x, rgb[i].y, rgb[i].z);
			//printf("%i\n",rgb[i].x);
		}
		glEnd();
	}
	
}


void OBJViewer(int data_num, vector<data_seq>& sequence, vector<texture_image> tex_list, vector<int> texture_num)
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

	glfw_state app_state;// Struct for managing rotation of pointcloud view

	register_glfw_callbacks(app, app_state); //포인트 클라우드를 마우스로 제어할 수 있도록 상태 변수와 콜백을 등록합니다.

	//printf("%s\n", tex_path);
	//printf("%s\n", tex_path2);
	int frame = 0;

	//std::vector<cv::Mat> image_list;

	//cv::imshow("test",texture_02);
	//cv::waitKey(0);

	printf("tex size %d\n", tex_list.size());

	while (app)
	{
		if (texture_num[frame] > 2)
		{
			printf("texure_count = %d !!!!\n", texture_num[frame]);
			Sleep(500);//?
		}
	
	//	printf("tex count %d\n", texture_num[frame]);

		app_state.tex.upload_mat(tex_list[frame].tex1.clone());

		if (texture_num[frame] == 2)
		{
			app_state.tex2.upload_mat2(tex_list[frame].tex2.clone());
		}

		int w_w = 1000;
		int w_h = 1000;

		glViewport(0, 0, w_w, w_h);

		glLoadIdentity(); // 현재 행렬을 단위 행렬로 바꿔준다.
		glPushAttrib(GL_ALL_ATTRIB_BITS); // push and pop the server attribute stack, The special mask can be used to save all stackable states.

		//	glClearColor(0.0, 0.0, 1.0, 1.0);
		//	glClear(GL_COLOR_BUFFER_BIT);

		glClear(GL_DEPTH_BUFFER_BIT); // clear buffers to preset values, 지울 버퍼를 나타내는 마스크의 비트는 OR이다.

		glMatrixMode(GL_PROJECTION);// 현재 행렬이 어떤 행렬인지 지정, 투영 행렬 스택에 후속 행렬 연산을 적용한다.
		glPushMatrix();// Set current matrix on the stack



		float clip_value = 5;
		glOrtho(-clip_value, clip_value, -clip_value, clip_value, 0.01f, 10.0f);	//실제 사이즈 2미만

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); //221103

		//함수 밖에서 아래 값들 넣어주기
		float size_x = 5.5904;
		float size_y = 1.2615;
		float size_z = 5.2045;

		float center_x = -2.7952;
		float center_y = -1.9063;
		float center_z = 3.8491;

		int x_up = -1;
		int y_up = 0;
		int z_up = 0;

		int distance = 5;

		gluLookAt(center_x, center_y + distance, center_z, center_x, center_y, center_z, x_up, y_up, z_up);

		//draw_with_cam(cam, 0,)
		int obj_w = 1000;
		int obj_h = 1000;



		char imwrite_path[256];

		//cv::Mat img(obj_w, obj_h, CV_8UC1)
		
		//cv::Mat projection_image;
		//cv::cvtColor(projection_image, projection_image, cv::COLOR_BGR2GRAY);
		//sprintf(imwrite_path, "C:/01_data/05_skeleton.png");
		//cv::imwrite(imwrite_path, projection_image);

		glTranslatef(0, 0, +0.5f + app_state.offset_y * 0.05f);
		//glTranslatef(-1.5f, 0, 0);
		glRotated(app_state.pitch, 1, 0, 0);
		glRotated(app_state.yaw, 0, 1, 0);

		glPointSize(1.9f); //The glPointSize function specifies the diameter of rasterized point
		glEnable(GL_DEPTH_TEST); //opengl 기능 활성화

		printf("frame %d\n", frame);

		if (texture_num[frame] == 2)
		{
			render(app_state, frame, sequence, 2);

		}
		render(app_state, frame, sequence, 1);

		//깜빡임 방지
		//glfwSwapBuffers(app);

		//for (int frame = 0; frame < 32; frame++)
	
		cv::Mat rendering_capture(obj_h, obj_w, CV_8UC3);
		Capture_OpenGLViewPort(rendering_capture);

			//cv::Mat projection_image(obj_w, obj_h, CV_8UC1, cv::Scalar(0));
		rendering_capture = rendering_capture(cv::Rect(0, 0, obj_w, obj_h));
		sprintf(imwrite_path, "C:/01_data/05_skeleton/%d_img.png", frame);
		cv::imwrite(imwrite_path, rendering_capture);
	



		ImGui_ImplGlfw_NewFrame(1);
		render_slider(app_state);
		ImGui::Render();

		//glfwPollEvents();
				

		frame++;


		if (frame == data_num)	//99
		{
			break;
			frame = 0;
		}


		cv::Mat projection_m;
		cv::Mat model_m;

		GLfloat model[16], projection[16];
		glGetFloatv(GL_PROJECTION_MATRIX, projection); // 선택한 매개변수의 값을 반환
		glGetFloatv(GL_MODELVIEW_MATRIX, model);

		//	for (int i = 0; i < 4; i++)
		//	{
		//		for (int j = 0; j < 4; j++)
		//		{
		//			projection_m.at<float>(j, i) = (float)projection[i * 4 + j];
		//			model_m.at<float>(j, i) = (float)model[i * 4 + j];
		//		}
		//	}

		75321;
			// OpenGL cleanup
		glPopMatrix();
		glMatrixMode(GL_PROJECTION); //현재 행렬인 행렬을 지정
		glPopMatrix();
		glPopAttrib();
		glFlush(); //제한된 시간에 opengl함수를 강제로 실행
		//glDeleteTextures(2, 0);
		//glDeleteTextures(2, 0);

		Sleep(20);
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
