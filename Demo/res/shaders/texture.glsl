@shader_type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoordinates;
layout(location = 2) in float textureIndex;
layout(location = 3) in vec4 color;
layout(location = 4) in vec4 parameters;

out vec2 o_tCoords;
out vec4 o_color;
out float o_textureIndex;

uniform float u_time;
uniform mat4 u_projectionViewMatrix;
	
void main() {
	if (parameters[0] == 1) {
		gl_Position = vec4(position.x, position.y, 0., 1.0);
	} else {
		gl_Position = u_projectionViewMatrix * vec4(position.x, position.y, position.z, 1.0);
	}

	gl_PointSize = 5.0;

	o_tCoords = textureCoordinates;
	o_color = color;
	o_textureIndex = textureIndex;
}


@shader_type fragment
#version 330 core
			
out vec4 fragmentColor;

in vec2 o_tCoords;
in vec4 o_color;
in float o_textureIndex;
			
uniform sampler2D u_textures[32];

void main() {
	fragmentColor = texture(u_textures[int(o_textureIndex)], o_tCoords) * o_color;
	//fragmentColor = o_color;
}