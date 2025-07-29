#pragma once

#include "../lib/dxflib/include/dl_dxf.h"
#include "../lib/dxflib/include/dl_creationadapter.h"
#include <memory>
#include <string>

namespace DeepNestCpp
{
	class Sketch;

	class DXFProcessor
	{
	public:
		DXFProcessor(std::shared_ptr<Sketch> sketch);
		~DXFProcessor();

		int read(const std::string& dxfFile);
	private:
		std::weak_ptr<Sketch> psketch;
	};
}