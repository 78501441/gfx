
#ifndef GFXDEMO_SHADER_LOAD_H_
#define GFXDEMO_SHADER_LOAD_H_


struct shader_desc {
  unsigned int type;
  const char *source;
  const char *name;
};


unsigned int
compile_shaders(const struct shader_desc *shaders,
                unsigned int shaders_count);


#endif /* GFXDEMO_SHADER_LOAD_H_ */
