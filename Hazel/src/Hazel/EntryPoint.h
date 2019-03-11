#pragma once


#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Core Logger! Set level to 'warn'");
	int a = 5;
	HZ_INFO("Initialized Client Logger! Set level to 'info' Var={0}", a);


	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;

}

#endif // HZ_PLATFORM_WINDOWS


