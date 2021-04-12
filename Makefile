COMPILER=tcc
LINKER=ld
CFLAGS=-Wall -g
LIBS=-lm -lGL -lGLU -lGLEW -ldl -lX11 -lpthread
STATIC_LIBS=static_lib/libglfw3.a static_lib/libGLEW.a
OBJECTS=combined.o dynarray.o gl_primitives.o programs_list.o ranges.o renderer.o shader_load.o main.o
OBJTOOL=objcopy
OBJTOOL_FLAGS=--input binary --output elf64-x86-64 --binary-architecture i386
OUTPUT=gfx_demo


all: $(OUTPUT)


$(OUTPUT): $(OBJECTS)
	$(COMPILER) $(LIBS) -o $(OUTPUT) $^ $(STATIC_LIBS)


combined.o: shaders/combined
	$(OBJTOOL) $(OBJTOOL_FLAGS) $^ $@


%.o: src/%.c
	$(COMPILER) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(OUTPUT) *.o
