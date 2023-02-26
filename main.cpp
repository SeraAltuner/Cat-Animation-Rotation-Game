/*********
FEATURES: 2 different cats with different themes move according to user choices made by arrow keys, has an animated background, characters can be moved manually or
automatically depending on which one the user selects, characters can be relocated with mouse click
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>


#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define START 0 //defining variables for appState variable
#define SHOW 1

#define MANUAL 0 //definig varibles for modeState variable
#define AUTO 1

#define NOT 0 //defining varibles for keyState variable
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define KUROMI 0 //defining variables for charState variable
#define MELODY 1


#define STARTING_SCREEN 0 //defining variables for backgroundState variable
#define CLICKED_SCREEN 1

#define TIMER_PERIOD  16 // Period for the timer.

#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height


int backgroundState = STARTING_SCREEN; //to control if the opening (initial screen) will be shown or if it will change to a background that a character has

int appState = START;   //to control whether the character will appear or not; 0 : only background, 1 : character apperaing on the clicked location

int modeState = MANUAL; //to control if the movement is manual or automatic; 0 : manual, 1 : automatic

int keyState = NOT; //to control which direction the character will go ; 0 : not pressed, 1 : up arrow, 2 : down arrow, 3 : left arrow, 4 : right arrow

int charState = KUROMI; //to control which character and background will appear; 0 : kuromi(rabbit with black ears), 1 : melody(rabbit with pink ears)

int i = 0; //variable to control the movements of clouds 

double xC1 = -320; //initial coordinate of cloud 1

double xC2 = 280; // initial coordiante of cloud 2

int xP = 0, yP = 0; //variables to control where the character will appear + which location it will go
int r, g, b;



//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

//
// To display onto window using OpenGL commands
//

void drawCharacter() { //function to draw the first character (charState = 0, kuromi(rabbit with black ears))

    glColor3ub(58, 58, 58); //grey part of the head
    circle(-10 + xP, 0 + yP, 50);
    circle(10 + xP, 0 + yP, 50);
    glBegin(GL_TRIANGLES); //connecting triangles to make the head an oval shape
    glVertex2f(-10 + xP, -50 + yP);
    glVertex2f(10 + xP, -50 + yP);
    glVertex2f(10 + xP, -10 + yP);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(-10 + xP, 5 + yP);
    glVertex2f(-50 + xP, 5 + yP);
    glVertex2f(-10 + xP, 50 + yP);
    glEnd();

    glBegin(GL_POLYGON); //ears
    glVertex2f(30 + xP, 35 + yP);
    glVertex2f(35 + xP, 35 + yP);
    glVertex2f(25 + xP, 60 + yP);
    glVertex2f(55 + xP, 35 + yP);
    glVertex2f(60 + xP, 82 + yP);
    glVertex2f(25 + xP, 60 + yP);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(-30 + xP, 35 + yP);
    glVertex2f(-35 + xP, 35 + yP);
    glVertex2f(-25 + xP, 60 + yP);
    glVertex2f(-55 + xP, 35 + yP);
    glVertex2f(-60 + xP, 82 + yP);
    glVertex2f(-25 + xP, 60 + yP);
    glEnd();
    circle(60.5 + xP, 83 + yP, 4);
    circle(-60.5 + xP, 83 + yP, 4);

    glColor3f(1, 1, 1); //white part of the head (face of the character)
    circle(10 + xP, -10 + yP, 40);
    circle(-10 + xP, -10 + yP, 40);
    glBegin(GL_TRIANGLES);
    glVertex2f(-20 + xP, 30 + yP);
    glVertex2f(20 + xP, 30 + yP);
    glVertex2f(10 + xP, 20 + yP);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(-20 + xP, -45 + yP);
    glVertex2f(20 + xP, -45 + yP);
    glVertex2f(10 + xP, -40 + yP);
    glEnd();

    glColor3f(0, 0, 0); //mouth of the character 
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(-5 + xP, -37.5 + yP);
    glVertex2f(5 + xP, -37.5 + yP);
    glEnd();

    glColor3ub(245, 114, 210); //nose of the character
    circle(-1 + xP, -25 + yP, 3);
    circle(1 + xP, -25 + yP, 3);

    glColor3f(0, 0, 0); // eyes of the character
    circle(25 + xP, -4 + yP, 5);
    circle(25 + xP, -6 + yP, 5);
    circle(-25 + xP, -4 + yP, 5);
    circle(-25 + xP, -6 + yP, 5);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(24 + xP, 0 + yP);
    glVertex2f(32 + xP, 2 + yP);
    glVertex2f(-24 + xP, 0 + yP);
    glVertex2f(-32 + xP, 2 + yP);
    glVertex2f(24 + xP, -4 + yP);
    glVertex2f(32 + xP, -2 + yP);
    glVertex2f(-24 + xP, -4 + yP);
    glVertex2f(-32 + xP, -2 + yP);
    glEnd();

    glColor3ub(58, 58, 58); //triangle headband that connects the face to the grey part of its head
    glBegin(GL_TRIANGLES);
    glVertex2f(0 + xP, 10 + yP);
    glVertex2f(30 + xP, 30 + yP);
    glVertex2f(-30 + xP, 30 + yP);
    glEnd();

    glColor3ub(245, 114, 210); //skull design on top of its head
    circle(0 + xP, 28 + yP, 8);
    glRectf(5 + xP, 22 + yP, -5 + xP, 18 + yP);
    glColor3ub(58, 58, 58);
    circle(4 + xP, 26 + yP, 2);
    circle(-4 + xP, 26 + yP, 2);
    glLineWidth(0.5);
    glBegin(GL_LINES);
    glVertex2f(-2 + xP, 20.8 + yP);
    glVertex2f(-2 + xP, 18 + yP);
    glVertex2f(2 + xP, 20.8 + yP);
    glVertex2f(2 + xP, 18 + yP);
    glEnd();
}

void drawCharacter2() { //function to draw the second character (charState = 1, melody(rabbit with pink ears))

    glColor3ub(247, 59, 172); //pink part of the head
    circle(-10 + xP, 0 + yP, 49);
    circle(10 + xP, 0 + yP, 49);

    glBegin(GL_TRIANGLES); //connecting triangles to make the head shape an oval
    glVertex2f(-10 + xP, -50 + yP);
    glVertex2f(10 + xP, -50 + yP);
    glVertex2f(10 + xP, -10 + yP);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(-10 + xP, 50 + yP);
    glVertex2f(0 + xP, 15 + yP);
    glVertex2f(-10 + xP, 50 + yP);
    glEnd();

    glColor3f(1, 1, 1); //white part of the head (face)
    circle(10 + xP, -20 + yP, 30);
    circle(-10 + xP, -20 + yP, 30);
    glBegin(GL_TRIANGLES);
    glVertex2f(-20 + xP, 10 + yP);
    glVertex2f(20 + xP, 5 + yP);
    glVertex2f(10 + xP, 10 + yP);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(-20 + xP, -49 + yP);
    glVertex2f(20 + xP, -49 + yP);
    glVertex2f(10 + xP, -37 + yP);
    glEnd();

    glColor3f(0, 0, 0); //mouth of the character
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(-5 + xP, -40 + yP);
    glVertex2f(5 + xP, -40 + yP);
    glEnd();

    glColor3f(0, 0, 0); // eyes of the character
    circle(25 + xP, -15 + yP, 5);
    circle(25 + xP, -17 + yP, 5);
    circle(-25 + xP, -15 + yP, 5);
    circle(-25 + xP, -17 + yP, 5);


    glColor3ub(253, 206, 15); //nose of the character
    circle(-1 + xP, -25 + yP, 3);
    circle(1 + xP, -25 + yP, 3);

    glColor3ub(247, 59, 172); //ears of the character
    circle(-17 + xP, 70 + yP, 13);
    circle(-17 + xP, 65 + yP, 13);
    circle(-17 + xP, 60 + yP, 13);
    circle(-17 + xP, 55 + yP, 13);
    circle(-17 + xP, 50 + yP, 13);
    circle(-17 + xP, 75 + yP, 13);
    circle(17 + xP, 70 + yP, 13);
    circle(17 + xP, 65 + yP, 13);
    circle(17 + xP, 60 + yP, 13);
    circle(17 + xP, 55 + yP, 13);
    circle(17 + xP, 50 + yP, 13);
    circle(17 + xP, 75 + yP, 13);

    glColor3f(1, 1, 1); //flower design on top of its head
    circle(-40 + xP, 40 + yP, 10);
    circle(-38 + xP, 27 + yP, 10);
    circle(-28 + xP, 42 + yP, 10);
    circle(-23 + xP, 30 + yP, 10);
    glColor3ub(253, 206, 15);
    circle(-32 + xP, 35 + yP, 6);

}

void initialBackground() { //initial background that tells the user to click in order to show the character

    glBegin(GL_POLYGON); //background color
    glColor3f(0, 0, 0);
    glVertex2f(-500, -400);
    glVertex2f(500, -400);
    glColor3f(0, 0, 0);
    glVertex2f(500, 800);
    glVertex2f(-500, 800);
    glEnd();



    glColor3ub(r, g, b); //clouds

    circle(xC1, 100, 20); //cloud number 1
    circle(xC1 + 35, 115, 35);
    glBegin(GL_TRIANGLES);
    glVertex2f(xC1, 80);
    glVertex2f(xC1 + 45, 80);
    glVertex2f(xC1 + 20, 100);
    glEnd();

    circle(xC2, 200, 20); //cloud number 2
    circle(xC2 + 35, 215, 35);
    circle(xC2 + 70, 200, 20);
    glBegin(GL_TRIANGLES);
    glVertex2f(xC2, 180);
    glVertex2f(xC2 + 70, 180);
    glVertex2f(xC2 + 70 - 50, 200);
    glEnd();

    //the explaining lines that are on the screen
    glColor3f(1, 1, 1);
    vprint(-190, 0, GLUT_BITMAP_9_BY_15, "Left click to create/relocate the character");

    vprint(360, 260, GLUT_BITMAP_9_BY_15, "Sera Altuner");

    vprint(-470, 260, GLUT_BITMAP_9_BY_15, "Mode:");
    vprint(-470, 240, GLUT_BITMAP_9_BY_15, "Direction:");

    vprint(-300, -220, GLUT_BITMAP_9_BY_15, "Right click to create moving clouds and randomly change their color");

    vprint(-300, -240, GLUT_BITMAP_9_BY_15, "Press <F1> to switch the mode and Arrow Keys to control the character");

    vprint(-300, -260, GLUT_BITMAP_9_BY_15, "Press the Space Bar to change the background color and the character");
    switch (modeState) {

    case 0:
        vprint(-420, 260, GLUT_BITMAP_9_BY_15, "MANUAL");
        break;
    case 1:
        vprint(-420, 260, GLUT_BITMAP_9_BY_15, "AUTONOMUS");
        break;
    }

    switch (keyState) {
    case 0:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "NOT SET");
        break;

    case 1:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "UP");
        break;

    case 2:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "DOWN");
        break;

    case 3:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "LEFT");
        break;

    case 4:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "RIGHT");
        break;
    }
}

void backgroundDesign() { //the background that the first character has

    glBegin(GL_POLYGON); //background color
    glColor3ub(245, 114, 210);
    glVertex2f(-500, -400);
    glVertex2f(500, -400);
    glColor3f(0, 0, 0);
    glVertex2f(500, 700);
    glVertex2f(-500, 700);
    glEnd();


    glColor3ub(252, 222, 245); //clouds

    circle(xC1, 100, 20); //cloud number 1
    circle(xC1 + 35, 115, 35);
    glBegin(GL_TRIANGLES);
    glVertex2f(xC1, 80);
    glVertex2f(xC1 + 45, 80);
    glVertex2f(xC1 + 20, 100);
    glEnd();

    circle(xC2, 200, 20); //cloud number 2
    circle(xC2 + 35, 215, 35);
    circle(xC2 + 70, 200, 20);
    glBegin(GL_TRIANGLES);
    glVertex2f(xC2, 180);
    glVertex2f(xC2 + 70, 180);
    glVertex2f(xC2 + 70 - 50, 200);
    glEnd();


    glColor3ub(0, 0, 0);

    //the explaining lines that are on the screen

    vprint(360, 260, GLUT_BITMAP_9_BY_15, "Sera Altuner");

    vprint(-470, 260, GLUT_BITMAP_9_BY_15, "Mode:");
    vprint(-470, 240, GLUT_BITMAP_9_BY_15, "Direction:");

    vprint(-300, -260, GLUT_BITMAP_9_BY_15, "Press <F1> to switch the mode and Arrow Keys to control the character");

    vprint(-300, -280, GLUT_BITMAP_9_BY_15, "Press the Space Bar to change the background color and the character");
    switch (modeState) {

    case 0:
        vprint(-420, 260, GLUT_BITMAP_9_BY_15, "MANUAL");
        break;
    case 1:
        vprint(-420, 260, GLUT_BITMAP_9_BY_15, "AUTONOMUS");
        break;
    }

    switch (keyState) {
    case 0:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "NOT SET");
        break;

    case 1:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "UP");
        break;

    case 2:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "DOWN");
        break;

    case 3:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "LEFT");
        break;

    case 4:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "RIGHT");
        break;
    }
}

void backgroundDesign2() {//background number two, the background that the second character has

    glBegin(GL_POLYGON); //background color
    glColor3f(1, 1, 1);
    glVertex2f(-500, -650);
    glVertex2f(500, -650);
    glColor3ub(255, 17, 200);
    glVertex2f(500, 500);
    glVertex2f(-500, 500);
    glEnd();

    glColor3ub(252, 222, 245); //clouds

    circle(xC1, 100, 20); //cloud number 1
    circle(xC1 + 35, 115, 35);
    glBegin(GL_TRIANGLES);
    glVertex2f(xC1, 80);
    glVertex2f(xC1 + 45, 80);
    glVertex2f(xC1 + 20, 100);
    glEnd();

    circle(xC2, 200, 20); //cloud number 2
    circle(xC2 + 35, 215, 35);
    circle(xC2 + 70, 200, 20);
    glBegin(GL_TRIANGLES);
    glVertex2f(xC2, 180);
    glVertex2f(xC2 + 70, 180);
    glVertex2f(xC2 + 70 - 50, 200);
    glEnd();
    glColor3ub(0, 0, 0);

    //the explaining lines that are on the screen
    vprint(360, 260, GLUT_BITMAP_9_BY_15, "Sera Altuner");

    vprint(-470, 260, GLUT_BITMAP_9_BY_15, "Mode:");
    vprint(-470, 240, GLUT_BITMAP_9_BY_15, "Direction:");

    vprint(-300, -260, GLUT_BITMAP_9_BY_15, "Press <F1> to switch the mode and Arrow Keys to control the character");

    vprint(-300, -280, GLUT_BITMAP_9_BY_15, "Press the Space Bar to change the background color and the character");

    switch (modeState) {

    case 0:
        vprint(-420, 260, GLUT_BITMAP_9_BY_15, "MANUAL");
        break;
    case 1:
        vprint(-420, 260, GLUT_BITMAP_9_BY_15, "AUTONOMUS");
        break;
    }

    switch (keyState) {
    case 0:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "NOT SET");
        break;

    case 1:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "UP");
        break;

    case 2:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "DOWN");
        break;

    case 3:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "LEFT");
        break;

    case 4:
        vprint(-380, 240, GLUT_BITMAP_9_BY_15, "RIGHT");
        break;
    }
}

void display() {

    initialBackground();


    if (backgroundState == CLICKED_SCREEN) { //checking if the initial screen will change or not

        switch (charState) { //checking which character and background will be shown
            backgroundDesign();

        case KUROMI:
            switch (appState) { //checking if the character will be shown
            case START:
                backgroundDesign();
                break;
            case SHOW:
                backgroundDesign();
                drawCharacter();
                break;
            }
            break;
        case MELODY:
            switch (appState) { //checking if the character will be shown
            case START:
                backgroundDesign2();
                break;
            case SHOW:
                backgroundDesign2();
                drawCharacter2();
                break;
            }
            break;
        }
    }

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == 32) {
        if (charState == KUROMI)
            charState = MELODY;
        else
            charState = KUROMI;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }
    if (key == GLUT_KEY_F1) { //checking which mode the program will work on
        if (modeState == MANUAL)
            modeState = AUTO;
        else
            modeState = MANUAL;
    }

    if (modeState == MANUAL) { //moving the charcter in manual mode according to which arrow key the user presses
        if (key == GLUT_KEY_UP) {
            if (yP < (winHeight / 2 - 90))
                yP++;
        }
        if (key == GLUT_KEY_DOWN) {
            if (yP > -(winHeight / 2 - 50))
                yP--;
        }
        if (key == GLUT_KEY_LEFT) {
            if (xP > -(winWidth / 2 - 62))
                xP--;
        }
        if (key == GLUT_KEY_RIGHT) {
            if (xP < (winWidth / 2 - 62))
                xP++;
        }
    }


    if (key == GLUT_KEY_UP) //changing the keyState variable according to the arrow key that user presses
        keyState = UP;
    if (key == GLUT_KEY_DOWN)
        keyState = DOWN;
    if (key == GLUT_KEY_LEFT)
        keyState = LEFT;
    if (key == GLUT_KEY_RIGHT)
        keyState = RIGHT;



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{

    if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN) {
        srand(time(NULL));
        r = (rand() % (256 - 1 + 1) + 1);
        g = (rand() % (256 - 1 + 1) + 1);
        b = (rand() % (256 - 1 + 1) + 1);
    }


    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) //changing the background state to change it from initialBackground to backgrounDesign
        backgroundState = CLICKED_SCREEN;


    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) { //to show to character at the location that user clicks with left mouse click
        if (appState == START) {
            appState = SHOW;
            xP = x - winWidth / 2;
            yP = winHeight / 2 - y;
        }
        else {
            xP = x - winWidth / 2;
            yP = winHeight / 2 - y;
        }


    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);

    if (xC1 + 70 < winWidth / 2) { //to move the clouds in and outside of the window
        xC1 -= 0.5;

    }

    if (xC2 + 105 < winWidth / 2) {
        xC2--;

    }

    if (xC1 + 35 == -535) { //to show the cloud from the right side of the window after it exits from the left side of the window 

        xC1 = 520;

        while (i < 100) {
            xC1--;

            i++;
        }
        i = 0; //making i zero to turn it into a loop
    }

    if (xC2 + 70 == -520) {//to show the cloud from the right side of the window after it exits from the left side of the window 
        xC2 = 520;

        while (i < 150) {
            xC2--;

            i++;
        }
        i = 0; //making i zero to turn it into a loop
    }

    if (modeState == AUTO) { //to move the character automatically according to the arrow keys the user pressses (by using the keyState variable)
        if (keyState == UP) {
            if (yP < (winHeight / 2 - 90))
                yP++;
            if (yP == (winHeight / 2 - 90))
                keyState = DOWN;

        }
        if (keyState == DOWN) {
            if (yP > -(winHeight / 2 - 50))
                yP--;
            if (yP == -(winHeight / 2 - 50))
                keyState = UP;
        }
        if (keyState == LEFT) {
            if (xP > -(winWidth / 2 - 62))
                xP--;
            if (xP == -(winWidth / 2 - 62))
                keyState = RIGHT;
        }
        if (keyState == RIGHT) {
            if (xP < (winWidth / 2 - 62))
                xP++;
            if (xP == (winWidth / 2 - 62))
                keyState = LEFT;
        }


    }


    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(500, 180);
    glutCreateWindow("Homework 1");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
