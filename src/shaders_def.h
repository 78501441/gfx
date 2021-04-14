
#ifndef SHADERS_DEF_H_
#define SHADERS_DEF_H_

#define UNUSED(t) (void)t;

static const char vert_shader_src_fp_[] = "shaders/v-shader.vert";
static const char frag_shader_src_fp_[] = "shaders/f-shader.frag";

static const char identity_shader_src_[] = "#version 330 core\n"
                                           "layout(location=0) in vec4 pos;\n"
                                           "void main() {\n"
                                           "  gl_Position = pos;\n"
                                           "}\n";

static const char aqua_shader_src_[] =
    "#version 330 core\n"
    "out vec4 out_color;\n"
    "void main() {\n"
    "  out_color = vec4(0.0f, 0.75f, 0.58f, 0.0f);\n"
    "}\n";

const float positions[] = {
    0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};

const float box[] = {
    /* Surrounding box. */
    -0.9f,
    0.9f,
    0.0f,
    0.9f,
    0.9f,
    0.0f,
    0.9f,
    -0.9f,
    0.0f,
    -0.9f,
    -0.9f,
    0.0f};

#endif
