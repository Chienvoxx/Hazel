#include "Sandbox2D.h"
#include "Hazel/Renderer/Renderer.h"
#include "ImGui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include "Hazel/Debug/Instrumentor.h"


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, false)
{

}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	this->m_DebugName = "Sandbox2d_Layer";

// 	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/checkerboard.png");
	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/checkerboard_plain.png");
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();
	// Update
	{
		m_CameraController.OnUpdate(ts);
	}


	// Render
	Hazel::Renderer2D::ResetStats();
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();
	}
	

	{
		static float rotation = 0.0f;
		rotation += -1.1f;

		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.5f, 0.0f }, { 0.25f, 1.0f }, m_SquareColor);
		Hazel::Renderer2D::DrawQuad({ 1.0f, -0.5f, 0.0f }, { 1.0f, 0.25f }, m_SquareColor * 1.25f);
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 1.0f, 0.0f }, { 1.0f, 0.25f }, m_Rotation1, { 0.2f, 0.3f, 0.8f, 1.0f });
 		Hazel::Renderer2D::DrawQuad(		{ 0.0f, 0.0f, m_AZDepth },	{ 100.0f, 100.0f },			m_CheckerboardTexture,	m_TilingFactor,	m_TintColor);
 		Hazel::Renderer2D::DrawRotatedQuad(	{ -6.0f, 0.0f, m_RZDepth }, { 5.0f, 5.0f }, rotation,	m_CheckerboardTexture,	5.0f,			m_TintColor);
// 		Hazel::Renderer2D::EndScene();
// 
// 		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -4.75f; y < 5.25f; y += 0.5f)
		{
			for (float x = -4.75f; x < 5.25f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Hazel::Renderer2D::DrawQuad({ x, y, 0.0f }, { 0.45f, 0.45f }, color);
			}
		}
		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
 	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
 	ImGui::ColorEdit4("Tint Color", glm::value_ptr(m_TintColor));
	ImGui::DragFloat("Tiling Factor", &m_TilingFactor, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat("Rotation1", &m_Rotation1, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat("Aligned Z Depth", &m_AZDepth, 0.001f, -0.9999f, 1.0f);
	ImGui::DragFloat("Rotated Z Depth", &m_RZDepth, 0.001f, -0.9999f, 1.0f);

	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

 
//  	for (auto& result : m_ProfileResults)
//  	{
//  		char label[50];
//  		strcpy(label, "%.3fms ");
//  		strcat(label, result.Name);
//  		ImGui::Text(label, result.Time);
//  	}
// 	m_ProfileResults.clear();
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);

	Hazel::EventDispatcher dispatcher1(event);
	dispatcher1.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(Sandbox2D::OnKeyPressedEvent));



 	if (!event.Handled && (event.GetEventType() == Hazel::EventType::MouseButtonPressed))
 	{
		Hazel::EventDispatcher dispatcher2(event);
  		dispatcher2.Dispatch<Hazel::MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(Sandbox2D::OnMouseButtonPressed));
 	}
}

bool Sandbox2D::OnKeyPressedEvent(Hazel::KeyPressedEvent& event)
{
	return false;
}

bool Sandbox2D::OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == HZ_MOUSE_BUTTON_LEFT)
	{
		// m_RZDepth += 0.01f;
	}
	if (e.GetMouseButton() == HZ_MOUSE_BUTTON_RIGHT)
	{
		// m_RZDepth -= 0.01f;
	}
	return true;
}
