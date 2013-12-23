// GLelipse.c

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <Box2D/Box2D.h>
#include "b2GLDraw.h"

double XMIN = 0;
double XMAX = 100;
double YMIN;
double YMAX;

b2World *world;

// global for box2d step
float32 timeStep = 1.0f / 60.f;
int32 velocityIterations = 10;
int32 positionIterations = 10;

void paint(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    world->Step(timeStep, velocityIterations, positionIterations);
    world->ClearForces();
    world->DrawDebugData();

    glFlush();
}

void scale(int w, int h) {
    double xrange = XMAX - XMIN;
    double yrange = h * xrange / w;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    YMIN = 0;
    YMAX = yrange;
    glOrtho(XMIN, XMAX, YMIN, YMAX, XMIN, XMAX);
}

void key(unsigned char c, int x, int y) {
    if (c == 'q') exit(0);
}

void rotate(void) {
    glutPostRedisplay();
}

void drawBox(double x, double y, int w, int h) {
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(x, y);
    myBodyDef.angle = 0;

    b2Body* dynamicBody = world->CreateBody(&myBodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(w, h);

    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 0.5;
    dynamicBody->CreateFixture(&boxFixtureDef);
}

void drawGround() {
    b2Body *groundBody;
    b2Fixture *bottomFixture;
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    groundBody = world->CreateBody(&groundBodyDef);

    b2EdgeShape groundBox;
    b2FixtureDef groundBoxDef;
    groundBoxDef.shape = &groundBox;

    groundBox.Set(b2Vec2(XMIN, YMIN), b2Vec2(XMAX, YMIN));
    bottomFixture = groundBody->CreateFixture(&groundBoxDef);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(60, 30);
    glutInitWindowSize(600, 600);
    glutCreateWindow("box2d");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    b2Vec2 gravity(0, -9.8);
    world = new b2World(gravity);

    // setup debugDraw
    b2GLDraw fooDrawInstance;
    world->SetDebugDraw(&fooDrawInstance);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;
    fooDrawInstance.SetFlags(flags);

    // draw bodies
    drawBox(5, 50, 2, 1);
    drawGround();

    glutDisplayFunc(paint);
    glutReshapeFunc(scale);
    glutKeyboardFunc(key);
    glutIdleFunc(rotate);
    glutMainLoop();
    return 0;
} // main
