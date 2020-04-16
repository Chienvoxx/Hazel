#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>
#include "Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include "ExampleLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"


class TestLayer : public Hazel::Layer
{
public:
	TestLayer()
		: Layer("Test"), m_CameraController(1280.0f / 720.0f, false), m_SquarePosition(0.0f)
	{
		this->m_DebugName = "TestLayer";

		m_TriangleVA = Hazel::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.3f, 0.7f, 1.3f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.7f, 0.2f, 1.8f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> triangleVB;
		triangleVB.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};
		triangleVB->SetLayout(layout);
		m_TriangleVA->AddVertexBuffer(triangleVB);


		uint32_t indices[3] = { 0, 1, 2 };
		Hazel::Ref<Hazel::IndexBuffer> triangleIB;
		triangleIB.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_TriangleVA->SetIndexBuffer(triangleIB);

		m_SqaureVA = Hazel::VertexArray::Create();

		//float squareVertices[3 * 4] = {
		//	-0.5f, -0.5f, 0.0f,
		//	 0.5f, -0.5f, 0.0f,
		//	 0.5f,  0.5f, 0.0f,
		//	-0.5f,	0.5f, 0.0f
		//};

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,	0.5f, 0.0f, 0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SqaureVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
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

		m_TriangleShader = Hazel::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

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

		m_FlatColorShader = Hazel::Shader::Create("FlatColorShader", flatColorVertexSrc, flatColorFragmentSrc);
		m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");

		std::string textureShaderVertexSrc = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 460 core

			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

// 		m_TextureShader.reset(Hazel::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Hazel::Texture2D::Create("assets/textures/checkerboard.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

	}


	void OnUpdate(Hazel::Timestep ts) override
	{

// 		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
// 			m_SquarePosition.x -= m_SquareSpeed * ts;
// 		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
// 			m_SquarePosition.x += m_SquareSpeed * ts;
// 
// 		if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
// 			m_SquarePosition.y += m_SquareSpeed * ts;
// 		else if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
// 			m_SquarePosition.y -= m_SquareSpeed * ts;

		// Update
		m_CameraController.OnUpdate(ts);


		// Render
		glm::mat4 transform = glm::translate(glm::mat4(1.0), m_SquarePosition);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();


		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

// 		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
// 		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		auto flatColorShader = m_ShaderLibrary.Get("FlatColor");
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(flatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		
		int c = 20;
		for (int x = 0; x < c; x++)
		{
			for (int y = 0; y < c; y++)
			{
				glm::vec3 pos(-((c/2)*0.11f) + (x * 0.11f), -((c/2) * 0.11f) + (y * 0.11f), 0.0f);
				glm::mat4 tran = glm::translate(glm::mat4(1.0), pos) * scale;
// 				Hazel::Renderer::Submit(m_FlatColorShader, m_SqaureVA, tran);
				Hazel::Renderer::Submit(flatColorShader, m_SqaureVA, tran);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();

		Hazel::Renderer::Submit(textureShader, m_SqaureVA, glm::scale(glm::mat4(1.0f), m_Scale));
		Hazel::Renderer::Submit(m_TriangleShader, m_TriangleVA, transform);

		Hazel::Renderer::EndScene();
	}


	virtual void OnImGuiRender()
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::DragFloat("global scale", &io.FontGlobalScale, 0.005f, 1.0f, 3.0f, "%.2f");      // scale everything
		ImGui::End();

		ImGui::Begin("m_Scale");
		ImGui::InputFloat3("Texture Square Size", glm::value_ptr(m_Scale));
		ImGui::End();
	}


	void OnEvent(Hazel::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}


private:
	Hazel::ShaderLibrary m_ShaderLibrary;
	Hazel::Ref<Hazel::Shader> m_TriangleShader;
	Hazel::Ref<Hazel::VertexArray> m_TriangleVA;


	Hazel::Ref<Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::VertexArray> m_SqaureVA;

	Hazel::Ref<Hazel::Texture2D> m_Texture;

	Hazel::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition;
	float m_SquareSpeed = 1.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
	glm::vec3 m_RedColor = { 0.8f, 0.3f, 0.2f};

	glm::vec3 m_Scale = { 1.5f , 1.5f , 1.5f };
};



class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
// 		PushLayer(new ExampleLayer());
// 		PushLayer(new TestLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}