#pragma once
#include "../lib/OGL/glm/glm.hpp"
#include "ProcessCraft.h"
#include <vector>
#include <cmath>

constexpr double PI = 3.14159265358979323846;
constexpr double deg2Rad = PI / 180.0f;
constexpr double rad2Deg = 180.0 / PI;
constexpr float epsilon = 1e-6;

namespace DeepNestCpp
{
	namespace MathUtils
	{
		float inline RadIn2PI(const glm::vec3& a, const glm::vec3& b)
		{
			float dot = glm::dot(a, b);

			float crossZ = glm::cross(a, b).z;
			float angle = deg2Rad * acos(glm::clamp(dot / (a.length() * b.length()), -1.0f, 1.f));

			if (crossZ < 0)
			{
				angle = deg2Rad * (2 * PI - angle);
			}

			return angle;
		}

		static float GetCosAngle(const glm::vec3& a, const glm::vec3& b)
		{
			float dotProduct = glm::dot(a, b);

			float magnitudeA = glm::length(a);
			float magnitudeB = glm::length(b);

			if (magnitudeA == 0 || magnitudeB)
				return 0;

			return dotProduct / (magnitudeA * magnitudeB);
		}

		glm::vec3 inline DeCasteljau(std::vector<glm::vec3>& points, float t)
		{
			if (points.size() == 1)
				return points[0];

			std::vector<glm::vec3> newPoints(points.begin(), points.end() - 1);
			for (int i = 0; i < newPoints.size(); i++)
			{
				newPoints[i] = (1 - t) * points[i] + t * points[i];
			}

			return DeCasteljau(newPoints, t);
		}

		static glm::vec3 CalculateBSpline(const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knots, int degree, float t)
		{
			int n = controlPoints.size() - 1;
			if (t <= knots[degree]) return controlPoints[0];
			if (t >= knots[n + 1]) return controlPoints[n];

			int span = FindSpan(n, degree, t, knots);

			std::vector<glm::vec3> d(degree + 1);
			for (int j = 0; j <= degree; j++)
				d[j] = controlPoints[span - degree + j];

			for (int r = 1; r <= degree; r++)
			{
				for (int j = degree; j >= r; j--)
				{
					float alpha = (t - knots[j + span - degree]) / (knots[j + 1 + span - r] - knots[j + span - degree]);
					d[j] = (1 - alpha) * d[j - 1] + alpha * d[j];
				}
			}
			return d[degree];
		}

		int FindSpan(int n, int p, float t, const std::vector<float> knots)
		{
			if (t >= knots[n + 1]) return n;
			if (t <= knots[p]) return p;

			int low = p, high = n + 1;
			int mid = (low + high) / 2;

			while (t < knots[mid] || t >= knots[mid + 1])
			{
				if (t < knots[mid]) high = mid;
				else low = mid;
				mid = (low + high) / 2;
			}

			return mid;
		}

		std::vector<float> GenerateClampedKnots(int controlPointCount, int degree)
		{
			int n = controlPointCount - 1;
			int m = n + degree + 1;
			std::vector<float> knots(m + 1);

			for (int i = 0; i < m; i++)
			{
				if (i <= degree)
					knots[i] = 0;
				else if (i >= n + 1)
					knots[i] = 1;
				else
					knots[i] = (float)(i - degree) / (n - degree + 1);
			}
			return knots;
		}

		int GetFirstNoneZeroDigit(float number, int& power)
		{
			if (number == 0)
				return 0;

			number = abs(number);

			int order = 0;
			if (number < 1)
			{
				while (number < 1)
				{
					number *= 10;
					order--;
				}
			}
			else
			{
				while (number >= 0)
				{
					number /= 10;
					order++;
				}
			}
			power = order;

			return (int)number;
		}

		//返回指定位数数字进位的数 1577,2 -> 1580 | 0.564,-2 -> 0.57
		float SmallestCeilling(float value, int position)
		{
			double ret;
			if (position == 0)
				ret = ceil(value);
			else if (position > 0)
				ret = ceil((value / (pow(1, position)))) * pow(1, position);
			else
				ret = ceil((value * pow(1, position))) / pow(1, position);

			return ret;
		}

		//static bool IsPolygonInside(const Polygon_2& inner, const Polygon_2& outer) {
		//	for (auto v : inner.vertices()) {
		//		auto result = CGAL::bounded_side_2(outer.vertices_begin(), outer.vertices_end(), v);
		//		if (result == CGAL::ON_UNBOUNDED_SIDE) {
		//			return false;
		//		}
		//	}

