
CXXFLAGS=-g -O0 -Wall -Iglm-0.9.2.7 -fpermissive
LDFLAGS= -lGL -lglut -lGLU

all: kiss_particles

kiss_particles: main.o uistate.o ArcBall.o PEmitter.o PManager.o Particle.o PGroup.o PAction.o
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 

test: 
	g++ -g main.cpp -Wall -lGL -lGLU -lglut -I glm-0.9.2.7/ -fpermissive PManager.cpp Particle.cpp PEmitter.cpp PGroup.cpp PAction.cpp uistate.cpp ArcBall.cpp

libkiss_particles.a: 
	g++ $(CXXFLAGS) -c -o $@ $^

clean:
	rm -f ptest *.o kiss_particles
