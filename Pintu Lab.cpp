#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <gl/glut.h>
#define KEY_ESC 27 /* GLUT doesn't supply this */

#pragma region property
struct imageFile
{
    int width;
    int height;
    unsigned char *data;
};

// Globals.
static GLuint texture[4];
const int numTextures = 4; // Array of texture ids.
static float angle = 0.0;  // Angle to rotate textured square.
// Local storage untuk image data.
imageFile *images[numTextures];

int textureIndex = 1;
#pragma endregion

imageFile *getBMP(std::string fileName)
{
    int offset, // No. of bytes to start of image data in input BMP file.
        w,      // Width in pixels of input BMP file.
        h;      // Height in pixels of input BMP file.

    // Initialize imageFile objects.
    imageFile *tempStore = new imageFile; // Temporary storage.
    imageFile *outRGB = new imageFile;    // RGB output file.
    imageFile *outRGBA = new imageFile;   // RGBA output file.

    // Initialize input stream.
    std::ifstream inFile(fileName.c_str(), std::ios::binary);

    // Get start point of image data in input BMP file.
    inFile.seekg(10);
    inFile.read((char *)&offset, 4);

    // Get image width and height.
    inFile.seekg(18);
    inFile.read((char *)&w, 4);
    inFile.read((char *)&h, 4);

    // Determine the length of padding of the pixel rows
    // (each pixel row of a BMP file is 4-byte aligned by padding with zero bytes).
    int padding = (3 * w) % 4 ? 4 - (3 * w) % 4 : 0;

    // Allocate storage for temporary input file, read in image data from the BMP file, close input stream.
    tempStore->data = new unsigned char[(3 * w + padding) * h];
    inFile.seekg(offset);
    inFile.read((char *)tempStore->data, (3 * w + padding) * h);
    inFile.close();

    // Set image width and height and allocate storage for image in output RGB file.
    outRGB->width = w;
    outRGB->height = h;
    outRGB->data = new unsigned char[3 * w * h];

    // Copy data from temporary input file to output RGB file adjusting for padding and performing BGR to RGB conversion.
    int tempStorePos = 0;
    int outRGBpos = 0;
    for (int j = 0; j < h; j++)
        for (int i = 0; i < 3 * w; i += 3)
        {
            tempStorePos = (3 * w + padding) * j + i;
            outRGBpos = 3 * w * j + i;
            outRGB->data[outRGBpos] = tempStore->data[tempStorePos + 2];
            outRGB->data[outRGBpos + 1] = tempStore->data[tempStorePos + 1];
            outRGB->data[outRGBpos + 2] = tempStore->data[tempStorePos];
        }

    // Set image width and height and allocate storage for image in output RGBA file.
    outRGBA->width = w;
    outRGBA->height = h;
    outRGBA->data = new unsigned char[4 * w * h];

    // Copy image data from output RGB file to output RGBA file, setting all A values to 1.
    for (int j = 0; j < 4 * w * h; j += 4)
    {
        outRGBA->data[j] = outRGB->data[(j / 4) * 3];
        outRGBA->data[j + 1] = outRGB->data[(j / 4) * 3 + 1];
        outRGBA->data[j + 2] = outRGB->data[(j / 4) * 3 + 2];
        outRGBA->data[j + 3] = 0xFF;
    }

    // Release temporary storage and the output RGB file and return the RGBA version.
    delete[] tempStore;
    delete[] outRGB;
    return outRGBA;
}

// Load external textures.
void loadTextures()
{
    // Nama file tekstur
    const char *textureFiles[numTextures] = {
        "Textures/logo_pens.bmp",
        "Textures/pintu.bmp",
        "Textures/gagang.bmp",
        "Textures/tembok.bmp"
        // Tambahkan lebih banyak nama file tekstur sesuai kebutuhan
    };

    // Loop untuk memuat setiap tekstur
    for (int i = 0; i < numTextures; i++)
    {
        // Memuat gambar
        images[i] = getBMP(textureFiles[i]);

        // Membuat dan mengikat objek tekstur
        // glBindTexture(GL_TEXTURE_2D, texture[i]);
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[i]->width, images[i]->height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, images[i]->data);

        // Mengatur parameter wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Mengatur parameter filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}

float bukaTranslateX = 0;
float bukaTranslateZ = 0;
float bukaRotate = 0;
int totalOpen = 0;

void keyboard(unsigned char key, int x, int y)
{
    // Fungsi ini akan dipanggil ketika sebuah tombol keyboard ditekan

    if (key == 32) // ASCII untuk spasi adalah 32
    {
        if (totalOpen == -1)
        {
            bukaTranslateX = 0;
            bukaTranslateZ = 0;
            bukaRotate = 0;
            totalOpen = 0;
        }
        else if (totalOpen == 0)
        {
            bukaRotate = 45;
            bukaTranslateX = .1f;
            bukaTranslateZ = .2f;
            totalOpen++;
        }
        else if (totalOpen == 1)
        {
            bukaRotate = 90;
            bukaTranslateX = .3f;
            bukaTranslateZ = .3f;
            totalOpen = -1;
        }
        std::cout << "Buka: " << bukaTranslateX << std::endl; // Pesan debugging
        glutPostRedisplay();                                  // Memicu redisplay untuk memperbarui tampilan
    }
    if (key == '0')
    {
        textureIndex = 0;
    }
    if (key == '1')
    {
        textureIndex = 1;
    }
}

// Camera parameters
GLfloat cameraPosition[] = {0.0f, 0.0f, 3.0f};
GLfloat cameraRotation[] = {0.0f, 0.0f, 0.0f};
GLfloat cameraSpeed = 0.1f;
GLfloat cameraRotationSpeed = 1.0f;

// Mouse input
int lastMouseX, lastMouseY;
bool mouseLeftPressed = false;
bool mouseRightPressed = false;

// Timing variables for delta time calculation

float deltaTime = 0.0f;

void updateCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(-cameraPosition[0], -cameraPosition[1], -cameraPosition[2]); // untuk mengatur posisi camera
    glRotatef(cameraRotation[0], 1.0f, 0.0f, 0.0f);
    glRotatef(cameraRotation[1], 0.0f, 1.0f, 0.0f);
}

