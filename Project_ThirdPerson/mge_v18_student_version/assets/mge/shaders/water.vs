//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 mvpMatrix;

uniform int time;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform int pulseSpeed; //Higher number = slower.
uniform int pulseAmount; //The modulo number decides the speed. 200 is one pulse at a time, <200 is more pulses, >200 is less pulses.
uniform float pulseDist; //How far out should the vertices pulse.

out vec2 texCoord; //make sure the texture coord is interpolated
out vec4 fWorldPos;

void main( void ){
	vec4 worldPos = mvpMatrix * vec4(vertex, 1.f);

//	int pulseTime = time / pulseSpeed;

//	int pulse = (pulseTime + int((worldPos.x - worldPos.z))) % pulseAmount; //Divide the vertices by assigning a number between 0 and 200 (instead of -1 to 1). Offset this number using pulseTime, and apply the pulse curve using pulseAmount.

	vec3 pos = vertex; //Get the vertex position.

	//pos = vertex + normal * (sin((time/100000.0f)*(worldPos.x))*sin((time/100000.0f)*(worldPos.z)))/8.0f;//wrong way of doing this
	//pos = vertex + (normal*0.1) * (sin((time/600.0f)+worldPos.x)+cos((time/600.0f)+worldPos.z)/20.0f);//attempt 2
//	pos = vertex + (normal*0.05) * (sin((time/300.0f)+worldPos.x)+cos((time/300.0f)+worldPos.z)/20.0f);//attempt 3
		pos = vertex + (normal*0.05) * (sin((time/600.0f)+worldPos.x)+cos((time/300.0f)+worldPos.z)/20.0f);//attempt 4

//	if(pulse < pulseAmount/2.0f) { //If the vertex got assigned a number in the first half of the total pulseAmount.
//		pos = vertex + normal * ((pulse / (pulseAmount/2.0f)) * pulseDist); //Pulse the vertex outwards. The higher the number, the further it pulses outwards. (goes from neutral to fully extended)
//	} else { //Vertices in the second half of the total pulseAmount.
//		pos = vertex + normal * (((pulseAmount-pulse) / (pulseAmount/2.0f)) * pulseDist); //Still pulse the vertex outwards. The LOWER the number, the further it pulses outwards. (goes from fully extended to neutral)
//	}


    gl_Position = mvpMatrix * vec4(pos, 1.f); //Apply our new position.
	texCoord = uv;
	fWorldPos = worldPos;
}
