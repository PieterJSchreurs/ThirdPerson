//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	mvpMatrix;

uniform int time;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ){
	int pulseSpeed = 15; //Higher number = slower.
	int pulseTime = time / pulseSpeed; 
	int pulseAmount = 150; //The modulo number decides the speed. 200 is one pulse at a time, <200 is more pulses, >200 is less pulses.
	float pulseDist = 0.5f; //How far out should the vertices pulse.

	int pulse = (pulseTime + int((vertex.x + 1.0f)*100.0f)) % pulseAmount; //Divide the vertices by assigning a number between 0 and 200 (instead of -1 to 1). Offset this number using pulseTime, and apply the pulse curve using pulseAmount.

	vec3 pos = vertex; //Get the vertex position.

	if(pulse < pulseAmount/2.0f) { //If the vertex got assigned a number in the first half of the total pulseAmount.
		pos = vertex + normal * ((pulse / (pulseAmount/2.0f)) * pulseDist); //Pulse the vertex outwards. The higher the number, the further it pulses outwards. (goes from neutral to fully extended)
	} else { //Vertices in the second half of the total pulseAmount.
		pos = vertex + normal * (((pulseAmount-pulse) / (pulseAmount/2.0f)) * pulseDist); //Still pulse the vertex outwards. The LOWER the number, the further it pulses outwards. (goes from fully extended to neutral)
	}


    gl_Position = mvpMatrix * vec4(pos, 1.f); //Apply our new position.
	texCoord = uv;
}
