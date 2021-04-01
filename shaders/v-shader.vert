#version 330 core
layout(location=0) in vec4 pos;
uniform vec4 pos_offset;
void main() {
    gl_Position = pos + pos_offset;
    gl_Position.y *= -1.0f;
}
