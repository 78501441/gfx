COMPILER=tcc
LINKER=ld
CFLAGS=-Wall -g
LIBS=-lm -lGL -lGLU -lGLEW -ldl -lX11 -lpthread
STATIC_LIBS=static_lib/libglfw3.a static_lib/libGLEW.a
OBJECTS=gl_primitives.o files.o renderer.o shader_load.o main.o
OUTPUT=gfx_demo


all: $(OUTPUT)


$(OUTPUT): $(OBJECTS)
	$(COMPILER) $(LIBS) -o $(OUTPUT) $^ $(STATIC_LIBS)


%.o: %.c
	$(COMPILER) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(OUTPUT) *.o
