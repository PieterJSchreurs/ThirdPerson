//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D splatmapTexture;

uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;
uniform sampler2D diffuseTexture4;

uniform float textureDetail;
uniform int time;

in vec2 texCoord;
out vec4 fragment_color;

void main( void ) {
	vec4 splatmapColor = texture(splatmapTexture, texCoord);

	vec2 fromCenter = texCoord - vec2(0.5f, 0.5f);
	vec2 offsetUV = texCoord + fromCenter * (cos(time/3000.0f)-1) * 0.1f; //Displace UVs away from the center. UVs that are further away are affected more.

	fragment_color =	texture(diffuseTexture1,texCoord*textureDetail) * splatmapColor.r 
						+ texture(diffuseTexture2,offsetUV*textureDetail*2) * splatmapColor.g 
						+ texture(diffuseTexture3,texCoord*textureDetail) * splatmapColor.b 
						+ texture(diffuseTexture4,texCoord*textureDetail) * splatmapColor.a;
}
