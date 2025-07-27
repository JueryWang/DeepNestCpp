#include "DrawEntity.h"
#include "../lib/OGL/glm/gtc/type_ptr.hpp"
#include "MathUtils.h"
#include <stdarg.h>

std::vector<glm::vec3> intermidatePolygon;

namespace DeepNestCpp
{
	
	Point2D::Point2D(glm::vec3 point)
	{
		this->point = point;
		centroid = point;
		this->bbox = new AABB(point, point);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//cgalPath.push_back(Point(point.x,point.y));
		bbox = new AABB(point, point);
	}
	Point2D::~Point2D()
	{

	}
	void Point2D::Paint()
	{
		if(vao < 0 || vbo < 0)
		{
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), glm::value_ptr(point), GL_STATIC_DRAW);
		}
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
		if (paramCount == 1)
		{
			va_list args;
			point = va_arg(args, glm::vec3);
			va_end(args);

			if (vao > 0 && vbo > 0)
			{
				glBindVertexArray(vao);

				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &point, GL_STATIC_DRAW);
			}

			//cgalPath.clear();
			//cgalPath.push_back(Point(point.x, point.y));
			*bbox = AABB(point,point);
		}
	}

	std::string Point2D::ToNcInstruction()
	{

	}



	Line2D::Line2D(glm::vec3 start, glm::vec3 end) : start(start),end(end)
	{
		bbox = new AABB(start,end);

		//cgalPath.push_back(Point(start.x,start.y));
		//cgalPath.push_back(Point(end.x, end.y));
	}

	Line2D::~Line2D()
	{

	}
	void Line2D::Paint()
	{
		if (vao < 0 || vbo < 0)
		{
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			glm::vec3 vertices[] = { start,end };
			glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(0);
		}

		centroid = (start + end);
		centroid /= 2;

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
		if (paramCount == 2)
		{
			va_list args;
			start = va_arg(args,glm::vec3);
			end = va_arg(args, glm::vec3);
			*bbox = AABB(start,end);
			area = 0;

			centroid = start + end;
			centroid /= 2;

			if (vao > 0 && vbo > 0)
			{
				glBindVertexArray(vao);
				glBindVertexArray(vbo);
				glm::vec3 arrays[] = { start,end };
				glBufferData(GL_ARRAY_BUFFER, 2*sizeof(glm::vec3), arrays, GL_STATIC_DRAW);
			}
			//cgalPath.clear();
			//cgalPath.push_back(Point(start.x,start.y));
			//cgalPath.push_back(Point(end.x, end.y));
		}
	}

	std::string Line2D::ToNcInstruction()
	{
		std::string s;
		char buffer[100];
		std::sprintf(buffer, "G1 X%f Y%f\r\n", end.x, end.y);
		s += buffer;
		return s;
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

		glBufferData(GL_ARRAY_BUFFER,3 * sizeof(glm::vec3),arcSamples.data(),GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,false, sizeof(glm::vec3),(void*)0);
		glEnableVertexAttribArray(0);

		centroid = glm::vec3(0.0);
		for (const glm::vec3& vec : arcSamples)
		{
			//cgalPath.push_back(Point(vec.x,vec.y));
			centroid += vec;
		}

		centroid /= arcSamples.size();
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
			bbox->Union(glm::vec3(center.x,center.y,0.0f));
		}
	}

	void Arc2D::Paint()
	{
		if (vao < 0 || vbo < 0)
		{
			glGenBuffers(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER,vbo);

			glBufferData(GL_ARRAY_BUFFER, arcSamples.size() * sizeof(glm::vec3), arcSamples.data(), GL_STATIC_DRAW);
			
			glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(0);
		}
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
		if (paramCount == 3)
		{
			arcSamples.clear();
			//cgalPath.clear();
			
			va_list args;
			center = va_arg(args,glm::vec3);
			startAngle = va_arg(args, float);
			endAngle = va_arg(args, float);
			radius = va_arg(args, float);
			area = abs((startAngle - endAngle) * deg2Rad / (PI * radius));
			start = glm::vec3(center.x + radius * (float)cos(startAngle * deg2Rad),center.y + radius * (float)sin(startAngle * deg2Rad),0.0f);
			end = glm::vec3(center.x + radius * (float)cos(endAngle * deg2Rad),center.y + radius * (float)sin(endAngle * deg2Rad),0.0f);

			*bbox = AABB(start, end);

			glm::vec3 start = glm::vec3(start);
			glm::vec3 end3d = glm::vec(end);

			if (glm::cross(start,end).z < 0)
			{
				auto temp = start;
				start = end;
				end = temp;
			}

			if (startAngle > endAngle)
			{
				endAngle = endAngle + 360;
			}

			GenerateArcSamples(startAngle,endAngle,center,arcSamples);

			if (vao > 0 && vbo > 0)
			{
				glBindVertexArray(vao);
				
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, arcSamples.size() * sizeof(glm::vec3), arcSamples.data(), GL_STATIC_DRAW);
			}

			centroid = glm::vec3(0.0f);
			for(const glm::vec3& p : arcSamples)
			{
				//cgalPath.push_back(Point(p.x,p.y));
				center += p;
			}
			center /= arcSamples.size();
		}
	}

	std::string Arc2D::ToNcInstruction()
	{
		std::string s = "";
		GeomDirection dir = MathUtils::GetDirection(this->centroid, start, end);
		float I = center.x - start.x;
		float J = center.y - start.y;
		char buffer[100];
		std::sprintf(buffer,"G0 X%f Y%f\r\n",start.x,start.y);
		s += buffer;
		if (dir == GeomDirection::CCW)
		{
			if (abs(startAngle - endAngle) > 180.0f)
			{
				std::sprintf(buffer, "G2 X%f Y%f I%f J%f\r\n", end.x, end.y, I, J);
				s += buffer;
			}
			else
			{
				std::sprintf(buffer, "G3 X%f Y%f I%f J%f\r\n", end.x, end.y, I, J);
				s += buffer;
			}
		}
		else
		{
			if (abs(startAngle - endAngle) > 180.0f)
			{
				std::sprintf(buffer, "G3 X%f Y%f I%f J%f\r\n", end.x, end.y, I, J);
				s += buffer;
			}
			else
			{
				std::sprintf(buffer, "G2 X%f Y%f I%f J%f\r\n", end.x, end.y, I, J);
				s += buffer;
			}
		}
		return s;
	}


	Circle2D::Circle2D(glm::vec3 center, float radius) : center(center),radius(radius)
	{
		bbox = new AABB(glm::vec3(center.x - radius,center.y - radius,0.0f),glm::vec3(center.x + radius,center.y + radius,0.0f));
		GenerateCircleSamplePoints(center,radius,5,circleSamples);
		area = 2 * (float)PI * radius;

		centroid = glm::vec3(0.0f);
		for (const glm::vec3& vec : circleSamples)
		{
			//cgalPath.push_back(Point(vec.x, vec.y));
			centroid += vec;
		}
		centroid /= circleSamples.size();

		bbox = new AABB(glm::vec3(center.x - radius,center.y - radius,0.0f),glm::vec3(center.x + radius,center.y + radius,0.0f));
	}
	Circle2D::~Circle2D()
	{
	}

	void Circle2D::Paint()
	{
		if (vao < 0 || vbo < 0)
		{
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			GenerateCircleSamplePoints(center, radius, 5, circleSamples);
			glBufferData(GL_ARRAY_BUFFER, circleSamples.size() * sizeof(glm::vec3), circleSamples.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(0);
		}
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
		if (paramCount == 2)
		{
			circleSamples.clear();
			//cgalPath.clear();

			va_list args;
			center = va_arg(args, glm::vec3);
			radius = va_arg(args, float);
			
			GenerateCircleSamplePoints(center,radius,10,circleSamples);
			area = 2 * (float)PI * radius;
			*bbox = AABB(glm::vec3(center.x - radius,center.y - radius,1.0f),glm::vec3(center.x + radius,center.y + radius,1.0f));
		
			if (vao > 0 && vbo > 0)
			{
				glBindVertexArray(vao);

				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, circleSamples.size() * sizeof(glm::vec3), circleSamples.data(), GL_STATIC_DRAW);
			}

			centroid = glm::vec3(0.0f);
			for (const glm::vec3& vec : circleSamples)
			{
				//cgalPath.push_back(Point(vec.x, vec.y));
				centroid += vec;
			}
			centroid /= circleSamples.size();
		}
	}

	std::string Circle2D::ToNcInstruction()
	{
		std::string s = "";
		float I = radius;
		float J = 0;
		glm::vec3 start = center + glm::vec3(0, radius, 0.0f);
		glm::vec3 end = center + glm::vec3(0, radius, 0.0f);
		char buffer[100];
		std::sprintf(buffer, "G0 X%f Y%f \r\n");
		s += buffer;
		std::sprintf(buffer,"G2 X%f Y%f \r\n");
		s += buffer;
		return s;
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


	Ellipse2D::Ellipse2D(glm::vec3 center, float radiusX, float radiusY) : center(center)
	{
		bbox = new AABB(glm::vec3(center.x - radiusX,center.y - radiusY,0.0),glm::vec3(center.x + radiusX,center.y + radiusX,0.0f));
	
		GenerateEllipseSamplePoints(center,radiusX,radiusY,5,ellipseSamples);

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);

		glBufferData(GL_ARRAY_BUFFER,ellipseSamples.size() * sizeof(glm::vec3),ellipseSamples.data(),GL_STATIC_DRAW);
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

	std::string Ellipse2D::ToNcInstruction()
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
		if (nodes[0] == *(nodes.end() - 1))
			isClosed = true;

		if(nodes.size() > 2)
		{
			bbox = new AABB(nodes[0],nodes[1]);
			for(int i = 2; i < nodes.size(); i++)
			{
				bbox->Union(nodes[i]);
			}
		}
		else
		{
			bbox = new AABB(nodes[0], nodes[0]);
		}

		centroid = glm::vec3(0.0f);
		for (glm::vec3& p : nodes)
		{
			//cgalPath.push_back(Point(p.x,p.y));
			centroid += p;
		}
		centroid /= nodes.size();
		//area = cgalPath.area();
	}
	Polyline2D::~Polyline2D()
	{
	}
	void Polyline2D::Paint()
	{
		if (vao < 0 || vbo < 0)
		{
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			glBufferData(GL_ARRAY_BUFFER, nodes.size() * sizeof(glm::vec3), nodes.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(0);
		}
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
		if (paramCount == 2)
		{
			//cgalPath.clear();

			va_list args;
			nodes = va_arg(args, std::vector<glm::vec3>);
			isClosed = va_arg(args, bool);

			if (vao > 0 && vbo > 0)
			{
				glBindVertexArray(vao);

				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, nodes.size() * sizeof(glm::vec3), nodes.data(), GL_STATIC_DRAW);
			}

			if (nodes.size() > 2)
			{
				*bbox = AABB(nodes[0],nodes[1]);
				for (int i = 2; i < nodes.size(); i++)
				{
					bbox->Union(nodes[i]);
				}
			}
			else
			{
				*bbox = AABB(nodes[0],nodes[0]);
			}

			centroid = glm::vec3(0.0f);
			for (glm::vec3& p : nodes)
			{
				//cgalPath.push_back(Point(p.x, p.y));
				centroid += p;
			}
			centroid /= nodes.size();
			//area = cgalPath.area();
			
		}
	}

	std::string Polyline2D::ToNcInstruction()
	{
		std::string s;
		char buffer[100];
		for (glm::vec3& pos : this->nodes)
		{
			std::sprintf(buffer, "G1 X%f Y%f\r\n", pos.x, pos.y);
			s += buffer;
		}
		if (isClosed)
		{
			std::sprintf(buffer, "G1 X%f Y%f\r\n", this->nodes[0].x, this->nodes[0].y);
			s += buffer;
		}
		return s;
	}

	void Polyline2D::Offset(double delta)
	{
	}

	void Polyline2D::Simplify(float epsilon)
	{
		intermidatePolygon = MathUtils::DouglasPeucker(this->nodes, epsilon);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, intermidatePolygon.size() * sizeof(glm::vec3), intermidatePolygon.data(), GL_STATIC_DRAW);
	}

	void Polyline2D::Smooth(float epsilon)
	{
	}

	Spline2D::Spline2D(const std::vector<glm::vec3>& controlPoints, const std::vector<float> knots,bool isPassTrough) : controlPoints(controlPoints),knots(knots),isPasstrhough(isPasstrhough)
	{

		if (!isPasstrhough)
		{
			GenerateSplineSamplePoints(controlPoints,splineSamples);
		}
		else
		{
			this->splineSamples = MathUtils::CatmullRomSmooth(controlPoints,100);
		}

		bbox = new AABB(splineSamples[0],splineSamples[1]);
		
		for (glm::vec3& p : splineSamples)
		{
			bbox->Union(p);
			//cgalPath.push_back(Point(p.x, p.y));
		}

		centroid = glm::vec3(0.0f);
		for (const glm::vec3& p : controlPoints)
		{
			centroid += p;
		}
		centroid /= controlPoints.size();
		//area = cgalPath.area();

	}
    Spline2D::~Spline2D()
	{

	}
	void Spline2D::Paint()
	{
		if (vao < 0 || vbo < 0)
		{
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			glBufferData(GL_ARRAY_BUFFER, splineSamples.size() * sizeof(glm::vec3), splineSamples.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(0);

			glGenVertexArrays(1, &vao_cntl);
			glBindVertexArray(vao_cntl);

			glGenBuffers(1, &vbo_cntl);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_cntl);
			glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(glm::vec3), controlPoints.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), 0);
			glEnableVertexAttribArray(0);
		}
		
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
		if (paramCount == 2)
		{
			controlPoints.clear();
			splineSamples.clear();
			//cgalPath.clear();

			va_list args;
			controlPoints = va_arg(args, std::vector<glm::vec3>);
			isPasstrhough = va_arg(args, bool);

			if (isPasstrhough)
			{
				GenerateSplineSamplePoints(controlPoints, splineSamples);
			}
			else
			{
				splineSamples = MathUtils::CatmullRomSmooth(controlPoints, 100);
			}

			if (vao > 0 && vbo > 0)
			{
				glBindVertexArray(vao);

				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, splineSamples.size() * sizeof(glm::vec3), splineSamples.data(), GL_STATIC_DRAW);
			}

			centroid = glm::vec3(0.0f);
			for (glm::vec3& p : controlPoints)
			{
				centroid += p;
				//cgalPath.push_back(Point(p.x,p.y));
			}
			centroid /= controlPoints.size();

			//area = cgalPath.area();
		}
	}

	std::string Spline2D::ToNcInstruction()
	{
		std::string s;
		char buffer[100];
		for (glm::vec3& pos : splineSamples)
		{
			sprintf(buffer, "G1 X%f Y%f\r\n", pos.x, pos.y);
			s += buffer;
		}
		return s;
	}

    void Spline2D::GenerateSplineSamplePoints(const std::vector<glm::vec3>& controlPoints,std::vector<glm::vec3> &samples)
	{
		if(samples.size() != 0)
			samples.clear();
		
		delete bbox;
		bbox = nullptr;

		for(float t = 0.f; t<=1.0f;t+=0.01f)
		{
			glm::vec3 samplePoint = MathUtils::CalculateBSpline(controlPoints,knots,3,t);
			samples.push_back(samplePoint);
			if(bbox == nullptr)
				bbox = new AABB(samplePoint,samplePoint);
			else
				bbox->Union(samplePoint);
		}
	}
}

