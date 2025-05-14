#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include "ReadBMP.h"
#include "ImageManipulation.h"
#include <math.h>
#include <vector>
using namespace std;

int ScreenWindowWidth = 800;
int ScreenWindowHeight = 600;

typedef GLfloat VertexArray[3];
typedef GLfloat NormalArray[3];

GLfloat Xpos = 150.0;
GLfloat Ypos = 0.0;
GLfloat Zpos = 0.0;
GLfloat Xangle = 0.0;
GLfloat Yangle = 0.0;
GLfloat XangInc = 5;
GLfloat YangInc = 5;
double scaleX = 1.0;
double scaleY = 1.0;
double scaleZ = 1.0;
static bool mousePanMode = false;
static bool mouseZoomMode = false;
static bool mouseRotationMode = false;
int mouseX, mouseY;

GLuint textureNumber[7];
// Rotation angle for each of the planets.
static float earthRotationAngle = 0.0;
static float moonRotationAngle = 0.0;
static float moonRevolutionAngle = 0.0;
static float sunRotationAngle = 0.0;
static float mercuryRotationAngle = 0.0;
static float venusRotationAngle = 0.0;
static float machineGunRotationAngle = 0.0f; // machine gun for rotating.
static int bulletCount = 10; // The capacity for missiles.
GLfloat cameraDistance = 6.0f; // The distance of the camera.
GLfloat cameraHeight = 3.0f; // The height of the camera.

// Function to draw the missiles.
void drawMissile()
{
    float tubeRadius = 0.1f; // Inner tubes.
    float tubeSpacing = 0.4f; // Distance from the center to the center of a tube.

    for (int i = 0; i < 4; i++)
    {
        float angle = i * 3.14159f / 2.0f;
        float x = cos(angle) * tubeSpacing;
        float y = sin(angle) * tubeSpacing;

        glPushMatrix();
        glRotatef(machineGunRotationAngle, 0.0f, 0.0f, 0.01f);
        glTranslatef(x, y, 0);
        glTranslatef(0, 0, -0.5f);

        // Missile.
        GLUquadricObj *pObj = gluNewQuadric();
        gluQuadricNormals(pObj, GLU_SMOOTH);
        glColor3f(0, 1.0f, 0);

        glPushMatrix();

        glTranslatef(0, 0, -0.3f);
        gluCylinder(pObj, 0.15f, 0.15f, 1.0f, 26, 13);
        gluDisk(pObj, 0, 0.15f, 26, 13);
        glPopMatrix();

        gluDeleteQuadric(pObj);

        glPopMatrix();
    }
}
//-----------------------------------------------------------------------------------
// A class that store the missile objects.
class Missile
{
public:
    float zPosition = 0.0f; // Z position of the missile object.

    // Constructor.
    Missile()
    {
        glPushMatrix();
        drawMissile();
        glTranslatef(5.0f, 0, 0);
        drawMissile();
        glPopMatrix();
        glFlush();
    }

    // A method increase the z position of the missile object.
    void shoot()
    {
        zPosition -= 1.0f;
    }
};

vector<Missile> missiles; // A vector to store the missile objects.
//-----------------------------------------------------------------------------------

// A function to shoot the missile.
void launchMissiles()
{
    Missile missile;
    missiles.push_back(missile);
}

// A function that turns the machine gun.
void myIdleFunc()
{
    // Increase rotation angle (adjust the speed as needed).
    machineGunRotationAngle += 0.5f;
    // Ensure angle wraps around within 360 degrees.
    if (machineGunRotationAngle >= 360.0f)
        machineGunRotationAngle -= 360.0f;

    Xpos += sin(Yangle * M_PI / 180.0) * 0.1;
    Zpos += cos(Yangle * M_PI / 180.0) * 0.1;
    // Trigger redisplay to animate.
    glutPostRedisplay();
}

