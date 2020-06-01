@shader_type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoordinates;

out vec2 o_tCoords;
	
void main() {
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	o_tCoords = textureCoordinates;
}


@shader_type fragment
#version 330 core
			
out vec4 fragmentColor;

in vec2 o_tCoords;

uniform float u_time;			
uniform sampler2D u_textures[4];

void main() {
	vec4 color = texture(u_textures[0], o_tCoords);

	float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));

	if (brightness > 0.75)
		fragmentColor = color * brightness;
	else
		fragmentColor = vec4(0., 0., 0., 1.);
}