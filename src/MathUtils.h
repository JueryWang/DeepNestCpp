#pragma once
#include "../lib/OGL/glm/glm.hpp"
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_offset_polygons_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <vector>
#include <cmath>

constexpr double PI = 3.14159265358979323846;
constexpr double deg2Rad = PI / 180.0f;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polygon_2<Kernel> Polygon;
typedef Kernel::FT FT;
typedef Kernel::Point_2 Point;
typedef Kernel::Point_2 Point;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel> PolygonWithHoles;
typedef std::shared_ptr<PolygonWithHoles> PolygonWithHolesPtr;
typedef std::vector<PolygonWithHolesPtr> PolygonWithHolesPtrVector;

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

		static bool IsPolygonInside(const Polygon& inner, const Polygon& outer) {
			for (auto v : inner.vertices()) {
				auto result = CGAL::bounded_side_2(outer.vertices_begin(), outer.vertices_end(), v);
				if (result == CGAL::ON_UNBOUNDED_SIDE) {
					return false;
				}
			}

			for (auto e : inner.edges())
			{
				Point mid = CGAL::midpoint(e.source(), e.target());
				auto result = CGAL::bounded_side_2(outer.vertices_begin(), outer.vertices_end(), mid);
				if (result == CGAL::ON_UNBOUNDED_SIDE)
				{
					return false;
				}
			}

			return true;
		}

		//static void PathOffset()
		//{

		//}

		//static void RemoveHolesExplicit()
		//{

		//}

		static std::vector<glm::vec3> AndrewConvex(const std::vector<glm::vec3> pointset)
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
		}
	}
}
