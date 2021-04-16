
#pragma once

struct shader_source;

unsigned int
compile_shaders(const struct shader_source *sources,
                unsigned int shaders_count);
