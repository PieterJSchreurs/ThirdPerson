//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	mvpMatrix;
uniform sampler2D heightmapTexture;

uniform float maxHeight;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ){
	vec4 vertColor = texture(heightmapTexture, uv);
	vec3 pos = vertex + normal * vertColor.r * maxHeight;

    gl_Position = mvpMatrix * vec4(pos, 1.f); //Apply our new position.
	texCoord = uv;
}
