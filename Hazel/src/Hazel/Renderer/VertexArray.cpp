#pragma once

#include "hzpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"



namespace Hazel {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::none:		HZ_CORE_ASSERT(false, "RendererAPI::none is currently not supported."); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}