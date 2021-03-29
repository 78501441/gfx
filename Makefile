COMPILER=gcc
CFLAGS=-Wall -g
LIBS=-lm -lGL -lGLU -lGLEW -ldl -lX11 -lpthread
STATIC_LIBS=static_lib/libglfw3.a static_lib/libGLEW.a
OUTPUT=gfx_demo


all: $(OUTPUT)


$(OUTPUT): main.c
	$(COMPILER) $(CFLAGS) $(LIBS) -o $(OUTPUT) $< $(STATIC_LIBS)


clean:
	rm -f $(OUTPUT)
