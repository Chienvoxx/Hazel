#pragma once

#include "Hazel.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Events/Event.h"
#include <memory>


struct AABB
{
	float top;
	float bottom;
	float left;
	float right;
};

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
	void OnUpdate(Hazel::Timestep ts);
	void UpdateAABB();
	AABB aabb;
	glm::vec3 position;
	glm::vec2 size;
	glm::vec4 color;
	glm::vec2 direction;
	glm::vec3 speed;
	float m_ScalarTranslationSpeed = 0.1f;
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
	bool m_ToggleCameraMovement = false;

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

	int m_ClickState = 0;
	float m_QuadScalarSpeed = 20.0f;
	Quad m_Quad;

	Quad mTopWall;
	Quad mBottomWall;
	Quad mLeftWall;
	Quad mRightWall;
	Quad mPong;
	Quad mPaddle;
	std::vector<Quad> mStaticQuads;

	float mSceneHeight			= 15.0f;
	float mSceneWidth			= 20.0f;
	float mWallWidth			= 0.1f;
	float mLeftWallPos			= -(mSceneWidth / 2);
	float mRightWallPos			=  (mSceneWidth / 2);
	float mTopWallPos			= -(mSceneHeight / 2);
	float mBottomWallPos		=  (mSceneHeight / 2);
	float mPongStartPos			= 0.0f;
	float mPongSize				= 0.25f;
	float mPaddleStartPos		= -(mSceneWidth / 2) + 0.5f;
	float mPaddleWidth			= 0.25f;
	float mPaddleHeight			= 2.0f;
	glm::vec2 mPaddleMotionAxis	= { 1.0f, 0.0 };
	float mCCE					= 2.0f;		// scale clear color this amount for wall color
	glm::vec4 mWallColor		= { m_ClearColor.r * mCCE, m_ClearColor.g * mCCE , m_ClearColor.b * mCCE , 1.0f};
};