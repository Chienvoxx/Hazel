#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"


namespace Hazel {

	class Application;

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
 		Layer(Application* app, const std::string& name = "Layer");

		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
 		Application* m_pApp;
	};
}