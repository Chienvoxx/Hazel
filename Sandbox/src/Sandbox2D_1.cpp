#include "Sandbox2D_1.h"

#include "Sandbox2D.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "ImGui/imgui.h"
#include "Hazel/Core/Input.h"
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
	m_Quad = Quad({ 0.0f, -5.0f, 0.0f }, { 0.5f, 0.5f }, { 0.4f, 0.6f, 0.8f, 1.0f });
}

Sandbox2D_1::Sandbox2D_1(Hazel::Application* app, const std::string& name /*= "Layer"*/)
	:
	Layer(app, "Sandbox2D_1"),
	m_CameraController(1280.0f / 720.0f, false)
{
	m_Quad = Quad({ 0.0f, -5.0f, 0.0f }, { 0.5f, 0.5f }, { 0.4f, 0.6f, 0.8f, 1.0f });
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


	/*	
	 *	Walls -- bake position, bake size
	 *	vector<Quad> mStaticQuads
	 *		Left		Position(mLeftPos),		Size(mWallWidth * mSceneHeight)
	 *		Right		Position(mRightPos),	Size(mWallWidth * mSceneHeight)
	 *		Top			Position(mTopPos),		Size(mWallWidth * mSceneWidth)
	 *		Bottom		Position(mBottomPos),	Size(mWallWidth * mScneWidth)
	 *		
	 *	Paddle -- dynamic position, dynamic size, receive input
	 *	Quad mPaddle
	 *					Position(mStartPaddlePos),	Size()
	 *					
	 *	Pong -- dynamic position, bake size
	 *	Quad mPong
	 *					Position(mStartPongPos),		Size()
	 *
	 *	Transform vertex positions before sending to renderer
		
		Quad mPong;
		Quad mPaddle;
		std::vector<Quad> mStaticQuads;

		float mWallWidth			= 1.0f;
		float mSceneHeight			= 15.0f;
		float mSceneWidth			= 20.0f;
		float mLeftPos				= -(mSceneWidth / 2);
		float mRightPos				=  (mSceneWidth / 2);
		float mTopPos				= -(mSceneHeight / 2);
		float mBottomPos			=  (mSceneHeight / 2);
		float mPongStartPos			= 0.0f;
		float mPongSize				= 0.25f;
		float mPaddleStartPos		= -(mSceneWidth / 2) + 0.5f;
		float mPaddleWidth			= 0.25f;
		float mPaddleHeight			= 2.0f;
		glm::vec2 mPaddleMotionAxis	= { 1.0f, 0.0 };
	 **/

	mTopWall	= Quad({ 0.0f,  mSceneHeight / 2, 0.0f }, { mSceneWidth, mWallWidth },   mWallColor);
	mBottomWall = Quad({ 0.0f, -mSceneHeight / 2, 0.0f }, { mSceneWidth, mWallWidth },   mWallColor);
	mLeftWall	= Quad({ -mSceneWidth / 2, 0.0f, 0.0f }, { mWallWidth,  mSceneHeight }, mWallColor);
	mRightWall	= Quad({  mSceneWidth / 2, 0.0f, 0.0f}, { mWallWidth,  mSceneHeight }, mWallColor);

}

