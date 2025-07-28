#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace DeepNestCpp
{
	class OpenGLRenderWindow;

	enum WindowState
	{
		DYNAMIC_DRAW = 0,			//��꼤��ĵ�ǰ����(ʵʱ��Ⱦ)
		STATIC_DRAW = 1,			//��̬�����(ֻ��Ⱦ��ǰһ֡)
		INACTIVE = 2				//δ�����(δ����,�������صĴ���)
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
