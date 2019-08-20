#include <Hazel.h>
#include "Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include "ExampleLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class TestLayer : public Hazel::Layer
{
public:
	TestLayer()
		: Layer("Test"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
	{
		m_TriangleVA.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.3f, 0.7f, 1.3f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.7f, 0.2f, 1.8f, 1.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> triangleVB;
		triangleVB.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};
		triangleVB->SetLayout(layout);
		m_TriangleVA->AddVertexBuffer(triangleVB);


		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazel::IndexBuffer> triangleIB;
		triangleIB.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_TriangleVA->SetIndexBuffer(triangleIB);

		m_SqaureVA.reset(Hazel::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,	0.5f, 0.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			});
		m_SqaureVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		m_SqaureVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 460 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				// color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_TriangleShader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatColorVertexSrc = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorFragmentSrc = R"(
			#version 460 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				//color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = vec4(u_Color, 1.0);
			}
		)";

		m_flatColorShader.reset(Hazel::Shader::Create(flatColorVertexSrc, flatColorFragmentSrc));
	}


	void OnUpdate(Hazel::Timestep ts) override
	{

		if (Hazel::Input::IsKeyPressed(HZ_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_SquarePosition.x -= m_SquareSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_SquarePosition.x += m_SquareSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
			m_SquarePosition.y += m_SquareSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
			m_SquarePosition.y -= m_SquareSpeed * ts;

		glm::mat4 transform = glm::translate(glm::mat4(1.0), m_SquarePosition);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPostion({ m_CameraPosition });
		m_Camera.SetRotation(m_CameraRotation);



		Hazel::Renderer::BeginScene(m_Camera);

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_flatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		int c = 20;
		for (int x = 0; x < c; x++)
		{
			for (int y = 0; y < c; y++)
			{
				glm::vec3 pos(-((c/2)*0.11f) + (x * 0.11f), -((c/2) * 0.11f) + (y * 0.11f), 0.0f);
				glm::mat4 tran = glm::translate(glm::mat4(1.0), pos) * scale;
				Hazel::Renderer::Submit(m_flatColorShader, m_SqaureVA, tran);
			}
		}
		Hazel::Renderer::Submit(m_TriangleShader, m_TriangleVA, transform);

		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender()
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
	}


private:
	std::shared_ptr<Hazel::Shader> m_TriangleShader;
	std::shared_ptr<Hazel::VertexArray> m_TriangleVA;


	std::shared_ptr<Hazel::Shader> m_flatColorShader;
	std::shared_ptr<Hazel::VertexArray> m_SqaureVA;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquarePosition;
	float m_SquareSpeed = 1.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
	glm::vec3 m_RedColor = { 0.8f, 0.3f, 0.2f};
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new TestLayer());
	}

	~Sandbox()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}