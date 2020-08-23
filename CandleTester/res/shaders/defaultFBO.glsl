@shader_type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoordinates;

out vec2 o_tCoords;

uniform float u_time;
	
void main() {
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	o_tCoords = textureCoordinates;
}


@shader_type fragment
#version 330 core
			
out vec4 fragmentColor;

in vec2 o_tCoords;
			
uniform sampler2D u_textures[4];

void main() {
	fragmentColor = texture(u_textures[0], o_tCoords);
}