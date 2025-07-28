#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace DeepNestCpp
{
	class OpenGLRenderWindow;

	enum WindowState
	{
		DYNAMIC_DRAW = 0,			//鼠标激活的当前窗口(实时渲染)
		STATIC_DRAW = 1,			//静态激活窗口(只渲染当前一帧)
		INACTIVE = 2				//未激活窗口(未销毁,但是隐藏的窗口)
	};

	enum RenderMode
	{
		Normal,
		ShowArrow,
		DrawTickers
	};

	static int g_RenderWindowID = 0;

	struct OpenGLWindowContext
	{
	public:
		OpenGLWindowContext(OpenGLRenderWindow* _window, WindowState _state) : window(_window), state(_state), id(g_RenderWindowID++)
		{

		}

		~OpenGLWindowContext()
		{
			delete window;
		}

		bool operator==(const struct OpenGLWindowContext& other)
		{
			return id == other.id;
		}

		int id;
		OpenGLRenderWindow* window = nullptr;
		WindowState state;
	};

	void InitializeOpenGL();
}
