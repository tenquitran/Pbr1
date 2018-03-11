#version 440 core
#pragma debug(on)
#pragma optimize(off)

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) uniform mat4 Model;
layout(location = 1) uniform mat4 MVP;

out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vWorldPos;

void main()
{
	vNormal = mat3(Model) * normal;

	vTexCoord = texCoord;

	vWorldPos = vec3(Model * position);

	gl_Position = MVP * position;
}
