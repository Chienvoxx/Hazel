// Basic flat color shader

#type vertex
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


#type fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_Position;

uniform vec3 u_Color;

void main()
{
	//color = vec4(v_Position * 0.5 + 0.5, 1.0);
	color = vec4(u_Color, 1.0);
}