// Body of the plane.
void PlaneBody()
{
    GLUquadricObj *pObj;
    pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);
    glColor3f(1, 1, 1);

    glPushMatrix();
    glScalef(0.05, 0.05, 0.05);
    gluCylinder(pObj, 1.0f, 1.0f, 10.0f, 26, 13);
    glTranslatef(0, 0, 10);
    gluDisk(pObj, 0, 1.0, 26, 13);
    glColor3f(0.5, 0.5, 0.5);
    gluCylinder(pObj, 0.5f, 0.5f, 0.5f, 26, 13);
    glColor3f(1, 0, 0);
    glTranslatef(0, 0, 0.5);
    gluDisk(pObj, 0, 0.5, 26, 13);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.085, 0.085, 0.085);
    glColor3f(0.5, 0.5, 0.5);
    glScalef(1, 1, 3);
    gluSphere(pObj, 1.0f, 26, 26);

    glPopMatrix();
    gluDeleteQuadric(pObj);
}

// Left wing for the plane.
void PlaneWing1(float r, float g, float b)
{

    glColor3f(r, g, b);

    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(0, 4, 0);
    glVertex3f(7, 1, 12);
    glVertex3f(9, 1, 12);
    glVertex3f(8, 4, 0);

    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(7, 0, 12);
    glVertex3f(9, 0, 12);
    glVertex3f(8, 0, 0);

    glNormal3f(0, 0, 1);
    glVertex3f(0, 4, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(7, 0, 12);
    glVertex3f(7, 1, 12);

    glNormal3f(1, 0, 0);
    glVertex3f(7, 1, 12);
    glVertex3f(7, 0, 12);
    glVertex3f(9, 0, 12);
    glVertex3f(9, 1, 12);

    glNormal3f(-1, 0, 0);
    glVertex3f(8, 0, 0);
    glVertex3f(8, 4, 0);
    glVertex3f(9, 1, 12);
    glVertex3f(9, 0, 12);
    glEnd();
}

// Right wing for the plane.
void PlaneWing2(float r, float g, float b)
{

    glColor3f(r, g, b);

    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(0, 4, 0);
    glVertex3f(7, 1, 12);
    glVertex3f(9, 1, 12);
    glVertex3f(8, 4, 0);

    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(7, 0, 12);
    glVertex3f(9, 0, 12);
    glVertex3f(8, 0, 0);

    glNormal3f(0, 0, 1);
    glVertex3f(0, 4, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(7, 0, 12);
    glVertex3f(7, 1, 12);

    glNormal3f(1, 0, 0);
    glVertex3f(7, 1, 12);
    glVertex3f(7, 0, 12);
    glVertex3f(9, 0, 12);
    glVertex3f(9, 1, 12);

    glNormal3f(-1, 0, 0);
    glVertex3f(8, 0, 0);
    glVertex3f(8, 4, 0);
    glVertex3f(9, 1, 12);
    glVertex3f(9, 0, 12);
    glEnd();
}

// Draw the wings for the plane.
void drawWing()
{
    glPushMatrix();
    glTranslatef(0, -0.8, -2);
    glRotatef(-90, 0, 1, 0);
    glScalef(0.5, 0.3, 1);
    PlaneWing1(0.25f, 0.25f, 0.25f);

    glTranslatef(13.5, 0, 0);
    glScalef(0.8, 1, 0.5);
    PlaneWing2(0.75f, 0.75f, 0.75f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -0.8, -2);
    glRotatef(-90, 0, 1, 0);
    glScalef(0.5, 0.3, -1);
    PlaneWing1(0.25f, 0.25f, 0.25f);

    glTranslatef(13.5, 0, 0);
    glScalef(0.8, 1, 0.5);
    PlaneWing2(0.75f, 0.75f, 0.75f);
    glPopMatrix();
}

// Draw the vertical stabilizer for the plane.
void VerticalWing()
{
    glPushMatrix();
    glScalef(0.05, 0.05, 0.05);
    glTranslatef(0.6, 0.5, 7.5);
    glRotatef(-90, 0, 1, 0);
    glScalef(0.3, 0.3, 0.3);
    glBegin(GL_QUADS);
    glColor3f(0, 0, 1);
    glVertex3f(2, 7, 2);
    glVertex3f(0, 0, 4);
    glVertex3f(8, 0, 4);
    glVertex3f(6, 7, 2);

    glVertex3f(2, 7, 2);
    glVertex3f(0, 0, 0);
    glVertex3f(8, 0, 0);
    glVertex3f(6, 7, 2);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);
    glVertex3f(2, 7, 2);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 4);

    glVertex3f(6, 7, 2);
    glVertex3f(8, 0, 0);
    glVertex3f(8, 0, 4);
    glEnd();
    glPopMatrix();
}

