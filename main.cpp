#include <GL/glew.h>
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

GLuint partbuf = 0;
const float dt = 16.0f;

// Peter's mystical ui controller for arcball transformation and stuff
static UIState *ui;

void setupEmitters(void)
{
    // Create a fountain
    PGroup* bounce_group = ParticleManager::get()->newGroup("gravity");
    Emitter *e = ParticleManager::get()->newEmitter();
    e->setLocation(glm::vec3(0, 0, 0))
     ->setParticleLocationF(new locationF(0.05f))
     ->setParticleVelocityF(new coneVelocityF(5.f, 1.f, glm::vec3(0, 1, 0), 0.85f))
     ->setParticleLifetimeF(new lifetimeNormalF(2.0, 0.1))
     ->setOutputRate(3000)
     ->setOutputGroup("gravity");
    ParticleManager::get()->addEmitter(e);
    bounce_group->addAction(new ConstForceF(6, glm::vec3(0, -1, 0)));
}

void showMessage(GLfloat x, GLfloat y, GLfloat z, char *message)
{
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(x, y, z);
    glScalef(.02, .02, .02);
    while (*message) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *message);
        message++;
    }
    glPopMatrix();
}

void displayParticleCount(void)
{
    char buffer [50];
    static int cnt = 0;
    cnt++;
    static int np = ParticleManager::get()->numParticles();
    if (!(cnt % 100)) np = ParticleManager::get()->numParticles();
    sprintf (buffer, "particles: %d", np); 
    showMessage(-15, 15, 0, buffer);
}

void redraw(void)
{
    // Update first
    ParticleManager::get()->startUpdate(dt * 0.001f);
    ParticleManager::get()->update();

    // Now render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply the camera transformation.
    ui->ApplyViewingTransformation();
    displayParticleCount();

    ParticleManager::get()->render(dt * 0.001f);
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

    if (key == 'r')
        ParticleManager::get()->startRecording();

    if (key == 's')
    {
        const double *times = ParticleManager::get()->stopRecording();
        std::cout << "Recorded times:\n"
            << "creation(us): " << times[0] << '\n'
            << "update(us):   " << times[1] << '\n'
            << "render(us):   " << times[2] << '\n';
    }
}

void idle()
{
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    glutCreateWindow("kiss_particle demo");
    glutDisplayFunc(redraw);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW: " << glewGetErrorString(err) << '\n';
        exit(1);
    }

    setupEmitters();

    glEnable(GL_DEPTH_TEST);

    ui = new UIState;
    ui->Trans() = glm::vec3(0, 0, 0);
    ui->Radius() = 80;
    ui->Near() = .1;
    ui->Far() = 1000;
    ui->CTrans() = glm::vec3(0, 0, -40);

    glGenBuffers(1, &partbuf);

    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}

void renderParticles(const std::vector<particleData> &data)
{
    glBindBuffer(GL_ARRAY_BUFFER, partbuf);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(particleData), &data.front(), GL_STREAM_DRAW);

    glMatrixMode(GL_MODELVIEW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 7*sizeof(float), 0);
    glColorPointer(4, GL_FLOAT, 7*sizeof(float), (void *)(3 * sizeof(float)));
    glDrawArrays(GL_POINTS, 0, data.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

