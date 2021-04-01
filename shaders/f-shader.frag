#version 330 core
out vec4 color;
uniform vec4 pos_offset;
void main() {
    if (pos_offset == vec4(0.0, 0.0, 0.0, 0.0)) {
        color = vec4(0.0f, 0.65f, 0.54f, 1.0f);
    } else {
        color = vec4(abs(gl_FragCoord.y - gl_FragCoord.x),
            abs(gl_FragCoord.x / 255.0),
            abs(gl_FragCoord.y / 255.0),
            1.0);
    }
}