void Sandbox2D_1::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D_1::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	m_Quad.m_ScalarTranslationSpeed = m_QuadScalarSpeed;
	if (m_ToggleCameraMovement)
	{
		m_CameraController.OnUpdate(ts);
	}
	else
	{
		m_Quad.OnUpdate(ts);
	}

	m_Quad.UpdateAABB();

	m_WindowDim[0] = this->m_pApp->GetWindow().GetWidth();
	m_WindowDim[1] = this->m_pApp->GetWindow().GetHeight();
	// Update
	// For now we'll update some properties each frame
	mWallColor = { m_ClearColor.r * mCCE, m_ClearColor.g * mCCE , m_ClearColor.b * mCCE , 1.0f };
	glm::vec4 WallColor2 = glm::vec4(m_ClearColor.r * mCCE, m_ClearColor.g * mCCE, m_ClearColor.b * mCCE, 0.5f);
	mTopWall.position = { 0.0f, mSceneHeight / 2, 0.0f };
	mTopWall.size = { mSceneWidth + mWallWidth, mWallWidth };
	mTopWall.color = mWallColor;
	mTopWall.UpdateAABB();

	mBottomWall.position = { 0.0f, -mSceneHeight / 2, 0.0f };
	mBottomWall.size = { mSceneWidth + mWallWidth, mWallWidth };
	mBottomWall.color = mWallColor;
	mBottomWall.UpdateAABB();

	mLeftWall.position = { -mSceneWidth / 2, 0.0f, 0.0f };
	mLeftWall.size = { mWallWidth, mSceneHeight - mWallWidth };
	mLeftWall.color = WallColor2;
	mLeftWall.UpdateAABB();

	mRightWall.position = { mSceneWidth / 2, 0.0f, 0.0f };
	mRightWall.size = { mWallWidth, mSceneHeight - mWallWidth };
	mRightWall.color = WallColor2;
	mRightWall.UpdateAABB();

	// Collision
	// Super naive 0.0
	
	// Right wall
	if (m_Quad.aabb.right >= mRightWall.aabb.left)
	{
		m_Quad.position.x = mRightWall.aabb.left - (m_Quad.size.x/2);
		m_Quad.UpdateAABB();
	}
	// Left wall
	if (m_Quad.aabb.left <= mLeftWall.aabb.right)
	{
		m_Quad.position.x = mLeftWall.aabb.right + (m_Quad.size.x / 2);
		m_Quad.UpdateAABB();
	}
	// Top wall
	if (m_Quad.aabb.top >= mTopWall.aabb.bottom)
	{
		m_Quad.position.y = mTopWall.aabb.bottom - (m_Quad.size.y / 2);
		m_Quad.UpdateAABB();
	}
	// Bottom wall
	if (m_Quad.aabb.bottom <= mBottomWall.aabb.top)
	{
		m_Quad.position.y = mBottomWall.aabb.top + (m_Quad.size.y / 2);
		m_Quad.UpdateAABB();
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
// 		Hazel::Renderer2D::DrawQuad({ 0.0f, -5.0f, 0.0f }, { 10.0f, 0.25f }, m_SquareColor);
// 		Hazel::Renderer2D::DrawQuad({ 1.0f, -0.5f, 0.0f }, { 1.0f, 0.25f }, m_SquareColor * 1.25f);
// 		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 1.0f, 0.0f }, { 1.0f, 0.25f }, m_Rotation1, { 0.2f, 0.3f, 0.8f, 1.0f });
		Hazel::Renderer2D::DrawQuad(mTopWall.position, mTopWall.size, mTopWall.color);
		Hazel::Renderer2D::DrawQuad(mLeftWall.position, mLeftWall.size, mLeftWall.color);
		Hazel::Renderer2D::DrawQuad(mBottomWall.position, mBottomWall.size, mBottomWall.color);
		Hazel::Renderer2D::DrawQuad(mRightWall.position, mRightWall.size, mRightWall.color);

		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D_1::OnImGuiRender()
{
	ImGui::Begin("Sandbox2D_1");
	ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
	ImGui::DragFloat("Rotation", &m_Rotation1, 0.001f, 0.0f, 0.0f, "%.3f", 1.0f);
	ImGui::DragFloat2("Window Size", m_WindowDim);
	if (ImGui::Button(m_ToggleCameraMovement ? "Camera" : "Pong"))
		m_ToggleCameraMovement = !m_ToggleCameraMovement;
	ImGui::DragFloat("Quad Scalar Speed", &m_QuadScalarSpeed, 0.01f, 0.0f, 20.0f, "%.2f", 1.0f);
	ImGui::Spacing();
	ImGui::DragFloat("mSceneHeight",		&mSceneHeight,		0.1f,	0.001f,	50.0f, "%.2f", 1.0f);
	ImGui::DragFloat("mSceneWidth",			&mSceneWidth,		0.1f,	0.001f,	50.0f, "%.2f", 1.0f);
	ImGui::DragFloat("mWallWidth",			&mWallWidth,		0.001f,	0.001f,	5.0f, "%.2f", 1.0f);
	ImGui::DragFloat("mLeftWallPos",		&mLeftWallPos,		0.01f,	0.001f,	50.0f, "%.2f", 1.0f);
	ImGui::DragFloat("mRightWallPos",		&mRightWallPos,		0.01f,	0.001f,	50.0f, "%.2f", 1.0f);
	ImGui::DragFloat("mTopWallPos",			&mTopWallPos,		0.01f,	0.001f,	50.0f, "%.2f", 1.0f);
	ImGui::DragFloat("mBottomWallPos",		&mBottomWallPos,	0.01f,	0.001f,	50.0f, "%.2f", 1.0f);
	ImGui::DragFloat("mPongStartPos",		&mPongStartPos,		0.01f,	0.001f,	50.0f, "%.2f", 1.0f);
	ImGui::DragFloat("mPongSize",			&mPongSize,			0.01f,	0.001f,	5.0f, "%.2f", 1.0f);
	ImGui::DragFloat("mPaddleStartPos",		&mPaddleStartPos,	0.01f,	0.001f,	50.0f, "%.2f", 1.0f);
	ImGui::DragFloat("mPaddleWidth",		&mPaddleWidth,		0.01f,	0.001f,	5.0f, "%.2f", 1.0f);
	ImGui::DragFloat("mPaddleHeight",		&mPaddleHeight,		0.01f,	0.001f,	50.0f, "%.2f", 1.0f);
	ImGui::DragFloat2("mPaddleMotionAxis",	glm::value_ptr(mPaddleMotionAxis));
	ImGui::DragFloat("mCCE",				&mCCE,				0.1f,	0.0f,	10.0f, "%.2f", 1.0f);

	ImGui::End();
}

void Sandbox2D_1::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
	if (!e.Handled && (e.GetEventType() == Hazel::EventType::KeyPressed))
	{
		Hazel::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(Sandbox2D_1::OnKeyPressedEvent));
	}
	if (!e.Handled && (e.GetEventType() == Hazel::EventType::MouseButtonPressed))
	{
		Hazel::EventDispatcher dispatcher(e);
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

bool Sandbox2D_1::OnKeyPressedEvent(Hazel::KeyPressedEvent& e)
{
	return false;
}


void Quad::OnUpdate(Hazel::Timestep ts)
{
	if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
		position.y += m_ScalarTranslationSpeed * ts;
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
		position.y -= m_ScalarTranslationSpeed * ts;
	if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		position.x -= m_ScalarTranslationSpeed * ts;
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		position.x += m_ScalarTranslationSpeed * ts;
}

void Quad::UpdateAABB()
{
	aabb.top	= (position.y + (size.y / 2));
	aabb.bottom = (position.y - (size.y / 2));
	aabb.left	= (position.x - (size.x / 2));
	aabb.right	= (position.x + (size.x / 2));
}
