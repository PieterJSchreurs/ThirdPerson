//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	mvpMatrix;

uniform int time;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

vec4 _WaterColor = vec4(0.4257677f, 0.8374597f, 0.9191176f, 0.2039216f);
vec4 _WaterFoamColor = vec4(0.5147058f, 0.5147058f, 0.5147058f, 0.5882353f);
int _WaveHeight = 30;
int _WaveInterval = 5000;
int _WaveBuildUp = 50;
int _WaveLength = 50;
int _WaveSpeed = 1;
vec4 _WaveDirection = vec4(-1.0f, -1.0f, 0.0f, 0.0f);

out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ){
	vec4 direction = normalize(_WaveDirection);
	float interval = _WaveSpeed * _WaveInterval; //2 = 0.5 * 4
	float timeScale = (int((uv.x * -direction.x) + (uv.y * -direction.y) + (1.0f * -direction.z) + time * _WaveSpeed * 100) % int(interval * 100)) / (interval * 100); //11 * 0.5 = 5.5 % 2 = 1.5
	float t = clamp(1.0f - timeScale * (1.0f / (_WaveLength / 100.0f)), 0.0f, 1.0f);
	float buildUp = clamp(1.0f - timeScale, 0.0f, 1.0f); // 1 - 1 = 0
	if (buildUp <= _WaveBuildUp / 100.0f)
	{
		t = 1 - (buildUp / (_WaveBuildUp / 100.0f)); // 1 - (0 / 0.2) = 1 - 0 = 1
	}
	vec3 pos = vertex + normal * t * (_WaveHeight / 100.0f);

	gl_Position = mvpMatrix * vec4(pos, 1.f); //Apply our new position.
	texCoord = uv;

	/*vertexToFragment o;
	// Transform the point to clip space:
	o.vertex = mul(UNITY_MATRIX_MVP,v.vertex); //mul = multiplication
	o.modelPosition = mul(UNITY_MATRIX_P, v.vertex);
	o.uv = v.uv;
	return o;



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
	texCoord = uv;*/
}
