@shader_type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 o_color;
	
void main() {
	gl_Position = vec4(position, 1.0);
	o_color = color;
}


@shader_type fragment
#version 330 core
			
out vec4 fragmentColor;

in vec4 o_color;
			
void main() {
	fragmentColor = o_color;
}