#include "hzpch.h"
#include "Layer.h"

namespace Hazel {

	Layer::Layer(const std::string& debugName)
		: m_pApp(nullptr), m_DebugName(debugName)
	{
	}

	Layer::Layer(Application* app, const std::string& debugName)
		:m_pApp(app), m_DebugName(debugName)
	{
	}

	Layer::~Layer()
	{
	}
}


