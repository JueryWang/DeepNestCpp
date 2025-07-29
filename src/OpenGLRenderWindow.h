#pragma once
#include <QImage>
#include <QSize>
#include <QWidget>
#include <map>
#include "Shader.h"
#include "OpenGLContext.h"

namespace DeepNestCpp
{
	class GLWidget;
	class Sketch;

	extern int bit_per_pixel;
	extern uchar* opengl_swap_buffer;

	class OpenGLRenderWindow : public QObject
	{
		friend class GLWidget;

	public:
		OpenGLRenderWindow(int width, int height, const char* title);
		~OpenGLRenderWindow();

		void SetReciverWidget(GLWidget* wiget);
		void SetMousePos(const QPoint& pos) {}

		void Resize(const QSize& size);
		GLWidget* GetReciverWidget() { return m_reciver; }
		QSize inline GetSize() const { return QSize(m_width,m_height); }
		int inline width() { return m_width; }
		int inline height() { return m_height; }

		QImage grabImage();

	protected:
		virtual void updateGL() = 0;

	private:
		struct Deleter {
			void operator()(GLFWwindow* w)
			{
				if (w != nullptr)
				{
					glfwTerminate();
					glfwDestroyWindow(w);
				}
			}
		};

	protected:
		GLWidget* m_reciver = nullptr;
		uchar* m_windowbuf = nullptr;
		std::map<RenderMode, Shader*> m_shaderMap;
		std::unique_ptr<GLFWwindow, Deleter> m_window;

		int m_width;
		int m_height;

		QPoint m_mousePos;
	};

}