void drawCube()
{
    // glColor3f(0.6f, 0.4f, 0.2f);
    glutSolidCube(1.0);
}

void setupLight()
{
    // Positional light source
    GLfloat light_position[] = {2.0f, 2.0f, 2.0f, 1.0f}; // Position in homogeneous coordinates
    GLfloat light_diffuse[] = {0.1f, 0.1f, 0.1f, 1.0f};  // Dimmed white diffuse color

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // Enable light source 0
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

void tembok()
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glColor3f(0.2f, 0.12f, 0.0f);
    glTranslatef(.6, 0, 0);
    glScalef(0.6f, 1.0f, 0.01f);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glColor3f(0.2f, 0.12f, 0.0f);
    glTranslatef(-.6, 0, 0);
    glScalef(0.6f, 1.0f, 0.01f);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glColor3f(0.2f, 0.12f, 0.0f);
    glTranslatef(0, .75, 0);
    glScalef(1.8, .5f, 0.01f);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glColor3f(0.2f, 0.12f, 0.0f);
    glTranslatef(0, -.5, 0);
    glScalef(1.8, .05f, 0.01f);
    drawCube();
    glPopMatrix();
}

void lawang()
{

    // pintu utama gede
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[textureIndex]);
    glColor3f(0.2f, 0.12f, 0.0f);
    glScalef(0.6f, 1.0f, 0.01f);
    drawCube();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // 1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glColor3f(0.2f, 0.1f, 0.0f);
    glScalef(0.5f, 0.3f, 0.05f);
    drawCube();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glColor3f(0.2f, 0.1f, 0.0f);
    glScalef(0.5f, 0.3f, 0.05f);
    glTranslatef(0.0f, 1.05f, 0.0f);
    drawCube();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // 3
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glColor3f(0.2f, 0.1f, 0.0f);
    glScalef(0.5f, 0.3f, 0.05f);
    glTranslatef(0.0f, -1.05f, 0.0f);
    drawCube();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // basic gagang
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glColor3f(0.75f, 0.75f, 0.75f);
    glScalef(0.04f, 0.04f, 0.07f);
    glTranslatef(-5.0f, 0.0f, 0.1f);
    drawCube();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // gagang
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glColor3f(0.75f, 0.75f, 0.75f);
    glTranslatef(-0.17f, 0.0f, 0.05f);
    glScalef(0.1f, 0.02f, 0.02f);
    drawCube();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // lubang kunci
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glColor3f(0.75f, 0.75f, 0.75f);
    glScalef(0.04f, 0.04f, 0.07f);
    glTranslatef(-5.0f, -1.5f, 0.1f);
    drawCube();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

float doorAngle = 0.0f; // Initial angle of the door

void update(int value)
{
    // Update the door angle for opening animation
    doorAngle += 1.0f;

    glutPostRedisplay();          // Trigger a redisplay to update the view
    glutTimerFunc(16, update, 0); // 60 frames per second
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();

    glPushMatrix();
    glTranslatef(bukaTranslateX, 0, bukaTranslateZ);
    glRotatef(bukaRotate, 0.0f, 1.0f, 0.0f); // Putar pintu jika buka == 1
    // glRotatef(doorAngle, 0.0f, 1.0f, 0.0f);
    lawang();
    glPopMatrix();

    tembok();

    glutSwapBuffers();
}
// reshape, agar objek saat di rotasi, bentuknya tetap sama
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseLeftPressed = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP)
        {
            mouseLeftPressed = false;
        }
    }

    else if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseRightPressed = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP)
        {
            mouseRightPressed = false;
        }
    }
}

void motion(int x, int y)
{
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;
    lastMouseX = x;
    lastMouseY = y;

    // Camera rotation with left mouse button
    if (mouseLeftPressed)
    {
        cameraRotation[0] += dy * cameraRotationSpeed;
        cameraRotation[1] += dx * cameraRotationSpeed;
    }

    // Camera translation with right mouse button
    if (mouseRightPressed)
    {
        cameraPosition[0] += dx * cameraSpeed * deltaTime;
        cameraPosition[1] -= dy * cameraSpeed * deltaTime; // Adjust this based on your coordinate system
    }

    glutPostRedisplay(); // Trigger a redisplay to update the view
}

void setupLightingAndMaterial()
{
    GLfloat ambient[] = {1.0, 0.2, 0.2, 1};
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1};
    GLfloat specular[] = {1.0, 1.0, 1.0, 1};
    GLfloat shininess = 150.0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void setup(void)
{
    glClearColor(0.0, 1.0, 0.0, 0.0);

    setupLightingAndMaterial();
    // Create texture ids.
    glGenTextures(2, texture);

    // Load external texture.
    loadTextures();

    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // Turn on OpenGL texturing.
    // glEnable(GL_TEXTURE_2D);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cube with Camera and Lighting");
    setup();

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}
