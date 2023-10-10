#include <GL/glut.h>
#include <cmath>
#include <chrono>

// Camera parameters
GLfloat cameraPosition[] = { 0.0f, 0.0f, 3.0f };
GLfloat cameraRotation[] = { 0.0f, 0.0f, 0.0f };
GLfloat cameraSpeed = 0.1f;
GLfloat cameraRotationSpeed = 1.0f;

// Mouse input
int lastMouseX, lastMouseY;
bool mouseLeftPressed = false;
bool mouseRightPressed = false;

// Timing variables for delta time calculation
std::chrono::high_resolution_clock::time_point lastFrameTime;
float deltaTime = 0.0f;

void updateCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(-cameraPosition[0], -cameraPosition[1], -cameraPosition[2]);
    glRotatef(cameraRotation[0], 1.0f, 0.0f, 0.0f);
    glRotatef(cameraRotation[1], 0.0f, 1.0f, 0.0f);
}

void drawCube() {
    //glColor3f(0.6f, 0.4f, 0.2f);
    glutSolidCube(1.0);
}

void setupLight() {
    // Positional light source
    GLfloat light_position[] = { 2.0f, 2.0f, 2.0f, 1.0f }; // Position in homogeneous coordinates
    GLfloat light_diffuse[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Dimmed white diffuse color

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // Enable light source 0
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();

    //setupLight(); // Enable and set up the light

    // Set the material properties of the cube
    //cube_diffuse[] = { 0.3f, 0.2f, 0.1f, 1.0f }; // Adjust the values to control cube brightness
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_diffuse);
    //pintu utama gede
    glPushMatrix();
    glColor3f(0.2f, 0.12f, 0.0f);
    glScalef(0.6f, 1.0f, 0.01f);
    drawCube();
    glPopMatrix();

    //1
    glPushMatrix();
    glColor3f(0.2f, 0.1f, 0.0f);
    glScalef(0.5f, 0.3f, 0.05f);
    drawCube();
    glPopMatrix();

    //2
    glPushMatrix();
    glColor3f(0.2f, 0.1f, 0.0f);
    glScalef(0.5f, 0.3f, 0.05f);
    glTranslatef(0.0f, 1.05f, 0.0f);
    drawCube();
    glPopMatrix();

    //3
    glPushMatrix();
    glColor3f(0.2f, 0.1f, 0.0f);
    glScalef(0.5f, 0.3f, 0.05f);
    glTranslatef(0.0f, -1.05f, 0.0f);
    drawCube();
    glPopMatrix();

    //basic gagang
    glPushMatrix();
    glColor3f(0.75f, 0.75f, 0.75f);
    glScalef(0.04f, 0.04f, 0.07f);
    glTranslatef(-5.0f, 0.0f, 0.1f);
    drawCube();
    glPopMatrix();

    //gagang
    glPushMatrix();
    glColor3f(0.75f, 0.75f, 0.75f);
    glTranslatef(-0.17f, 0.0f, 0.05f);
    glScalef(0.1f, 0.02f, 0.02f);
    drawCube();
    glPopMatrix();

//lubang kunci
    glPushMatrix();
    glColor3f(0.75f, 0.75f, 0.75f);
    glScalef(0.04f, 0.04f, 0.07f);
    glTranslatef(-5.0f, -1.5f, 0.1f);
    drawCube();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseLeftPressed = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP) {
            mouseLeftPressed = false;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseRightPressed = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP) {
            mouseRightPressed = false;
        }
    }
}

void motion(int x, int y) {
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;
    lastMouseX = x;
    lastMouseY = y;

    // Camera rotation with left mouse button
    if (mouseLeftPressed) {
        cameraRotation[0] += dy * cameraRotationSpeed;
        cameraRotation[1] += dx * cameraRotationSpeed;
    }

    // Camera translation with right mouse button
    if (mouseRightPressed) {
        cameraPosition[0] += dx * cameraSpeed * deltaTime;
        cameraPosition[1] -= dy * cameraSpeed * deltaTime; // Adjust this based on your coordinate system
    }

    glutPostRedisplay(); // Trigger a redisplay to update the view
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        cameraPosition[2] -= cameraSpeed * deltaTime;
        break;
    case 's':
        cameraPosition[2] += cameraSpeed * deltaTime;
        break;
    case 'a':
        cameraPosition[0] -= cameraSpeed * deltaTime;
        break;
    case 'd':
        cameraPosition[0] += cameraSpeed * deltaTime;
        break;
    }
}

void updateDeltaTime() {
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
    lastFrameTime = currentTime;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cube with Camera and Lighting");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    // Initialize lastFrameTime for delta time calculation
    lastFrameTime = std::chrono::high_resolution_clock::now();

    glutMainLoop();

    return 0;
}
