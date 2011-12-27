#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       /* for cos(), sin(), and sqrt() */
#include <GL/glut.h>
#include "PManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "uistate.h"

int windowWidth = 800, windowHeight = 600;

// Peter's mystical ui controller for arcball transformation and stuff
static UIState *ui;

void ptest(void)
{
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
    _add(1,1,0,0);
#undef _add

    std::vector<glm::vec4> tornadoc;
#define _add(_r,_g,_b,_a) tornadoc.push_back(glm::vec4(_r,_g,_b,_a))
    _add(0.8, 1.8, 0.8, 1);
    _add(0.6, 1.6, 0.6, 1);
    _add(0.6, 1.6, 0.6, 1);
    _add(0.9, 1.9, 0.9, 1);
    _add(0.9, 1.9, 0.9, 1);
    _add(0.8, 1.8, 0.8, 1);
    _add(0.8, 1.8, 0.8, 1);
    _add(0.2, 1.2, 0.2, 1);
    _add(0.5, 1.5, 0.5, 1);
    _add(0.5, 1.5, 0.5, 1);
    _add(0.5, 1.5, 0.5, 1);
    _add(0.8, 1.8, 0.8, 1);
#undef _add

    glm::vec3 up;
    float radius;

    PGroup* bounce_group = ParticleManager::newGroup("bounce");
    Emitter *e;
    // Left emitter
    glm::vec3 left_loc = glm::vec3(-10.0, 0.0, 0.0);
    e = ParticleManager::get()->newEmitter();
    e->setLocation(left_loc)
     ->setParticleLocationF(new locationF(2.f))
     ->setParticleVelocityF(new velocityF(5.f, 20.f, 2.f))
     ->setParticleLifetimeF(new lifetimeNormalF(0.5, 0.1))
     ->setOutputGroup("bounce");
    ParticleManager::get()->addEmitter(e);
    bounce_group->addAction(
        new PPlaneBounceF(left_loc - glm::vec3(0,1,0), 
                          glm::vec3(0,1,0), 1)); 

    // center emitter
    // tornado hazard
    up = glm::vec3(0, 1, 0);
    radius = 2.f;
    glm::vec3 tornado_center = glm::vec3(0.f, -3.f, 0.f);

    PGroup *pg_tornado = ParticleManager::newGroup("tornado");
    pg_tornado->addAction(new CentripetalForceF(tornado_center, up, radius));
    pg_tornado->addAction(new ConstForceF(4, up));

    e = ParticleManager::get()->newEmitter();
    e->setLocation(tornado_center)
     ->setParticleLocationF(new circleLocationF(radius, up))
     ->setParticleVelocityF(new circleTangentVelocityF(4.f, 0.f, up))
     ->setParticleLifetimeF(new lifetimeNormalF(1.4, 0.4))
     ->setOutputRate(500)
     ->setParticleColorF(new discreteColorF(tornadoc))
     ->setOutputGroup("tornado");
    ParticleManager::get()->addEmitter(e);

    // top left, hopefully 
    // tornado hazard
    up = glm::vec3(0, 1, 0);
    radius = 2.f;
    tornado_center += glm::vec3(-10, 10, 0);

    pg_tornado = ParticleManager::newGroup("tornado2");
    pg_tornado->addAction(new CentripetalForceF(tornado_center, up, radius));

    e = ParticleManager::get()->newEmitter();
    e->setLocation(tornado_center)
     ->setParticleLocationF(new circleLocationF(radius, up))
     ->setParticleVelocityF(new circleTangentVelocityF(4.f, 0.f, up))
     ->setParticleLifetimeF(new lifetimeNormalF(1.4, 0.4))
     ->setOutputRate(500)
     ->setParticleColorF(new discreteColorF(tornadoc))
     ->setOutputGroup("tornado2");
    ParticleManager::get()->addEmitter(e);
    e->addEmitterAction(new PERandomF(5));

    // bottom emitter
    up = glm::vec3(0, 0, 1);
    e = ParticleManager::get()->newEmitter();
    e->setLocation(glm::vec3(0.0, -10.0, 0.0))
     ->setParticleLocationF(new circleInteriorLocationF(3.0f, up))
     ->setParticleVelocityF(new coneVelocityF(4.f, 1.f, up, 1.0))
     ->setParticleLifetimeF(new lifetimeNormalF(1.5, 0.1))
     ->setOutputRate(500);
    ParticleManager::get()->addEmitter(e);

    // Right emitter
    // black hole
    up = glm::vec3(0, 0, 1);
    e = ParticleManager::get()->newEmitter();
    e->setLocation(glm::vec3(10.0, 0.0, 0.0))
     ->setParticleLocationF(new circleLocationF(3.0f, up))
     ->setParticleVelocityF(new circleTangentVelocityF(4.f, 1.f, up))
     ->setParticleLifetimeF(new lifetimeNormalF(0.5, 0.1))
     ->setOutputRate(1500);
    ParticleManager::get()->addEmitter(e);

    // middle top emitter
    // A fake spinning sphere
    up = glm::vec3(0, 1, 0);
    e = ParticleManager::get()->newEmitter();
    e->setLocation(glm::vec3(0.0, 10.0, 0.0))
     ->setParticleLocationF(new locationF(3.f))
     ->setParticleVelocityF(new circleTangentVelocityF(4.f, 1.f, up))
     ->setParticleLifetimeF(new lifetimeNormalF(0.2, 0.1))
     ->setOutputRate(2500);
    ParticleManager::get()->addEmitter(e);

    // top right emitter
    // "jump puff"
    up = glm::vec3(0, 1, 0);
    e = ParticleManager::get()->newEmitter();
    e->setLocation(glm::vec3(10.0, 10.0, 0.0))
     ->setParticleLocationF(new circleLocationF(3.0f, up))
     ->setParticleVelocityF(new coneVelocityF(4.f, 1.f, up, 0.5))
     ->setParticleLifetimeF(new lifetimeNormalF(0.2, 0.1))
     ->setOutputRate(2500)
     ->setParticleColorF(new discreteColorF(c));
    ParticleManager::get()->addEmitter(e);

    // bottom right emitter
    // combiner example, spinning disc thingy
    up = glm::vec3(0, 1, 0);
    e = ParticleManager::get()->newEmitter();
    e->setLocation(glm::vec3(10.0, -10.0, 0.0))
     ->setParticleLocationF(new circleLocationF(3.0f, up))
     ->setParticleVelocityF(new velocityCombinerF(
                 new coneVelocityF(4.f, 1.f, up, 0.5),
                 new circleTangentVelocityF(4., 1.f, up),
                 0.5))
     ->setParticleLifetimeF(new lifetimeNormalF(0.2, 0.1))
     ->setOutputRate(2500)
     ->setParticleColorF(new discreteColorF(c));
    ParticleManager::get()->addEmitter(e);


    // bottom left emitter
    // looks like a firework
    PGroup *pg_gravity = ParticleManager::newGroup("gravity");
    Emitter *e2 = ParticleManager::get()->newEmitter();
    e2->setParticleLocationF(new locationF(3.f))
        ->setParticleColorF(new discreteColorF(c))
        ->setParticleVelocityF(new circleTangentVelocityF(0.f, 0.f, glm::vec3(0, 1, 0)))
        ->setParticleLifetimeF(new lifetimeF(5.f))
        ->setLocation(glm::vec3(-10, -10, 0))
        ->setOutputRate(250)
        ->setOutputGroup("gravity"); 

    //pg_gravity->addAction(new GravityActionF(55));
    glm::vec3 blackhole_loc = glm::vec3(0, -8, 0);
    glm::vec3 blackhole_loc2 = glm::vec3(-12, -20, 0);
    pg_gravity->addAction(new PPointAttractorF(blackhole_loc, 50));
    pg_gravity->addAction(new PPointSinkF(blackhole_loc, .8f));
    pg_gravity->addAction(new PPlaneSinkF(blackhole_loc, glm::vec3(-1, 0, 0)));
    pg_gravity->addAction(new PPointAttractorF(blackhole_loc2, 50));
    pg_gravity->addAction(new PPointSinkF(blackhole_loc2, .8f));
    pg_gravity->addAction(new PPlaneSinkF(blackhole_loc2, glm::vec3(0, 1, 0)));
    
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
    void
showMessage(GLfloat x, GLfloat y, GLfloat z, char *message)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(.02, .02, .02);
    while (*message) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *message);
        message++;
    }
    glPopMatrix();
}
void redraw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply the camera transformation.
    ui->ApplyViewingTransformation();
    char buffer [50];
    static int cnt = 0;
    cnt++;
    static int np = ParticleManager::get()->numParticles();
    if (!(cnt % 100)) np = ParticleManager::get()->numParticles();
    sprintf (buffer, "particles: %d", np); 
    showMessage(-15, 15, 0, buffer);
    ParticleManager::get()->render(msecs * 0.001f);
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    
    if( width <= 0 || height <= 0 ) return;
    
    ui->WindowX() = width;
    ui->WindowY() = height;
    
    ui->Aspect() = float( width ) / height;
    ui->SetupViewport();
    ui->SetupViewingFrustum();
}

void mouse(int button, int state, int x, int y)
{
    // Just pass it on to the ui controller.
    ui->MouseFunction(button, state, x, y);
}

void motion(int x, int y)
{
    // Just pass it on to the ui controller.
    ui->MotionFunction(x, y);
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
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glEnable(GL_DEPTH_TEST);

    ui = new UIState;
    ui->Trans() = glm::vec3(0, 0, 0);
    ui->Radius() = 80;
    ui->Near() = .1;
    ui->Far() = 1000;
    ui->CTrans() = glm::vec3(0, 0, -40);

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

