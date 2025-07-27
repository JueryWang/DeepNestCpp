#include <algorithm>
#include "Sketch.h"
#include "MathUtils.h"
#include "DrawEntity.h"
#include "Canvas.h"

DeepNestCpp::Sketch::Sketch()
{

}

DeepNestCpp::Sketch::~Sketch()
{
}

void DeepNestCpp::Sketch::AddEntity(Entity* e)
{
	entities.push_back(e);
}

void DeepNestCpp::Sketch::EraseEntity(Entity* e)
{
	auto find = std::find(entities.begin(), entities.end(), e);
	if (find != entities.end())
	{
		entities.erase(find);
	}
}

void DeepNestCpp::Sketch::ClearEntities()
{
	entities.clear();
	parts.clear();
	partPreview.clear();
	envolopProfile.clear();
}

void DeepNestCpp::Sketch::UpdateSketch()
{
	mainCanvas->entities = this->entities;
	mainCanvas->envolopCurve = envolopProfile;
	mainCanvas->UpdateOCS();
	mainCanvas->Paint();
}

void DeepNestCpp::Sketch::SplitPart()
{
}

void DeepNestCpp::Sketch::SimplifyGeometry(float delta)
{
}

void DeepNestCpp::Sketch::SmoothGeometry()
{
}

void DeepNestCpp::Sketch::GenEnvolop(int expandValue, int smoothValue)
{
}

void DeepNestCpp::Sketch::ToNcProgram(std::string file)
{
}
