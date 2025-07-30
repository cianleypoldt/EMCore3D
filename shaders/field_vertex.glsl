#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aFieldValue;

uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uModel;

out float fieldValue;

void main() {
    fieldValue = aFieldValue;
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}