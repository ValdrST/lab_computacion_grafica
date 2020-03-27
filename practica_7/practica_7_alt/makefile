SRCDIR=$(PWD)
NAMEDEST=ventana
SRCDISTNAME=practica7_luces.cpp
SRCOBJ=Camera.o Light.o Mesh_tn.o Sphere.o Window.o DirectionalLight.o Material.o PointLight.o Shader_light.o Texture.o
SRCCODE=Camera.cpp Light.cpp Mesh_tn.cpp Sphere.cpp Window.cpp DirectionalLight.cpp Material.cpp PointLight.cpp Shader_light.cpp Texture.cpp
CC=g++
CFLAGS=-std=c++2a -Wall -lGL -lGLEW -lglfw
INCLUDES=-I$(SRCDIR)/glm -I$(SRCDIR)/include
.PHONY: all
all:
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAMEDEST) $(SRCDISTNAME) $(SRCOBJ)
	chmod +x $(NAMEDEST)
	./$(NAMEDEST)
objects.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRCCODE)
.PHONY: dist
dist:
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAMEDEST) $(SRCDISTNAME) $(SRCOBJ)
	chmod +x $(NAMEDEST)
	./$(NAMEDEST)
.PHONY: clean
clean:
	rm -f *.o