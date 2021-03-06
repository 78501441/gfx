COMPILER=tcc
LINKER=ld
CFLAGS=-Wall -g -Iext/glfw/include -Iext/glfw/deps
LIBS=-lm -lGL -ldl -lX11 -lpthread
STATIC_LIBS=ext/glfw/src/libglfw3.a ext/glfw/deps/libglad_gl.a
C_FILES=$(subst src/,,$(wildcard src/*.c))
O_FILES=$(subst .c,.o,$(C_FILES)) combined.o
OBJTOOL=objcopy
OBJTOOL_FLAGS=--input binary --output elf64-x86-64 --binary-architecture i386
OUTPUT=gfx_demo


all: $(OUTPUT)


source_format:
	clang-format -i src/*.c src/*.h


$(OUTPUT): $(O_FILES)
	$(COMPILER) $(LIBS) -o $(OUTPUT) $^ $(STATIC_LIBS)


combined.o: shaders/combined
	$(OBJTOOL) $(OBJTOOL_FLAGS) $^ $@


%.o: src/%.c
	$(COMPILER) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(OUTPUT) *.o
