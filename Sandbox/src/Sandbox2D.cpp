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
	this->m_DebugName = "Sandbox2d_Layer";

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();
	// Update
	{
		HZ_PROFILE_SCOPE("Update");
		m_CameraController.OnUpdate(ts);
	}


	// Render
	{
		HZ_PROFILE_SCOPE("Render");
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();
	}
	

	{
		HZ_PROFILE_FUNCTION();
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 0.25f, 1.0f }, m_SquareColor);
		Hazel::Renderer2D::DrawQuad({ 1.0f, -0.5f, 0.0f }, { 1.0f, 0.25f }, m_SquareColor * 1.25f);
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.5f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
 	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
 
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

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
