#pragma once
#include "../OGL/glm/glm.hpp"

namespace DeepNestCpp
{
	enum class GeomDirection
	{
		CW,
		CCW,
		AtLine
	};

	enum LeadType
	{
		None,
		Linear,
		Arc,
	};

	struct LeadLineParams
	{
		LeadType leadType;
		float length;
		int angle;
	};

	struct LeadLine
	{
		glm::vec3 start;
		glm::vec3 end;

		LeadType tRule;
		float length;
		int angle;
		int radius;
	};
}