
CXXFLAGS=-g -O0 -Wall -Iglm-0.9.2.7 -fpermissive
LDFLAGS= -lGL -lGLUT

all: kiss_particles

kiss_particles: main.o PEmitter.o PManager.o Particle.o 
	g++ $(CXXFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f ptest *.o
