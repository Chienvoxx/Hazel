#include "Sandbox2D_1.h"

#include "Sandbox2D.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "ImGui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include "Hazel/Debug/Instrumentor.h"

Sandbox2D_1::Sandbox2D_1()
	:
	Layer("Sandbox2D_1"),
	m_CameraController(1280.0f / 720.0f, false)
{
	m_Quad = Quad({ 0.0f, -5.0f, 0.0f }, { 0.1f, 0.5f }, { 0.4f, 0.6f, 0.8f, 1.0f });
}

Sandbox2D_1::Sandbox2D_1(Hazel::Application* app, const std::string& name /*= "Layer"*/)
	:
	Layer(app, "Sandbox2D_1"),
	m_CameraController(1280.0f / 720.0f, false)
{
	m_Quad = Quad({ 0.0f, -5.0f, 0.0f }, { 0.1f, 0.5f }, { 0.4f, 0.6f, 0.8f, 1.0f });
}

void Sandbox2D_1::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	this->m_DebugName = "Sandbox2d_Layer";

	// Initialize Timestep
	auto ts = Hazel::Timestep();
	ts.GetMilliseconds();
	m_WindowDim[0] = m_WindowWidth;
	m_WindowDim[1] = m_WindowHeight;
}

void Sandbox2D_1::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D_1::OnUpdate(Hazel::Timestep ts)
{
	m_WindowDim[0] = this->m_pApp->GetWindow().GetWidth();
	m_WindowDim[1] = this->m_pApp->GetWindow().GetHeight();
	HZ_PROFILE_FUNCTION();
	// Update
	{
		
		m_CameraController.OnUpdate(ts);
		float dx = ts.GetMilliseconds();
		if (dx < 200)
		{
			float newX = m_Quad.position.x;
			switch (m_ClickState)
			{
			case 0:
				break;
			case 1:
				m_Quad.position.x += (dx * 0.005f);
				break;
			case 2:
				m_Quad.position.x -= (dx * 0.005f);
				break;
			default:
				break;
			}
		}
	}

	// Render
	Hazel::Renderer2D::ResetStats();
	{
		HZ_PROFILE_SCOPE("Renderer Prep")
		Hazel::RenderCommand::SetClearColor(m_ClearColor);
		Hazel::RenderCommand::Clear();
	}
	
	{
		static float rotation = 0.0f;
		rotation += -1.1f;

		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		m_Quad.Draw();
		Hazel::Renderer2D::DrawQuad({ 0.0f, -5.0f, 0.0f }, { 10.0f, 0.25f }, m_SquareColor);
		Hazel::Renderer2D::DrawQuad({ 1.0f, -0.5f, 0.0f }, { 1.0f, 0.25f }, m_SquareColor * 1.25f);
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 1.0f, 0.0f }, { 1.0f, 0.25f }, m_Rotation1, { 0.2f, 0.3f, 0.8f, 1.0f });

		Hazel::Renderer2D::EndScene();

	}
}

void Sandbox2D_1::OnImGuiRender()
{
	ImGui::Begin("Sandbox2D_1");
	ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
	ImGui::DragFloat("Rotation", &m_Rotation1, 0.001f, 0.0f, 0.0f, "%.3f", 1.0f);
	ImGui::DragFloat2("Window Size", m_WindowDim);
// 	ImGui::ShowDemoWindow(&demoWindow);
// 	ImGui::ShowUserGuide();
// 	ImGui::ShowMetricsWindow(&metricsWindow);
	ImGui::End();
}

void Sandbox2D_1::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);

	if (!event.Handled && (event.GetEventType() == Hazel::EventType::MouseButtonPressed))
	{
		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(Sandbox2D_1::OnMouseButtonPressed));
	}
}

bool Sandbox2D_1::OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == HZ_MOUSE_BUTTON_LEFT)
	{
		m_ClickState = ++m_ClickState % 3;
	}
	return false;
}

bool Sandbox2D_1::OnKeyPressedEvent(Hazel::KeyPressedEvent& event)
{
	return false;
}
