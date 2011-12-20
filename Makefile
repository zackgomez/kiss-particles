
CXXFLAGS=-g -O0 -Wall -Iglm-0.9.2.7 -fpermissive
LDFLAGS= -lGL -lglut -lGLU

all: kiss_particles

kiss_particles: main.o PEmitter.o PManager.o Particle.o 
	g++ $(CXXFLAGS) $(LDFLAGS) -o $@ $^

test: 
	g++ main.cpp -lglut -I glm-0.9.2.7/ -fpermissive -lGLU trackball.c PManager.cpp Particle.cpp PEmitter.cpp

clean:
	rm -f ptest *.o
