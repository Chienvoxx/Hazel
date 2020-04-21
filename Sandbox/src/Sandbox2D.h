#pragma once

#include "Hazel.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"



class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() = default;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Hazel::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& event) override;
	bool OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& event);
	bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event);

private:
	Hazel::OrthographicCameraController m_CameraController;

	// Temp
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_TintColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	float m_TilingFactor = 20.0f;
	float m_Rotation1 = 0.0f;
	float m_AZDepth = -0.02f;
	float m_RZDepth = -0.01f;
};