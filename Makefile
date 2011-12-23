
CXXFLAGS=-g -O0 -Wall -Iglm-0.9.2.7 -fpermissive
LDFLAGS= -lGL -lglut -lGLU

all: kiss_particles libkiss_particles.a

kiss_particles: main.o uistate.o ArcBall.o libkiss_particles.a
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 

libkiss_particles.a:  PEmitter.o PManager.o Particle.o PGroup.o PAction.o
	ar rcs $@ $^

clean:
	rm -f ptest *.o kiss_particles libkiss_particles.a
