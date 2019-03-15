//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D diffuseTexture;

uniform int time;

uniform vec4 waterColor;
uniform vec4 waterFoamColor;

uniform int pulseSpeed;//Higher number = slower.
uniform int pulseAmount; //The modulo number decides the speed. 200 is one pulse at a time, <200 is more pulses, >200 is less pulses.
uniform float pulseDist; //How far out should the vertices pulse.

in vec4 fWorldPos;
in vec2 texCoord;

out vec4 fragment_color;

void main( void ) {
	int pulseTime = time / pulseSpeed; 

	int pulse = (pulseTime + int((fWorldPos.x + fWorldPos.z))) % pulseAmount; //Divide the vertices by assigning a number between 0 and 200 (instead of -1 to 1). Offset this number using pulseTime, and apply the pulse curve using pulseAmount.

	vec4 col = texture(diffuseTexture, texCoord);

	vec4 waterFoam = vec4(col.x * waterFoamColor.x * col.a * waterFoamColor.a, col.y * waterFoamColor.y* col.a * waterFoamColor.a, col.z * waterFoamColor.z * col.a * waterFoamColor.a, col.a * waterFoamColor.a); //Why do i have to do this...

	if(pulse < pulseAmount/2.0f) { //If the vertex got assigned a number in the first half of the total pulseAmount.
		fragment_color = waterColor + ((waterFoam) * ((pulse / (pulseAmount/2.0f))+0.15f));	 //Pulse the vertex outwards. The higher the number, the further it pulses outwards. (goes from neutral to fully extended)
	} else { //Vertices in the second half of the total pulseAmount.
		//Still pulse the vertex outwards. The LOWER the number, the further it pulses outwards. (goes from fully extended to neutral)
		fragment_color = waterColor + ((waterFoam) * (((pulseAmount-pulse) / (pulseAmount/2.0f))+0.15f));
	}
}