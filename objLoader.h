#pragma once
//#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <map>
#include <thread>
#include <chrono>
#include <cstring>
#include <iterator>
#include <queue>
#include <atomic>
#include <functional>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>		//다운받아서 물려줬음 근데 이 방법이 최선?!?
#include <freeglut_std.h>
#include <imgui.h>
#include <imgui_impl_glfw.h> 

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp> 
#include <opencv2/imgproc/imgproc.hpp> 

using namespace std;

struct Vector3 {
	float x;
	float y;
	float z;
};

struct TexCoord {
	float u;
	float v;
};

struct element_index {
	uint x;
	uint y;
	uint z;
};

struct data_seq {
	vector<Vector3> v;
	vector<TexCoord> t;

	vector<element_index> fi;
	vector<element_index> ui;
	vector<element_index> ni;

	vector<element_index> fi2;
	vector<element_index> ui2;
	vector<element_index> ni2;
};


struct texture_image {
	cv::Mat tex1;
	cv::Mat tex2;
};


class texture
{
	GLuint gl_handle = 0;

public:

	void upload_mat(const cv::Mat& Mat_)
	{

		cv::flip(Mat_, Mat_, 0); // 수평으로 반전
		cvtColor(Mat_, Mat_, cv::COLOR_BGR2RGB); //cvtColar로 RGB컬러로 변환


		if (Mat_.empty()) return;
		//if (!gl_handle)
		
		glGenTextures(2, &gl_handle);
		GLenum err = glGetError();

		int width = Mat_.cols;
		int height = Mat_.rows;

		glBindTexture(GL_TEXTURE_2D, gl_handle);

//#define GL_BGRA 0x80E1 // Use this for 32bit bmp

		GLint cx = width, cy = height;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &cx);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &cy);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Mat_.cols, Mat_.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, Mat_.ptr());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

		glBindTexture(GL_TEXTURE_2D, 0);


	}

	void upload_mat2(const cv::Mat& Mat_)
	{

		cv::flip(Mat_, Mat_, 0);
		cvtColor(Mat_, Mat_, cv::COLOR_BGR2RGB);


		if (Mat_.empty()) return;
		//if (!gl_handle)
		
		glGenTextures(2, &gl_handle);
		GLenum err = glGetError();

		int width = Mat_.cols;
		int height = Mat_.rows;

		glBindTexture(GL_TEXTURE_2D, gl_handle);

//#define GL_BGRA 0x80E1 // Use this for 32bit bmp

		GLint cx = width, cy = height;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &cx);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &cy);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Mat_.cols, Mat_.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, Mat_.ptr());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

		glBindTexture(GL_TEXTURE_2D, 0);


	}

	void upload_bmp(std::string src)
	{
		if (!gl_handle){
			glGenTextures(2, &gl_handle);
		}

		GLenum err = glGetError();

		cv::Mat frame = cv::imread(src);
		cv::flip(frame, frame, 0);

		glBindTexture(GL_TEXTURE_2D, gl_handle);
#define GL_BGR 0x80E0
		glTexImage2D(GL_TEXTURE_2D, 0, 3, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glBindTexture(GL_TEXTURE_2D, 0);
	}


	GLuint get_gl_handle() { return gl_handle; }
};

class window

{
public:
	std::function<void(bool)>           on_left_mouse = [](bool) {};
	std::function<void(double, double)> on_mouse_scroll = [](double, double) {};
	std::function<void(double, double)> on_mouse_move = [](double, double) {};
	std::function<void(int)>            on_key_release = [](int) {};

	window(int width, int height, const char* title)
		: _width(width), _height(height)
	{
		glfwInit();
		win = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!win)
			throw std::runtime_error("Could not open OpenGL window, please check your graphic drivers or use the textual SDK tools");
		glfwMakeContextCurrent(win);

		glfwSetWindowUserPointer(win, this);
		glfwSetMouseButtonCallback(win, [](GLFWwindow* w, int button, int action, int mods)
			{
				auto s = (window*)glfwGetWindowUserPointer(w);
				if (button == 0) s->on_left_mouse(action == GLFW_PRESS);
			});

		glfwSetScrollCallback(win, [](GLFWwindow* w, double xoffset, double yoffset)
			{
				auto s = (window*)glfwGetWindowUserPointer(w);
				s->on_mouse_scroll(xoffset, yoffset);
			});

		glfwSetCursorPosCallback(win, [](GLFWwindow* w, double x, double y)
			{
				auto s = (window*)glfwGetWindowUserPointer(w);
				s->on_mouse_move(x, y);
			});

		glfwSetKeyCallback(win, [](GLFWwindow* w, int key, int scancode, int action, int mods)
			{
				auto s = (window*)glfwGetWindowUserPointer(w);
				if (0 == action) // on key release
				{
					s->on_key_release(key);
				}
			});
	}

	float width() const { return float(_width); }
	float height() const { return float(_height); }

	operator bool()
	{
		glPopMatrix();
		glfwSwapBuffers(win);

		auto res = !glfwWindowShouldClose(win);

		glfwPollEvents();
		glfwGetFramebufferSize(win, &_width, &_height);

		// Clear the framebuffer
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, _width, _height);

		// Draw the images
		glPushMatrix();
		glfwGetWindowSize(win, &_width, &_height);
		glOrtho(0, _width, _height, 0, -1, +1);		//확인

		return res;
	}

	~window()
	{
		glfwDestroyWindow(win);
		glfwTerminate();
	}

	operator GLFWwindow* () { return win; }

private:
	GLFWwindow* win;
	int _width, _height;
};


// Struct for managing rotation of pointcloud view
struct glfw_state {
	glfw_state() : yaw(0.0), pitch(0.0), last_x(0.0), last_y(0.0),
		ml(false), offset_x(2.f), offset_y(2.f) {}
	double yaw;
	double roll;
	double pitch;
	double last_x;
	double last_y;
	bool ml;
	float offset_x;
	float offset_y;
	bool over = 0;
	texture tex;
	texture tex2;
};

void OBJLoader(string path, int i, vector<data_seq>& sequence, vector<int>& texture_num);
void OBJViewer(int data_num, vector<data_seq>& sequence, vector<texture_image> tex_list, vector<int> texture_num);
void register_glfw_callbacks(window& app, glfw_state& app_state);
void render(glfw_state app_state, int frame, vector<data_seq> sequence, int type);