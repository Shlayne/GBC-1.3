#type vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 tintColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in uint texIndex;
layout(location = 4) in float tilingFactor;

out vec2 _texCoord;
out vec4 _tintColor;
out flat uint _texIndex;
out flat float _tilingFactor;

uniform mat4 viewProjection;

void main()
{
	_texCoord = texCoord;
	_tintColor = tintColor;
	_texIndex = texIndex;
	_tilingFactor = tilingFactor;
	gl_Position = viewProjection * vec4(position, 1.0);
}

#type fragment
#version 460 core

in vec2 _texCoord;
in vec4 _tintColor;
in flat uint _texIndex;
in flat float _tilingFactor;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D textures[32];

vec4 GetTextureColor(vec2 uv)
{
	switch (_texIndex)
	{
		case 0: return texture(textures[0], uv);
		case 1: return texture(textures[1], uv);
		case 2: return texture(textures[2], uv);
		case 3: return texture(textures[3], uv);
		case 4: return texture(textures[4], uv);
		case 5: return texture(textures[5], uv);
		case 6: return texture(textures[6], uv);
		case 7: return texture(textures[7], uv);
		case 8: return texture(textures[8], uv);
		case 9: return texture(textures[9], uv);
		case 10: return texture(textures[10], uv);
		case 11: return texture(textures[11], uv);
		case 12: return texture(textures[12], uv);
		case 13: return texture(textures[13], uv);
		case 14: return texture(textures[14], uv);
		case 15: return texture(textures[15], uv);
		case 16: return texture(textures[16], uv);
		case 17: return texture(textures[17], uv);
		case 18: return texture(textures[18], uv);
		case 19: return texture(textures[19], uv);
		case 20: return texture(textures[20], uv);
		case 21: return texture(textures[21], uv);
		case 22: return texture(textures[22], uv);
		case 23: return texture(textures[23], uv);
		case 24: return texture(textures[24], uv);
		case 25: return texture(textures[25], uv);
		case 26: return texture(textures[26], uv);
		case 27: return texture(textures[27], uv);
		case 28: return texture(textures[28], uv);
		case 29: return texture(textures[29], uv);
		case 30: return texture(textures[30], uv);
		case 31: return texture(textures[31], uv);
		default: return vec4(0.0);
	}
}

void main()
{
	outColor = _tintColor * GetTextureColor(_texCoord * _tilingFactor);
	if (outColor.a == 0.0)
		discard;
}
