#pragma once

#include "hzpch.h"
#include "ImGuiLayer.h"

//#include "imgui.h"
//#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Hazel/ImGui/ImGuiBuild.cpp"


// Temporary - Hack
//#include <GLFW/glfw3.h>
//#include <glad/glad.h>


//#include "examples/imgui_impl_opengl3.cpp"
//#include "examples/imgui_impl_glfw.cpp"

#include "Hazel/Application.h"

namespace Hazel {

	ImGuiLayer::ImGuiLayer()
		: Layer ("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows <--- Causing unhandled exception!! Commnent out one line or the other stops the crash
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		io.FontGlobalScale = 2.0f;									// See example in imgui_demo.cpp::ShowStyleEditor(&style); line 3375

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);  // Enable Multi-Viewport / Platform Windows <--- Causing unhandled exception!! Commnent out one line or the other stops the crash
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		//glfwDestroyWindow(window);
		//glfwTerminate();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool p_open = true;
		ImGui::ShowDemoWindow(&p_open);
		//ImGui::ShowUserGuide();
		//ImGui::ShowMetricsWindow(&show);
		//ImGui::ShowFontSelector();

		ImGuiStyle  style = ImGui::GetStyle();
		ImGui::ShowStyleEditor(&style);
	}

	void ImGuiLayer::begin()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}



	void ImGuiLayer::end()
	{
		// Tell ImGui what size the window is
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());


		// Rendering
		ImGui::Render();
		ImGui::GetDrawData();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}