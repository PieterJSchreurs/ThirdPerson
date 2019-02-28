//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D diffuseTexture;

uniform int time;

in vec2 texCoord;
out vec4 fragment_color;

vec4 _WaterColor = vec4(0.035, 0.6f, 0.95f, 0.5f);
vec4 _WaterFoamColor = vec4(0.5147058f, 0.5147058f, 0.5147058f, 0.5882353f);
int _WaveHeight = 30;
int _WaveInterval = 5000;
int _WaveBuildUp = 50;
int _WaveLength = 50;
int _WaveSpeed = 1;
vec4 _WaveDirection = vec4(-1.0f, -1.0f, 0.0f, 0.0f);

void main( void ) {

	vec4 direction = normalize(_WaveDirection);
	float interval = _WaveSpeed * _WaveInterval; //2 = 0.5 * 4
	float timeScale = (int((texCoord.x * -direction.x) + (texCoord.y * -direction.y) + (1.0f * -direction.z) + time * _WaveSpeed * 100) % int(interval * 100)) / (interval * 100); //11 * 0.5 = 5.5 % 2 = 1.5
	float alpha = clamp(1.0f - timeScale * (1.0f / (_WaveLength / 100.0f)), 0.0f, 1.0f);
	float buildUp = clamp(1.0f - timeScale, 0.0f, 1.0f); // 1 - 1 = 0
	if (buildUp <= _WaveBuildUp / 100.0f)
	{
		alpha = 1 - (buildUp / (_WaveBuildUp / 100.0f)); // 1 - (0 / 0.2) = 1 - 0 = 1
		//alpha = 1 - (buildUp / _WaveBuildUp);
	}
	vec4 col = texture(diffuseTexture, texCoord);

	fragment_color = _WaterColor + (col * (_WaterFoamColor * (1 - alpha + 0.25f)));
	//fragment_color = _WaterColor + ((col * (1 - alpha) * (_WaveHeight / 100.0f)) * _WaterFoamColor);
	// coordinate-wise multiplication!:


	//fragment_color = texture(diffuseTexture,texCoord);
}