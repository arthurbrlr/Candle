@shader_type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoordinates;

out vec2 o_tCoords;
out vec2 o_blurTextureCoords[11];

uniform float u_time;
uniform vec2 u_winDim;
	
void main() {
	gl_Position = vec4(position.x, position.y, 0.0, 1.0);
	o_tCoords = textureCoordinates;

	vec2 centerTexCoords = position.xy * 0.5 + 0.5;
	float pixelSize = 2.0 / u_winDim.x;

	for (int i = -5; i < 5; i++) {
		o_blurTextureCoords[i + 5] = centerTexCoords + vec2(pixelSize * i, 0);
	}
}


@shader_type fragment
#version 330 core
			
out vec4 fragmentColor;

in vec2 o_tCoords;
in vec2 o_blurTextureCoords[11];
			
uniform sampler2D u_texture;

void main() {
	fragmentColor = vec4(0.0);

	fragmentColor += texture(u_texture, o_blurTextureCoords[0]) * 0.0093;
    fragmentColor += texture(u_texture, o_blurTextureCoords[1]) * 0.028002;
    fragmentColor += texture(u_texture, o_blurTextureCoords[2]) * 0.065984;
    fragmentColor += texture(u_texture, o_blurTextureCoords[3]) * 0.121703;
    fragmentColor += texture(u_texture, o_blurTextureCoords[4]) * 0.175713;
    fragmentColor += texture(u_texture, o_blurTextureCoords[5]) * 0.198596;
    fragmentColor += texture(u_texture, o_blurTextureCoords[6]) * 0.175713;
    fragmentColor += texture(u_texture, o_blurTextureCoords[7]) * 0.121703;
    fragmentColor += texture(u_texture, o_blurTextureCoords[8]) * 0.065984;
    fragmentColor += texture(u_texture, o_blurTextureCoords[9]) * 0.028002;
    fragmentColor += texture(u_texture, o_blurTextureCoords[10]) * 0.0093;
}