// Draw the machine guns for the plane.
void drawMachineGun()
{
    GLUquadricObj *pObj;
    pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);
    glColor3f(0.1f, 0.1f, 0.1f);

    // Main cylinder
    glPushMatrix();
    gluCylinder(pObj, 0.7f, 0.7f, 1.9f, 26, 13); // Main cylinder.
    glTranslatef(0, 0, 1.5f);
    gluDisk(pObj, 0, 0.7f, 26, 13); // Cap for the main cylinder.
    glPopMatrix();

    gluDeleteQuadric(pObj);
}

// The connect points for machine gun.
void ShortCylinder()
{
    GLUquadricObj *pObj;
    pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);
    glColor3f(0.5f, 0.5f, 0.5f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(pObj, 0.2f, 0.2f, 0.9f, 20, 20); // Adjust slices and stacks as needed.

    gluDeleteQuadric(pObj);
}

// Draw the full plane.
void DrawPlane()
{
    glRotatef(Yangle, 0.0f, 1.0f, 0.0f);
    PlaneBody();
    // Draw Left Wing
    glPushMatrix();
    glScalef(0.05, 0.05, 0.05);
    glTranslatef(-2.5f, 0.0f, 2.0f);
    drawWing();
    glPopMatrix();

    // Draw Right Wing
    glPushMatrix();
    glScalef(0.05, 0.05, 0.05);
    glTranslatef(2.5f, 0.0f, 2.0f);
    drawWing();
    glPopMatrix();

    VerticalWing();

    // Left machine gun.
    glPushMatrix();
    glScalef(0.05, 0.05, 0.05);
    glTranslatef(-2.5f, -2.3f, 1.0f);
    drawMachineGun();
    drawMissile();
    // Update the shot missiles' position.
    for (int i = 0; i < missiles.size(); i++)
    {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, missiles[i].zPosition);
        drawMissile();
        glPopMatrix();
    }
    glPopMatrix();

    // Right machine gun.
    glPushMatrix();
    glScalef(0.05, 0.05, 0.05);
    glTranslatef(2.5f, -2.3f, 1.0f);
    drawMachineGun();
    drawMissile();
    // Update the shot missiles' position.
    for (int i = 0; i < missiles.size(); i++)
    {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, missiles[i].zPosition);
        drawMissile();
        glPopMatrix();
    }
    glPopMatrix();

    // Draw the connect points.
    glPushMatrix();
    glScalef(0.05, 0.05, 0.05);
    glTranslatef(2.5f, -0.7f, 1.7f);
    ShortCylinder(); // Draw a gray cylinder.
    glPopMatrix();

    glPushMatrix();
    glScalef(0.05, 0.05, 0.05);
    glTranslatef(-2.5f, -0.7f, 1.7f);
    ShortCylinder(); // Draw a gray cylinder.
    glPopMatrix();

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

