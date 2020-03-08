SRCDIR=$(PWD)
NAMEDEST=ventana
all:
	g++ -std=c++2a -Wall -lGL -lGLEW -lglfw $(SRCDIR)/*.cpp -I$(SRCDIR)/glm -I$(SRCDIR)/include -o $(NAMEDEST)
	./$(NAMEDEST)
clean:
	rm -f *.o