#version 330 core
out vec4 color;
void main() {
    color = vec4(abs(gl_FragCoord.y - gl_FragCoord.x),
        abs(gl_FragCoord.x / 255.0),
        abs(gl_FragCoord.y / 255.0),
        1.0);
}
