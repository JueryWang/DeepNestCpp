#include "GLWidget.h"
#include "OpenGLRenderWindow.h"
#include "OpenGLContext.h"
#include "Sketch.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

namespace DeepNestCpp
{
	GLWidget::GLWidget(OpenGLRenderWindow* glwindow, Sketch* sketch, WindowState state)
	{
		m_context = new OpenGLWindowContext(glwindow, state);
		glwindow->setParent(this);
		this->setMouseTracking(true);

		this->resize(m_context->window->GetSize());
		m_context->window->SetReciverWidget(this);

		connect(&m_updateTimer, &QTimer::timeout, this, &GLWidget::update);
		this->resize(glwindow->GetSize());
		this->update();

		this->installEventFilter(m_context->window);
	}
	GLWidget::~GLWidget()
	{
		delete m_context;
	}

	void GLWidget::SetWindowStatus(WindowState state)
	{
		m_context->state = state;
		this->update();
	}

	void GLWidget::update()
	{
		if (m_context->state == INACTIVE)
		{
			m_updateTimer.stop();
			return;
		}
		if (m_context->state == STATIC_DRAW)			//OpenGL双缓冲必须运行两次才能得到正确结果
		{
			for (int i = 0; i < 2; i++)
			{
				m_context->window->updateGL();
			}
		}
		if (m_context->state == DYNAMIC_DRAW)
		{
			m_updateTimer.start(16);
			m_context->window->updateGL();
			QMetaObject::invokeMethod(this, "repaint");
		}
	}

	void GLWidget::paintEvent(QPaintEvent* event)
	{
		QPainter painter;
		painter.begin(this);
		QSize size = m_context->window->GetSize();
		painter.drawImage(QRect(0,0,size.width(),size.height()),m_context->window->grabImage());
		painter.end();
	}
}
