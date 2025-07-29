#pragma once
#include "OCS.h"
#include "OpenGLContext.h"
#include "OpenGLRenderWindow.h"
#include <functional>
#include <unordered_map>
#include <QWidget>
#include <QTimer>

namespace DeepNestCpp
{
	class GLWidget : public QWidget
	{
		Q_OBJECT
	public:
		GLWidget(OpenGLRenderWindow* glwindow, Sketch* sketch, WindowState state = STATIC_DRAW);
		~GLWidget();

		void SetWindowStatus(WindowState state);
		OpenGLWindowContext* GetContet() { return m_context; }

		void update();

	protected:
		void paintEvent(QPaintEvent* event) override;

	private:
		OpenGLWindowContext* m_context;

		QTimer m_updateTimer;

		bool middleBtnPressing = false;
		QPointF previousMousePos;

	};
}