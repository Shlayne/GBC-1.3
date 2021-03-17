#type vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 tintColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in uint texIndex;

out vec2 _texCoord;
out vec4 _tintColor;
out flat uint _texIndex;

uniform mat4 viewProjection;

void main()
{
	_texCoord = texCoord;
	_tintColor = tintColor;
	_texIndex = texIndex;
	gl_Position = viewProjection * vec4(position, 1.0);
}

#type fragment
#version 460 core

in vec2 _texCoord;
in vec4 _tintColor;
in flat uint _texIndex;

layout(location = 0) out vec4 outColor;

uniform sampler2D textures[32];

vec4 GetTextureColor()
{
	switch (_texIndex)
	{
		case  0: return texture(textures[ 0], _texCoord);
		case  1: return texture(textures[ 1], _texCoord);
		case  2: return texture(textures[ 2], _texCoord);
		case  3: return texture(textures[ 3], _texCoord);
		case  4: return texture(textures[ 4], _texCoord);
		case  5: return texture(textures[ 5], _texCoord);
		case  6: return texture(textures[ 6], _texCoord);
		case  7: return texture(textures[ 7], _texCoord);
		case  8: return texture(textures[ 8], _texCoord);
		case  9: return texture(textures[ 9], _texCoord);
		case 10: return texture(textures[10], _texCoord);
		case 11: return texture(textures[11], _texCoord);
		case 12: return texture(textures[12], _texCoord);
		case 13: return texture(textures[13], _texCoord);
		case 14: return texture(textures[14], _texCoord);
		case 15: return texture(textures[15], _texCoord);
		case 16: return texture(textures[16], _texCoord);
		case 17: return texture(textures[17], _texCoord);
		case 18: return texture(textures[18], _texCoord);
		case 19: return texture(textures[19], _texCoord);
		case 20: return texture(textures[20], _texCoord);
		case 21: return texture(textures[21], _texCoord);
		case 22: return texture(textures[22], _texCoord);
		case 23: return texture(textures[23], _texCoord);
		case 24: return texture(textures[24], _texCoord);
		case 25: return texture(textures[25], _texCoord);
		case 26: return texture(textures[26], _texCoord);
		case 27: return texture(textures[27], _texCoord);
		case 28: return texture(textures[28], _texCoord);
		case 29: return texture(textures[29], _texCoord);
		case 30: return texture(textures[30], _texCoord);
		case 31: return texture(textures[31], _texCoord);
		default: return vec4(0.0);
	}
}

void main()
{
	outColor = _tintColor * GetTextureColor();
	if (outColor.a == 0.0)
		discard;
}
