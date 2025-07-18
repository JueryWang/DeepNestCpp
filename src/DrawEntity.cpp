#include "DrawEntity.h"
#include "../lib/OGL/glm/gtc/type_ptr.hpp"
#include "MathUtils.h"

namespace DeepNestCpp
{

	Point2D::Point2D(glm::vec3 point)
	{
		this->point = point;
		this->aabb = new AABB(point, point);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), glm::value_ptr(point), GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
	Point2D::~Point2D()
	{

	}
	void Point2D::Paint()
	{
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	void Point2D::Move(glm::vec3 offset)
	{
		point += offset;
	}
	void Point2D::Rotate(glm::vec3 center, float angle)
	{
		glm::vec3 axis = glm::vec3(1,0,0);
		glm::vec3 temp = point - center;

		float dot = glm::dot(temp, axis);
		float cos_t = dot / (temp.length());
		float radian = acos(cos_t);
		float targetAngle = radian + angle;
		point = center + glm::vec3(cos(deg2Rad*targetAngle)*temp.length(),sin(deg2Rad* targetAngle)*temp.length(),0.0f);
	}
	void Point2D::Mirror(glm::vec3 center)
	{
		glm::vec3 reflect = center - point;
		reflect *= 2;
		point += reflect;
	}

	void Point2D::SetParameter(int paramCount, ...)
	{
	}

	void Point2D::ToNcInstruction()
	{

	}



	Line2D::Line2D(glm::vec3 start, glm::vec3 end) : start(start),end(end)
	{
		aabb = new AABB(start,end);

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		
		glm::vec3 vertices[] = {start,end};
		glBufferData(GL_ARRAY_BUFFER,2 * sizeof(glm::vec3),vertices,GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(glm::vec3),(void*)0);
		glEnableVertexAttribArray(0);
	}

	Line2D::~Line2D()
	{

	}
	void Line2D::Paint()
	{
		glPointSize(1.0f);
		glBindVertexArray(vao);
		glDrawArrays(GL_LINES,0,2);
	}

	void Line2D::Move(glm::vec3 offset)
	{
		start += offset;
		end += offset;
	}

	void Line2D::Rotate(glm::vec3 center,float angle)
	{

	}
	void Line2D::Mirror(glm::vec3 center)
	{

	}

	void Line2D::SetParameter(int paramCount, ...)
	{
	}

	void Line2D::ToNcInstruction()
	{
	}


	Arc2D::Arc2D(glm::vec3 center, float radius, float startAngle, float endAngle) : center(center),radius(radius),startAngle(startAngle),endAngle(endAngle)
	{
		start = glm::vec3(center.x + cos(startAngle * deg2Rad),center.y + sin(startAngle * deg2Rad),0.0f);
		end = glm::vec3(center.x + cos(endAngle * deg2Rad),center.y + sin(endAngle*deg2Rad),0.0f);

		if(glm::cross(start,end).z < 0)
		{
			std::swap(start,end);
		}

		if(startAngle > endAngle)
		{
			endAngle = endAngle + 360;
		}

		GenerateArcSamples(startAngle,endAngle,center,arcSamples);

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);

		glBufferData(GL_ARRAY_BUFFER,3 * sizeof(glm::vec3),arcSamples.data(),GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,false, sizeof(glm::vec3),(void*)0);
		glEnableVertexAttribArray(0);
	}
	Arc2D::~Arc2D()
	{

	}

	void Arc2D::GenerateArcSamples(float startAngle,float endAngle,const glm::vec3& center,std::vector<glm::vec3>& samples)
	{
		if(samples.size())
			samples.clear();
		
		for(int angle = startAngle; angle <= (endAngle+0.99f); angle++)
		{
			float x = (float)(radius * cos(angle * deg2Rad));
			float y = (float)(radius * sin(angle * deg2Rad));

			
			samples.push_back(glm::vec3(center.x + x,center.y + y,0.0f));
			aabb->Union(glm::vec3(center.x,center.y,0.0f));
		}
	}

	void Arc2D::Paint()
	{
		glLineWidth(1);
		glBindVertexArray(vao);
		glDrawArrays(GL_LINE_STRIP,0,arcSamples.size());
	}
	void Arc2D::Move(glm::vec3 offset)
	{

	}
	void Arc2D::Rotate(glm::vec3 center,float angle)
	{

	}
	void Arc2D::Mirror(glm::vec3 center)
	{
		
	}

	void Arc2D::SetParameter(int paramCount, ...)
	{
	}

	void Arc2D::ToNcInstruction()
	{
	}


