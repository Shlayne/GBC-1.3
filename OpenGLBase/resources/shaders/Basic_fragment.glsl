#version 460 core

in vec2 _texCoord;
in vec4 _tintColor;
in float _texIndex;

out vec4 outColor;

uniform sampler2D samplers;

void main()
{
	outColor = _tintColor * texture(samplers, _texCoord);
	if (outColor.a == 0.0)
		discard;
}
