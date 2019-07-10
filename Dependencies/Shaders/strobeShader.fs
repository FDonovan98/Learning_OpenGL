#version 330 core

out vec4 FragColor;

uniform float color;

void main() {
   FragColor = vec4(color, color, 0.0f, 1.0f);
}