// Draw the sphere for the planets.
void DrawSphere(GLuint texture, float radius, int slices, int stacks)
{
    GLUquadric *quad = gluNewQuadric();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluQuadricTexture(quad, GL_TRUE);

    GLfloat planet_ambient[] = {0.2, 0.2, 0.2, 1.0};  // Ambient reflection.
    GLfloat planet_diffuse[] = {0.8, 0.8, 0.8, 1.0};  // Diffuse reflection.
    GLfloat planet_specular[] = {1.0, 1.0, 1.0, 1.0}; // Specular reflection.
    GLfloat planet_shininess = 50.0;                  // Shininess factor.

    glMaterialfv(GL_FRONT, GL_AMBIENT, planet_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, planet_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, planet_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, planet_shininess);

    gluSphere(quad, radius, slices, stacks);
    gluDeleteQuadric(quad);
    glDisable(GL_TEXTURE_2D);
}

// Draw asteroids.
void asteroids()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureNumber[5]);

    glPushMatrix();
    glScalef(0.15 * scaleX, 0.15 * scaleY, 0.15 * scaleZ);

    // Front face.
    glBegin(GL_POLYGON);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 1.2f, 0.7f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-1.1f, 0.8f, 0.6f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.5f, -0.4f, 0.0f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(-0.5f, -1.4f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.6f, -1.6f, 0.0f);
    glTexCoord2f(1.0f, 0.5f);
    glVertex3f(1.3f, -0.9f, 0.4f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(1.6f, 0.6f, -0.3f);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.7f, 1.5f, 0.5f);
    glEnd();

    // Back face.
    glBegin(GL_POLYGON);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 1.2f, -0.7f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-1.1f, 0.8f, -0.6f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.5f, -0.4f, 0.1f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(-0.5f, -1.4f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.6f, -1.6f, 0.3f);
    glTexCoord2f(1.0f, 0.5f);
    glVertex3f(1.3f, -0.9f, -0.3f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(1.6f, 0.6f, 0.5f);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.7f, 1.5f, -0.5f);
    glEnd();

    // Connecting faces.
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.0f, 1.2f, 0.7f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.0f, 1.2f, -0.7f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.1f, 0.8f, -0.6f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.1f, 0.8f, 0.6f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.1f, 0.8f, 0.6f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.1f, 0.8f, -0.6f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.5f, -0.4f, 0.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.5f, -0.4f, 0.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.5f, -0.4f, 0.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, -1.4f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-0.5f, -1.4f, 0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.5f, -0.4f, 0.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, -1.4f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, -1.4f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.6f, -1.6f, 0.3f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.6f, -1.6f, 0.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.6f, -1.6f, 0.3f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.6f, -1.6f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.3f, -0.9f, -0.3f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.3f, -0.9f, 0.4f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.3f, -0.9f, 0.4f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.3f, -0.9f, -0.3f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.6f, 0.6f, 0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.6f, 0.6f, -0.3f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.6f, 0.6f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.6f, 0.6f, -0.3f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.7f, 1.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.7f, 1.5f, 0.5f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.7f, 1.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.7f, 1.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, 1.2f, -0.7f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 1.2f, 0.7f);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glFlush();
}

// Draw satellite.
void satellite()
{
    GLUquadricObj *pObj;
    pObj = gluNewQuadric();

    glRotatef(180, 0, 1, 0);
    glScalef(3, 3, 3);

    // Satellite Body.
    glPushMatrix();
    glColor3f(0.6, 0.6, 0.6);
    glTranslatef(0, 0, -0.3);
    gluCylinder(pObj, 0.2f, 0.2f, 0.6f, 32, 32);
    gluDisk(pObj, 0, 0.2f, 32, 32);
    glTranslatef(0, 0, 0.6);
    gluDisk(pObj, 0, 0.2f, 32, 32);
    glPopMatrix();

    // Satellite Panels.
    glPushMatrix();
    glTranslatef(0, 0.0, 0.0);
    glScaled(3.7, 0.1, 1.0);
    glutSolidCube(0.4);
    glPopMatrix();

    // Solar Panel Head.
    glPushMatrix();
    glTranslatef(0, 0.0, -0.6);
    glColor3f(0.5, 0.5, 0.5);
    gluCylinder(pObj, 0.2f, 0.2f, 0.2f, 26, 13);
    glTranslatef(0, 0.0, 0.2);
    glColor3f(1, 1, 1);
    gluDisk(pObj, 0, 0.2f, 26, 13);
    glPopMatrix();

    // Solar Panel Torus.
    glPushMatrix();
    glColor3f(0.2, 0.2, 0.1);
    glTranslatef(0, 0.0, 0.6);
    glScaled(0.4, 0.4, 0.3);
    glutSolidTorus(0.1, 0.2, 20, 20);
    glPopMatrix();
    glFlush();
}

