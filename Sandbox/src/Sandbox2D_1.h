#pragma once

#include "Hazel.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Events/Event.h"
#include <memory>



class Quad
{
public:
	Quad() {}
	Quad(glm::vec3 position, glm::vec2 size, glm::vec4 color)
		: size(size), position(position), color(color)
	{
	}
	void Draw() { Hazel::Renderer2D::DrawQuad({ position.x, position.y, position.z }, { size.x, size.y }, { color.r, color.g, color.b, color.a }); }
	void OnEvent(Hazel::Event& event)  {}
	glm::vec3 position;
	glm::vec2 size;
	glm::vec4 color;
	glm::vec2 direction;
	glm::vec3 speed;
};

class Hazel::Application;

class Sandbox2D_1 : public Hazel::Layer
{
public:
	Sandbox2D_1();
	Sandbox2D_1(Hazel::Application* app, const std::string& name = "Layer");
	~Sandbox2D_1() = default;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Hazel::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event & event) override;
	bool OnMouseButtonPressed(Hazel::MouseButtonPressedEvent & event);
	bool OnKeyPressedEvent(Hazel::KeyPressedEvent & event);

private:
	std::shared_ptr<Hazel::Application> app;
	Hazel::OrthographicCameraController m_CameraController;

	// Temp
 	glm::vec4 m_ClearColor = { 0.094f, 0.141f, 0.176f, 1.0f };
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_TintColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	float m_TilingFactor = 20.0f;
	float m_Rotation1 = 0.0f;
	float m_AZDepth = -0.02f;
	float m_RZDepth = -0.01f;
	float m_WindowDim[2];
	float m_WindowWidth;
	float m_WindowHeight;

	Quad m_Quad;
	int m_ClickState = 0;
};