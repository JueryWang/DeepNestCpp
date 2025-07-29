#pragma once
#include <vector>
#include <string>
#include <map>
#include "DrawEntity.h"
#include "Canvas.h"
#include "../lib/dxflib/include/dl_entities.h"

namespace DeepNestCpp
{
	class Canvas;

	class Sketch
	{
		friend class OCS;
		friend class Canvas;
	public:
		Sketch();
		~Sketch();

		void AddEntity(Entity* e);
		void EraseEntity(Entity* e);
		void ClearEntities();
		void UpdateSketch();
		void SplitPart();
		void SimplifyGeometry(float delta);
		void SmoothGeometry();
		void GenEnvolop(int expandValue, int smoothValue);
		void ToNcProgram(std::string file);

	private:

		EntityType entType;
		Canvas* mainCanvas;
		std::map<Canvas*,EntCompound*> partPreview;
		std::vector<EntCompound*> parts;
		std::vector<Entity*> entities;
		std::vector<Polyline2D*> envolopProfile;

		
	};
}