	Circle2D::Circle2D(glm::vec3 center, float radius) : center(center),radius(radius)
	{
		aabb = new AABB(glm::vec3(center.x - radius,center.y - radius,0.0f),glm::vec3(center.x + radius,center.y + radius,0.0f));
		
		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		
		glBufferData(GL_ARRAY_BUFFER,circleSamples.size() * sizeof(glm::vec3),circleSamples.data(),GL_DYNAMIC_DRAW);
		
		glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(glm::vec3),(void*)0);
		glEnableVertexAttribArray(0);
	}
	Circle2D::~Circle2D()
	{
	}

	void Circle2D::Paint()
	{
		glBindVertexArray(vao);
		glDrawArrays(GL_LINE_STRIP,0,circleSamples.size());
	}
	void Circle2D::Move(glm::vec3 offset)
	{
	}
	void Circle2D::Rotate(glm::vec3 center,float angle)
	{
	}
	void Circle2D::Mirror(glm::vec3 center)
	{

	}

	void Circle2D::SetParameter(int paramCount, ...)
	{
	}

	void Circle2D::ToNcInstruction()
	{
	}

	void Circle2D::GenerateCircleSamplePoints(const glm::vec3& center,float radius,int stepAngle,std::vector<glm::vec3> &samples)
	{
		if(circleSamples.size())
			samples.clear();

		for(int angle = 0; angle <=360; angle+= stepAngle)
		{
			float x = (radius * cos(angle * deg2Rad));
			float y = (radius * sin(angle * deg2Rad));

			samples.push_back(glm::vec3(center.x + x,center.y + y,0.0));
		}
	}


	Ellipse2D::Ellipse2D(glm::vec3 center, float radiusX, float radiusY) : center(center),radiusX(radiusX),radiusY(radiusY)
	{
		aabb = new AABB(glm::vec3(center.x - radiusX,center.y - radiusY,0.0),glm::vec3(center.x + radiusX,center.y + radiusX,0.0f));
	
		GenerateEllipseSamplePoints(center,radiusX,radiusY,5,ellipseSamples);

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);

		glBufferData(GL_ARRAY_BUFFER,ellipseSamples.size() * sizeof(glm::vec3),ellipseSamples.data(),GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(glm::vec3),(void*)0);
		glEnableVertexAttribArray(0);
	}

	Ellipse2D::~Ellipse2D()
	{

	}
	void Ellipse2D::Paint()
	{
		glBindVertexArray(vao);
		glDrawArrays(GL_LINE_STRIP,0,ellipseSamples.size());
	}
	void Ellipse2D::Move(glm::vec3 offset)
	{
	}
	void Ellipse2D::Rotate(glm::vec3 center,float angle)
	{
	}
	void Ellipse2D::Mirror(glm::vec3 center)
	{
	}

	void Ellipse2D::SetParameter(int paramCount, ...)
	{
	}

	void Ellipse2D::ToNcInstruction()
	{
	}

	void Ellipse2D::GenerateEllipseSamplePoints(glm::vec3 center,float radiusX,float radiusY,int stepAngle,std::vector<glm::vec3>& samples)
	{
		if(samples.size() != 0)
			samples.clear();

		for(int angle = 0; angle <= 360;angle += stepAngle)
		{
			float x = radiusX * cos(angle * deg2Rad);
			float y = radiusY * sin(angle * deg2Rad);

			samples.push_back(glm::vec3(center.x + x,center.y + y,0.0f));
		}
	}

	Polyline2D::Polyline2D(const std::vector<glm::vec3>& points,bool isClosed) : nodes(points),isClosed(isClosed)
	{
		if(nodes.size() > 2)
		{
			aabb = new AABB(nodes[0],nodes[1]);
			for(int i = 2; i < nodes.size(); i++)
			{
				aabb->Union(nodes[i]);
			}
		}

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);

		glBufferData(GL_ARRAY_BUFFER,nodes.size() * sizeof(glm::vec3),nodes.data(),GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(glm::vec3),(void*)0);
		glEnableVertexAttribArray(0);
	}
	Polyline2D::~Polyline2D()
	{
	}
	void Polyline2D::Paint()
	{
		glBindVertexArray(vao);
		glDrawArrays(isClosed ? GL_LINE_LOOP : GL_LINE_STRIP,0,nodes.size());
	}
	void Polyline2D::Move(glm::vec3 offset)
	{

	}
	void Polyline2D::Rotate(glm::vec3 center,float angle)
	{
	}
	void Polyline2D::Mirror(glm::vec3 center)
	{

	}

	void Polyline2D::SetParameter(int paramCount, ...)
	{
	}

	void Polyline2D::ToNcInstruction()
	{
	}

	Spline2D::Spline2D(const std::vector<glm::vec3>& controlPoints, const std::vector<float> knots) : controlPoints(controlPoints),knots(knots)
	{
		GenerateSplineSamplePoints(controlPoints,splineSamples);

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		
		glBufferData(GL_ARRAY_BUFFER,splineSamples.size() * sizeof(glm::vec3),splineSamples.data(),GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(glm::vec3),(void*)0);
		glEnableVertexAttribArray(0);

		glGenVertexArrays(1,&vao_cntl);
		glBindVertexArray(vao_cntl);

		glGenVertexArrays(1,&vbo_cntl);
		glBindBuffer(GL_ARRAY_BUFFER,vbo_cntl);
		glBufferData(GL_ARRAY_BUFFER,controlPoints.size() * sizeof(glm::vec3),controlPoints.data(),GL_DYNAMIC_DRAW);
	
		glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(glm::vec3),(void*)0);
		glEnableVertexAttribArray(0);
	}
    Spline2D::~Spline2D()
	{

	}
	void Spline2D::Paint()
	{
		glBindVertexArray(vao);
		glDrawArrays(GL_LINE_STRIP,0,splineSamples.size());
	}
    void Spline2D::Move(glm::vec3 offset)
	{

	}
    void Spline2D::Rotate(glm::vec3 center, float angle)
	{

	}
    void Spline2D::Mirror(glm::vec3 center)
	{

	}

	void Spline2D::SetParameter(int paramCount, ...)
	{
	}

	void Spline2D::ToNcInstruction()
	{
	}

    void Spline2D::GenerateSplineSamplePoints(const std::vector<glm::vec3>& controlPoints,std::vector<glm::vec3> &samples)
	{
		if(samples.size() != 0)
			samples.clear();
		
		delete aabb;
		aabb = nullptr;

		for(float t = 0.f; t<=1.0f;t+=0.01f)
		{
			glm::vec3 samplePoint = MathUtils::CalculateBSpline(controlPoints,knots,3,t);
			samples.push_back(samplePoint);
			if(aabb == nullptr)
				aabb = new AABB(samplePoint,samplePoint);
			else
				aabb->Union(samplePoint);
		}
	}
}