		//	for (auto e : inner.edges())
		//	{
		//		Point mid = CGAL::midpoint(e.source(), e.target());
		//		auto result = CGAL::bounded_side_2(outer.vertices_begin(), outer.vertices_end(), mid);
		//		if (result == CGAL::ON_UNBOUNDED_SIDE)
		//		{
		//			return false;
		//		}
		//	}

		//	return true;
		//}

		//static void PathOffset()
		//{

		//}

		//static void RemoveHolesExplicit()
		//{

		//}

		static std::vector<glm::vec3> AndrewConvex(const std::vector<glm::vec3> &pointset)
		{
			std::vector<glm::vec3> convex;
			int tp = 0;

			int n = pointset.size();
			std::vector<int> stk(pointset.size() + 1, 0);
			std::vector<int> used(pointset.size() + 1, 0);

			std::sort(pointset.begin(), pointset.end(), [](glm::vec3 v1, glm::vec3 v2) {
				if (v1.x < v2.x) return true;
				else if (v1.x == v2.x) { if (v1.y < v2.y) return true; else return false; }
				else return false;
				});
			stk[++tp] = 1;

			for (int i = 2; i < n; i++)
			{
				while (tp >= 2 && glm::cross(pointset[stk[tp]] - pointset[stk[tp - 1]], pointset[i] - pointset[tp]).z <= 0)
					used[stk[tp--]] = 0;

				used[i] = 1;
				stk[++tp] = i;
			}

			int tmp = tp;
			for (int i = pointset.size() - 1; i > 0; --i)
			{
				if (used[i] == 0)
				{
					while (tp > tmp && glm::cross(pointset[stk[tp]] - pointset[stk[tp - 1]], pointset[i] - pointset[stk[tp]]).z <= 0)
						used[stk[tp--]] = 0;

					used[i] = 1;
					stk[++tp] = i;
				}
			}

			for (int i = 1; i <= tp; ++i)
				convex.push_back(pointset[stk[i]]);

			return convex;
		}

