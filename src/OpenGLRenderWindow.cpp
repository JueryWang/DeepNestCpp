#include "OpenGLRenderWindow.h"
#include "Sketch.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

namespace DeepNestCpp
{
	int bit_per_pixel = 3;

	uchar* opengl_swap_buffer = nullptr;

	OpenGLRenderWindow::OpenGLRenderWindow(int width, int height, const char* title) : m_width(width), m_height(height)
	{
		if (opengl_swap_buffer == nullptr)
		{
			opengl_swap_buffer = (uchar*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * bit_per_pixel);
		}
		if (m_windowbuf == nullptr)
		{
			m_windowbuf = (uchar*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * bit_per_pixel);
		}

		GLFWwindow* glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
		if (glfwWindow == nullptr)
		{
			glfwTerminate();
			return;
		}
		m_window.reset(glfwWindow);
		glfwMakeContextCurrent(m_window.get());
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}

	OpenGLRenderWindow::~OpenGLRenderWindow()
	{
		m_window.reset();
		for (std::pair<RenderMode, Shader*> pair : m_shaderMap)
		{
			if (pair.second)
				delete pair.second;
		}
		free(m_windowbuf);
		free(opengl_swap_buffer);
	}

	void OpenGLRenderWindow::SetReciverWidget(GLWidget* widget)
	{
		m_reciver = widget;
	}

	void OpenGLRenderWindow::Resize(const QSize& size)
	{
		m_width = size.width(); m_height = size.height();
		glfwSetWindowSize(m_window.get(), m_width, m_height);
	}

	QImage OpenGLRenderWindow::grabImage()
	{
		return QImage(m_windowbuf, m_width, m_height, QImage::Format_RGB888).mirrored(false, true);
	}
}