// A function that draws multiple asteroids around the earth.
void DrawAsteroid()
{
    glPushMatrix();
    glTranslatef(0 * scaleX, 12 * scaleY, 0 * scaleZ);
    asteroids();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(6 * scaleX, 12 * scaleY, 0 * scaleZ);
    asteroids();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0 * scaleX, 12 * scaleY, 8 * scaleZ);
    asteroids();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(7 * scaleX, 11 * scaleY, 8 * scaleZ);
    asteroids();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(6 * scaleX, 11 * scaleY, 9 * scaleZ);
    glRotatef(90, 0, 0, 1);
    asteroids();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(7 * scaleX, 11 * scaleY, 8 * scaleZ);
    glRotatef(-90, 0, 0, 1);
    asteroids();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(7 * scaleX, 14 * scaleY, 8 * scaleZ);
    glRotatef(-90, 1, 0, 0);
    asteroids();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0 * scaleX, 13 * scaleY, 0 * scaleZ);
    glRotatef(90, 1, 0, 0);
    asteroids();
    glPopMatrix();
    glFlush();
}

// A function that draws multiple satellites around the earth.
void DrawSatellite()
{
    glPushMatrix();
    glTranslatef(0 * scaleX, 14 * scaleY, 0 * scaleZ);
    glScalef(0.2 * scaleX, 0.2 * scaleY, 0.2 * scaleZ);
    satellite();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0 * scaleX, -14 * scaleY, 0 * scaleZ);
    glRotatef(180, 1, 0, 0);
    glScalef(0.2 * scaleX, 0.2 * scaleY, 0.2 * scaleZ);
    satellite();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(11 * scaleX, 13 * scaleY, 0 * scaleZ);
    glScalef(0.2 * scaleX, 0.2 * scaleY, 0.2 * scaleZ);
    satellite();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(12 * scaleX, -13 * scaleY, 0 * scaleZ);
    glRotatef(180, 1, 0, 0);
    glScalef(0.2 * scaleX, 0.2 * scaleY, 0.2 * scaleZ);
    satellite();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(13 * scaleX, 11 * scaleY, 0 * scaleZ);
    glRotatef(90, 0, 0, 1);
    glScalef(0.2 * scaleX, 0.2 * scaleY, 0.2 * scaleZ);
    satellite();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-12 * scaleX, 12 * scaleY, 0 * scaleZ);
    glRotatef(-90, 0, 0, 1);
    glScalef(0.2 * scaleX, 0.2 * scaleY, 0.2 * scaleZ);
    satellite();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0 * scaleX, 0 * scaleY, -10 * scaleZ);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.2 * scaleX, 0.2 * scaleY, 0.2 * scaleZ);
    satellite();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10 * scaleX, 0 * scaleY, 10 * scaleZ);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2 * scaleX, 0.2 * scaleY, 0.2 * scaleZ);
    satellite();
    glPopMatrix();
    glFlush();
}

