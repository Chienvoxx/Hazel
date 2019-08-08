#include "hzpch.h"
#include "RenderCommand.h"


namespace Hazel {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
