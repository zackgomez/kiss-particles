
/* Copyright (c) Mark J. Kilgard, 1994.  */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

/* New GLUT 3.0 glutGetModifiers() functionality used to make Shift-Left
   mouse scale the dinosaur's size. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       /* for cos(), sin(), and sqrt() */
#include <GL/glut.h>
#include "trackball.h"
#include "PManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

int W = 300, H = 300;

void ptest(void)
{
    Emitter *e = ParticleManager::get()->newEmitter();

    e->setParticleLocationF(new locationF(5.f))
     ->setParticleVelocityF(new velocityF(2.f, 20.f, 2.f));
    ParticleManager::get()->addEmitter(e);
    e->setLocation(glm::vec3(1, 10, 0));
    

    // Add a red + blue one
    std::vector<glm::vec4> c;
#define _add(_r,_b,_g,_a) c.push_back(glm::vec4(_r,_g,_b,_a))
    _add(1,0,0,0);
    _add(1,0,0,0);
    _add(1,0,0,0);
    _add(1,0,0,0);
    _add(1,0,0,0);
    _add(1,0,0,0);
    _add(0,1,0,0);
#undef _add
    Emitter *e2 = ParticleManager::get()->newEmitter();
    e2->setParticleLocationF(new locationF(2.f))
            ->setParticleColorF(new discreteColorF(c))
        ->setParticleVelocityF(new velocityF(2.f, 5.f, 2.f));

    ParticleManager::get()->addEmitter(e2);

}

void timerCallback (int value);

static const int msecs = 16;
void setupParticleLoop()
{
    glutTimerFunc(msecs, timerCallback, 0);
}

void timerCallback (int value)
{
    /* call back again after elapsedUSecs have passed */
    glutPostRedisplay();
    glutTimerFunc(msecs, timerCallback, value);
}

void redraw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ParticleManager::get()->render(msecs * 0.001f);
    glutSwapBuffers();
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    W = w;
    H = h;
}

void mouse(int button, int state, int x, int y)
{
    // TODO
}

void motion(int x, int y)
{
    // TODO
}

void keyboard(GLubyte key, GLint x, GLint y)
{
    // Quit on ESC
    if (key == 27)
        exit(0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    setupParticleLoop();
    ptest();


    glutCreateWindow("kiss_particle demo");
    glutDisplayFunc(redraw);
    glutReshapeFunc(myReshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective( /* field of view in degree */ 40.0,
            /* aspect ratio */ 1.0,
            /* Z near */ 1.0, /* Z far */ 200.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 80.0,  /* eye position */
            0.0, 0.0, 0.0,      /* center is at (0,0,0) */
            0.0, 1.0, 0.);      /* up is in positive Y direction */

    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}

void renderParticle(const glm::vec3 &loc,
        const glm::vec3 &part_size, const glm::vec4 &part_color)
{
    glMatrixMode(GL_MODELVIEW);

    glColor3fv(glm::value_ptr(part_color));
    glBegin(GL_POINTS);
    glVertex3fv(glm::value_ptr(loc));
    glEnd();
}

