#pragma once


class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//if(Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
		//HZ_INFO("ExampleLayer::OnUpdate::Tab Key Pressed!");
	}

	virtual void OnImGuiRender()
	{
		//ImGui::Begin("Test");
		//ImGui::Text("Hello from Test!!");
		//ImGui::End();

		//ImGui::Begin("Programming");
		//ImGui::Text("Hello from Programming!!");
		//ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		//if (event.GetEventType() == Hazel::EventType::KeyPressed)
		//{
		//	Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
		//	if (e.GetKeyCode() == HZ_KEY_TAB)
		//		HZ_INFO("ExampleLayer::OnUpdate::Tab Key Pressed!");
		//	HZ_TRACE("-->{0}", (char)e.GetKeyCode());

		//}

		//Hazel::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(TestLayer::OnKeyPressedEvent));
	}

	//bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event)
	//{
	//	return false;
	//}

};