// Apply the texture for the background.
void DrawBackground()
{
    // Save the current matrix mode and matrices.
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Set up an orthographic projection.
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);

    // Switch to the modelview matrix and reset it.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth testing and depth writing.
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureNumber[6]);

    // Draw the textured quad.
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(0.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(800.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex2f(800.0, 600.0);
    glTexCoord2f(0.0, 1.0);
    glVertex2f(0.0, 600.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // Re-enable depth testing and depth writing.
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    // Restore the previous matrices.
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Draw the disk shape for the UFO.
void drawDisk(float innerRadius, float outerRadius, int slices, int loops) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluDisk(quad, innerRadius, outerRadius, slices, loops);
    gluDeleteQuadric(quad);
}

// Draw UFO.
void drawUFO() {
    glPushMatrix();
        glColor3f(0.6f, 0.6f, 0.6f);

        // Bottom part of the UFO (disk).
        glPushMatrix();
        glTranslatef(0.0f, -0.5f, 0.0f);
        glRotatef(-90, 1.0f, 0.0f, 0.0f); // Rotate to align with the XZ plane.
        drawDisk(0.0f, 1.5f, 50, 50); // Larger disk.
        glPopMatrix();

        // Middle part of the UFO (tapered cylinder).
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        glRotatef(-90, 1.0f, 0.0f, 0.0f); // Rotate to align with the XZ plane.
        GLUquadric* quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluCylinder(quad, 1.5f, 0.5f, 1.0f, 50, 50); // Tapered middle part.
        gluDeleteQuadric(quad);
        glPopMatrix();

        // Top dome of the UFO.
        glPushMatrix();
        glTranslatef(0.0f, 0.5f, 0.0f);
        glScalef(0.5f, 0.25f, 0.5f); // Scale to make it a dome.
        glutSolidSphere(1.0f, 50, 50); // Top dome.
        glPopMatrix();

        // Lights around the middle.
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for the lights.
        for (int i = 0; i < 360; i += 30) {
            float angle = i * M_PI / 180.0f;
            float x = 1.4f * cos(angle);
            float z = 1.4f * sin(angle);
            glPushMatrix();
            glTranslatef(x, 0.0f, z);
            glutSolidSphere(0.1f, 20, 20);
            glPopMatrix();
        }

        // Lights on the bottom.
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for the bottom lights.
        for (int i = 0; i < 360; i += 45) {
            float angle = i * M_PI / 180.0f;
            float x = 1.2f * cos(angle);
            float z = 1.2f * sin(angle);
            glPushMatrix();
            glTranslatef(x, -0.5f, z);
            glutSolidSphere(0.1f, 20, 20);
            glPopMatrix();
        }
    glPopMatrix();
}

// Draw all the planets, satellites and the asteroids.
void DrawPlanet()
{
    glPushMatrix();
        // Draw Sun
        glRotatef(sunRotationAngle, 0.0, 1.0, 0.0); // Sun's rotation.

        GLfloat sun_position[] = {0.0, 0.0, 0.0, 1.0}; // Position at the center.
        GLfloat sun_ambient[] = {0.8, 0.8, 0.0, 1.0};  // Ambient light color (yellowish).
        GLfloat sun_diffuse[] = {1.0, 1.0, 0.0, 1.0};  // Diffuse light color (yellowish).
        GLfloat sun_specular[] = {1.0, 1.0, 1.0, 1.0}; // Specular light color (white).

        glLightfv(GL_LIGHT0, GL_POSITION, sun_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, sun_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, sun_specular);

        glEnable(GL_LIGHT0);

        DrawSphere(textureNumber[0], 48.0 * scaleX, 52, 26); // Adjust Sun size.

        glPushMatrix();
        glTranslatef(0.0f, 70.0f * scaleX, 0.0f); // Adjust height above the sun.
        glScalef(10*scaleX,10*scaleY,10*scaleZ);
        drawUFO();
        glPopMatrix();

        // Draw Mercury
        glPushMatrix();
        glRotatef(mercuryRotationAngle, 0.0, 1.0, 0.0);     // Mercury's rotation.
        glTranslatef(80.0 * scaleX, 0.0, 0.0);              // Position Mercury.
        DrawSphere(textureNumber[1], 4.0 * scaleX, 40, 20); // Adjust Mercury size.
        glPopMatrix();

        // Draw Venus
        glPushMatrix();
        glRotatef(venusRotationAngle, 0.0, 1.0, 0.0);        // Venus's rotation.
        glTranslatef(120.0 * scaleX, 0.0, 0.0);              // Position Venus.
        DrawSphere(textureNumber[2], 11.2 * scaleX, 40, 20); // Adjust Venus size.
        glPopMatrix();

        // Draw Earth
        glPushMatrix();
            glRotatef(moonRevolutionAngle, 0.0, 1.0, 0.0);      // Earth's revolution around the Sun.
            glTranslatef(160.0 * scaleX, 0.0, 0.0);             // Position Earth.
            glRotatef(earthRotationAngle, 0.0, 1.0, 0.0);       // Earth's rotation.
            DrawSphere(textureNumber[3], 9.6 * scaleX, 52, 26); // Adjust Earth size.
            DrawSatellite();
            DrawAsteroid();

            // Draw Moon
            glPushMatrix();
            glRotatef(moonRevolutionAngle, 0.0, 1.0, 0.0);      // Moon's revolution around the Earth.
            glTranslatef(20.0 * scaleX, 0.0, 0.0);              // Position the Moon.
            glRotatef(moonRotationAngle, 0.0, 1.0, 0.0);        // Moon's rotation around its axis.
            DrawSphere(textureNumber[4], 2.6 * scaleX, 40, 20); // Adjust Moon size.
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

// A function to handle the camera position and keep track on the plane.
void camera()
{
    // Calculate the camera position to track with the plane position.
    GLfloat camX = Xpos - sin(Yangle * M_PI / 180.0) * cameraDistance;
    GLfloat camY = Ypos + cameraHeight;
    GLfloat camZ = Zpos - cos(Yangle * M_PI / 180.0) * cameraDistance;

    // Set the camera to look at the plane.
    gluLookAt(camX, camY, camZ, Xpos, Ypos, Zpos, 0.0, 1.0, 0.0);
    // Draw the plane.
    glPushMatrix();
    glTranslatef(Xpos, Ypos, Zpos);
    glRotatef(-180, 0.0, 1.0, 0.0);
    DrawPlane();
    glPopMatrix();
}

// Display function.
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Draw all the models.
    camera();
    DrawBackground();
    DrawPlanet();
    glFlush();
    glutSwapBuffers();
}

// Adjust the vision.
void myReshape(int width, int height)
{
    static GLfloat fieldOfViewAngle = 80.0;
    static GLfloat zNear = 0.1;
    static GLfloat zFar = 500.0;

    ScreenWindowWidth = width;
    ScreenWindowHeight = height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfViewAngle, (GLfloat)width / (GLfloat)height, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Initialize function.
void myInit(void)
{
    MyImageStruct myImage = {0, 0, 0};
    char *fileNames[] = {"bmp/sun.bmp", "bmp/mercury.bmp", "bmp/venus.bmp", "bmp/earth.bmp", "bmp/moon.bmp", "bmp/asteroid.bmp", "bmp/space.bmp"};

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glGenTextures(7, textureNumber);
    for (int j = 0; j < 7; j++)
    {
        ReadBMP(fileNames[j], &myImage);
        CropImageToPowerOf2(&myImage, 128, 128);
        glBindTexture(GL_TEXTURE_2D, textureNumber[j]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myImage.Width, myImage.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, myImage.Pixels);
        free(myImage.Pixels);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
}

// Keyboard key functions.
void myKeyboard(unsigned char key, int x, int y)
{
    static const GLdouble posInc = 2;
    GLdouble radianAngle = Yangle * M_PI / 180.0;

    switch (key)
    {
    case 'w':
    case 'W':
        Zpos += posInc * cos(radianAngle);
        Xpos += posInc * sin(radianAngle);
        break;
    case 's':
    case 'S':
        Zpos -= posInc * cos(radianAngle);
        Xpos -= posInc * sin(radianAngle);
        break;
    case 'a':
    case 'A':
        Xpos += posInc * cos(radianAngle);
        Zpos -= posInc * sin(radianAngle);
        break;
    case 'd':
    case 'D':
        Xpos -= posInc * cos(radianAngle);
        Zpos += posInc * sin(radianAngle);
        break;
    case 'q':
    case 'Q':
        Yangle += YangInc;
        break;
    case 'e':
    case 'E':
        Yangle -= YangInc;
        break;
    case 'f':
    case 'F':
        if (bulletCount > 0)
        {
            launchMissiles();
            bulletCount--;
        }
        break;
    case 'r':
    case 'R':
        bulletCount = 10;
        break;
    case 27:
        exit(1);
        break;
    }
    glutPostRedisplay();
}

// Mouse Functions.
void myMouse(int button, int state, int x, int y)
{
    y = ScreenWindowHeight - y;
    switch (button)
    {
    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_DOWN && !mousePanMode)
        {
            mouseX = x;
            mouseY = y;
            mousePanMode = true;
        }
        if (state == GLUT_UP && mousePanMode)
        {
            mousePanMode = false;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN && !mouseZoomMode)
        {
            mouseY = y;
            mouseZoomMode = true;
        }
        if (state == GLUT_UP && mouseZoomMode)
        {
            mouseZoomMode = false;
        }
        break;
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN && !mouseRotationMode)
        {
            mouseX = x;
            mouseY = y;
            mouseRotationMode = true;
        }
        if (state == GLUT_UP && mouseRotationMode)
        {
            mouseRotationMode = false;
        }
        break;

        glutPostRedisplay();
    }
}

// Mouse moving functions.
void myMotion(int x, int y)
{
    y = ScreenWindowHeight - y;
    if (mousePanMode)
    {
        Xpos += (x - mouseX) * 0.01;
        Ypos += (y - mouseY) * 0.01;
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }
    if (mouseZoomMode)
    {
        GLdouble inc = (mouseY - y) * 0.01;
        scaleX += inc;
        scaleY += inc;
        scaleZ += inc;
        if (scaleX < 0.1)
            scaleX = 0.1;
        if (scaleY < 0.1)
            scaleY = 0.1;
        if (scaleZ < 0.1)
            scaleZ = 0.1;
        mouseY = y;
        glutPostRedisplay();
    }
    if (mouseRotationMode)
    {
        Xangle -= (mouseY - y) * 0.5;
        Yangle -= (x - mouseX) * 0.5;
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }
}

// Keyboard special key functions.
void mySpecialKey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_DOWN:
        Ypos -= 1;
        break;
    case GLUT_KEY_UP:
        Ypos += 1;
        break;
    case GLUT_KEY_HOME:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case GLUT_KEY_END:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    }
    glutPostRedisplay();
}

