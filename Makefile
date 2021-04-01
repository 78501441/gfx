COMPILER=gcc
LINKER=ld
CFLAGS=-Wall -g
LIBS=-lm -lGL -lGLU -lGLEW -ldl -lX11 -lpthread
STATIC_LIBS=static_lib/libglfw3.a static_lib/libGLEW.a
OUTPUT=gfx_demo


all: $(OUTPUT)


$(OUTPUT): main.o shader_load.o
	$(COMPILER) $(LIBS) -o $(OUTPUT) $^ $(STATIC_LIBS)


main.o: main.c
	$(COMPILER) $(CFLAGS) -c -o $@ $<


shader_load.o: shader_load.c
	$(COMPILER) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(OUTPUT) *.o
