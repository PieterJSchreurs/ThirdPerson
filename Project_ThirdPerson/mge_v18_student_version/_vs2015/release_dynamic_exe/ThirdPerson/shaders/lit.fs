//DIFFUSE COLOR FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform vec3 diffuseColor;

uniform vec3 lightPosition;
uniform vec3 lightColor;

out vec4 fragment_color;

void main( void ) {

	if(lightPosition.x - gl_FragCoord.x > 50000000f) {
		fragment_color = vec4 (lightColor,1);
	} else {
		fragment_color = vec4 (diffuseColor,1);
	}
}