// Update all the animation for every time repaint.
void update(int value)
{
    sunRotationAngle += 0.2f;
    if (sunRotationAngle > 360)
        sunRotationAngle -= 360;

    mercuryRotationAngle += 0.3f;
    if (mercuryRotationAngle > 360)
        mercuryRotationAngle -= 360;

    venusRotationAngle += 0.25f;
    if (venusRotationAngle > 360)
        venusRotationAngle -= 360;

    earthRotationAngle += 0.3f;
    if (earthRotationAngle > 360)
        earthRotationAngle -= 360;

    moonRotationAngle += 0.5f; // Synchronous rotation with revolution
    if (moonRotationAngle > 360)
        moonRotationAngle -= 360;

    moonRevolutionAngle += 0.1f;
    if (moonRevolutionAngle > 360)
        moonRevolutionAngle -= 360;

    // Update the position for all the missile objects.
    for (int i = 0; i < missiles.size();)
    {
        missiles[i].shoot();
        // Remove the missile when flied over 70 units.
        if (missiles[i].zPosition < -70.0f)
        {
            missiles.erase(missiles.begin() + i);
        }
        else
        {
            i++;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(ScreenWindowWidth, ScreenWindowHeight);
    glutCreateWindow("Space");
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecialKey);
    glutMotionFunc(myMotion);
    glutMouseFunc(myMouse);
    glutIdleFunc(myIdleFunc);
    myInit();
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