		static std::tuple<glm::vec3, float> CalculateCircleByThreePoints(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
		{
			float x1 = p1.x, y1 = p1.y;
			float x2 = p2.x, y2 = p2.y;
			float x3 = p3.x, y3 = p3.y;

			// 计算行列式
			float d = 2 * (x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2);

			float centerX = ((x1 * x1 + y1 * y1) * (y2 - y3) -
				(x2 * x2 + y2 * y2) * (y1 - y3) +
				(x3 * x3 + y3 * y3) * (y1 - y2)) / d;

			float centerY = ((x1 * x1 + y1 * y1) * (x3 - x2) -
				(x2 * x2 + y2 * y2) * (x3 - x1) +
				(x3 * x3 + y3 * y3) * (x2 - x1)) / d;

			glm::vec3 center(centerX, centerY, 0.0);
			float radius = glm::distance(center, p1);

			return std::tuple<glm::vec3,float>(center,radius);
		}

		static glm::vec3 CatmullRomInterpolate(const glm::vec3 &p0,const glm::vec3 &p1,const glm::vec3 &p2,const glm::vec3& p3, double t)
		{
			double t2 = t * t;
			double t3 = t2 * t;

			double x = 0.5 * (
				(-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t3 +
				(2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * t2 +
				(-p0.x + p2.x) * t +
				(2 * p1.x));

			double y = 0.5 * (
				(-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t3 +
				(2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * t2 +
				(-p0.y + p2.y) * t +
				(2 * p1.y));

			return glm::vec3((float)x, (float)y,0.0f);
		}

		static std::vector<glm::vec3> CatmullRomSmooth(const std::vector<glm::vec3>& controlPoints, int segmentPerInterval = 10)
		{
			std::vector<glm::vec3> result;
			if (controlPoints.size() < 4)
			{
				return result;
			}

			glm::vec3 virtualFirst = glm::vec3(
				2 * controlPoints[0].x - controlPoints[1].x,
				2 * controlPoints[0].y - controlPoints[1].y,
				0.0f
			);

			glm::vec3 virtualLast = glm::vec3(
				2 * controlPoints[controlPoints.size() - 1].x - controlPoints[controlPoints.size() - 2].x,
				2 * controlPoints[controlPoints.size() - 1].y - controlPoints[controlPoints.size() - 2].y,
				0.0f
			);

			std::vector<glm::vec3> extendedPoints = {virtualFirst,controlPoints[0]};
			extendedPoints.insert(extendedPoints.end(), controlPoints.begin(), controlPoints.end());
			extendedPoints.push_back(virtualLast);

			for (int i = 0; i < extendedPoints.size() - 3; i++)
			{
				glm::vec3 p0 = extendedPoints[i];
				glm::vec3 p1 = extendedPoints[i + 1];
				glm::vec3 p2 = extendedPoints[i + 2];
				glm::vec3 p3 = extendedPoints[i + 3];

				for (int j = 0; j <= segmentPerInterval; j++)
				{
					double t = (double)j / segmentPerInterval;
					glm::vec3 interpolatePoint = CatmullRomInterpolate(p0, p1, p2, p3, t);
					result.push_back(interpolatePoint);
				}
			}

			return result;
		}

		typedef std::pair<glm::vec3, glm::vec3> line;

		static double PerpendicularDistance(const glm::vec3& point, line ln)
		{
			double area = abs(0.5 * (ln.first.x * ln.second.y + ln.second.x * point.y + 
										point.x * ln.first.y - ln.second.x * ln.first.y -
										point.x * ln.second.y - ln.first.x * point.y));
			double bottom = sqrt(pow(ln.second.x - ln.first.x, 2));

			return area / bottom * 2;
		}

		static std::vector<glm::vec3> DouglasPeucker(const std::vector<glm::vec3>& points, double epsilon)
		{
			if (points.size() < 3)
				return points;

			double dmax = 0;
			int index = 0;
			line ln = line(points[0], points[1]);

			for (int i = 1; i < points.size() - 1; i++)
			{
				double d = PerpendicularDistance(points[i], ln);
				if (d > dmax)
				{
					index = i;
					dmax = d;
				}
			}

			if (dmax > epsilon)
			{
				std::vector<glm::vec3> result1 = DouglasPeucker(std::vector<glm::vec3>(points.begin(), points.begin() + index + 1), epsilon);
				std::vector<glm::vec3> result2 = DouglasPeucker(std::vector<glm::vec3>(points.begin() + index, points.begin() + points.size() - index), epsilon);

				result1.erase(result1.end()-1);
				result1.insert(result1.begin(), result2.begin(), result2.end());
				return result1;
			}
			else
			{
				return { points[0],points[points.size() - 1] };
			}
		}

		static double NormalizeAngle(double angle)
		{
			angle = angle - ((2*PI) * angle/(2 * PI));
			return angle < 0 ? angle + 2 * PI : angle;
		}

		static GeomDirection GetDirection(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
		{
			float cross = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
			float len1 = glm::distance(p1, p2);
			float len2 = glm::distance(p2, p3);
			float MaxLen = std::max(len1, len2);

			if (cross < 0)
				return GeomDirection::CW;
			if (cross > 0)
				return GeomDirection::CCW;
			if (abs(cross) < epsilon * MaxLen)
				return GeomDirection::AtLine;
		}

		static std::tuple<glm::vec3, float, float, float> GetCircleParamsByThreePoints(const glm::vec3& p1,const glm::vec3 &p2,const glm::vec3& p3)
		{
			glm::vec3 center;
			float angleStart, angleEnd, radius;

			double area = p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y);

			double midX12 = (p1.x + p2.x) / 2.0;
			double midY12 = (p1.y + p2.y) / 2.0;

			double midX23 = (p2.x + p3.x) / 2.0;
			double midY23 = (p2.y + p3.y) / 2.0;

			double slope12 = (p2.y - p1.y) / (p2.x - p1.x);

			double slope23 = (p3.y - p2.y) / (p3.x - p2.x);

			double perpSlope12;
			if (abs(p2.x - p1.x) < 1e-9)
			{
				perpSlope12 = 0;
			}
			else
			{
				perpSlope12 = -1 / slope12;
			}

			double perpSlope23;
			if (abs(p3.x - p2.x) < 1e-9)
			{
				perpSlope23 = 0;
			}
			else
			{
				perpSlope23 = -1 / slope23;
			}

			if (abs(p2.x - p1.x) < 1e-9)
			{
				center.y = (float)midY12;
				center.x = (float)(midX23 + perpSlope23 * (center.y - midY23));
			}
			else if (abs(p3.x - p2.x) < 1e-9)
			{
				center.y = (float)midY23;
				center.x = (float)(midX12 + perpSlope12 * (center.y - midY12));
			}
			else
			{
				center.x = (float)((perpSlope12 * midX12 - midY12 - perpSlope23 * midX23 + midY23) / (perpSlope12 - perpSlope23));
				center.y = (float)(perpSlope12 * (center.x - midX12) + midY12);
			}

			radius = (float)sqrt(pow(p1.x - center.x, 2) + pow(p1.y - center.y, 2));

			double angle1 = atan2(p1.y - center.y, p1.x - center.x);
			double angle2 = atan2(p2.y - center.y, p2.x - center.x);
			double angle3 = atan2(p3.y - center.y, p3.x - center.x);

			angle1 = NormalizeAngle(angle1) * rad2Deg;
			angle2 = NormalizeAngle(angle2) * rad2Deg;
			angle3 = NormalizeAngle(angle3) * rad2Deg;

			GeomDirection dir = GetDirection(p1, p2, p3);

			if (dir == GeomDirection::CCW)
			{
				double maxAngle = std::max(angle1, angle3);
				double minAngle = std::min(angle1, angle3);
				if (angle2 > maxAngle)
				{
					if (angle1 < angle3)
					{
						angle1 = angle1 + 360;
					}
					else if (angle3 < angle1)
					{
						angle3 = angle3 + 360;
					}
				}
				if (angle2 < minAngle)
				{
					if (angle1 > angle3)
					{
						angle1 = angle1 - 360;
					}
					else
					{
						angle3 = angle3 - 360;
					}
				}
				angleStart = (float)std::min(angle1, angle3);
				angleEnd = (float)std::max(angle1, angle3);
			}
			else
			{
				double minAngle = std::min(angle1, angle3);
				double maxAngle = std::max(angle1, angle3);

				if (angle2 < minAngle)
				{
					if (angle1 > angle3)
					{
						angle1 = angle1 - 360;
					}
					else
					{
						angle3 = angle3 - 360;
					}
				}
				if (angle2 > maxAngle)
				{
					if (angle1 < angle3)
					{
						angle1 = angle1 + 360;
					}
					else
					{
						angle3 = angle3 + 360;
					}
				}
				angleStart = (float)std::max(angle1, angle3);
				angleEnd = (float)std::min(angle1, angle3);
			}

			return std::tuple<glm::vec3, float, float, float>(center, angleStart, angleEnd, radius);
		}

		//罗德里格斯旋转公式(三维旋转)
		static glm::vec3 Rotate(const glm::vec3& vector,const glm::vec3& axis,float angle)
		{
			float angleInRad = angle * deg2Rad;
			glm::normalize(axis);
			float cosTheta = (float)cos(angleInRad);
			float sinTheta = (float)sin(angleInRad);

			return vector * cosTheta + glm::cross(axis, vector) * sinTheta + axis * glm::dot(axis, vector) * (1 - cosTheta);
		}

		static glm::vec3 Rotate(const glm::vec3& v, float angle)
		{
			float angleInRad = angle * deg2Rad;
			float cosVal = cos(angleInRad);
			float sinVal = sin(angleInRad);

			float xPrime = v.x * cosVal - v.y * sinVal;
			float yPrime = v.x * sinVal + v.y * cosVal;

			return glm::vec3(xPrime, yPrime,0.0f);
		}

		static float getDeltaT(float d0, float d1, float d2, float a0, float a1, int flag)
		{
			return ((float)(d1 * (1.0f +
				1.5f * d0 * (std::min(PI / 2.0,PI - a0) / (d0 + d1)) +
				1.5f * d2 * (std::min(PI / 2.0,PI - a1) / (d1 + d2)) * flag)));
		}

		static std::vector<float> FoleyParameterize(const std::vector<glm::vec3> &points)
		{
			int n = points.size();
			std::vector<float> y = std::vector<float>(n,0);
			if (n == 2) { y[1] = 1; return y; }

			std::vector<float> dist = std::vector<float>(n,0);
			std::vector<float> alpha = std::vector<float>(n - 1,0);
			for (int i = 0; i < n - 1; ++i)
				dist[i] = std::max(0.01f, glm::distance(points[i + 1], points[i]));
			for (int i = 1; i < n - 1; ++i) {
				float cosvalue = GetCosAngle(points[i - 1] - points[i], points[i + 1] - points[i]);
				alpha[i] = (float)std::min(PI - acos(cosvalue), PI / 2);
			}

			y[1] = (float)(dist[0] * (1 + 1.5 * alpha[1] * dist[1] / (dist[0] + dist[1])));
			for (int i = 2; i < n - 1; ++i)
			{
				y[i] = (float)(y[i - 1] + dist[i - 1] * (1 + 1.5 * alpha[i - 1] * dist[i - 2] / (dist[i - 2] + dist[i - 1]) + 1.5 * alpha[i] * dist[i] / (dist[i - 1] + dist[i])));
			}

			y[n - 1] = (float)(y[n - 2] + dist[n - 2] * (1 + 1.5 * alpha[n - 2] * dist[n - 3] / (dist[n - 3] + dist[n - 2])));

			for (int i = 0; i < y.size(); i++)
			{
				y[i] = y[i] / y[n - 1];
			}

			return y;
		}
	}
}
