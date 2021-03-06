// SpaceInvadersGL.cpp: definisce il punto di ingresso dell'applicazione console.


#include "stdafx.h"
#include <GL/glew.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include "math.h"
#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/glu.h>	

#include <GL/freeglut.h>

using namespace std;

#define SPACEBAR 32
#define ENTER 13

//To manage gamestart
boolean start = false;

int anim = 0;
int offInvaders = 2;

int larghezza, altezza, nvertice = 30, win_id, is = 0;
float rsfondo = 0, gsfondo = 0, bsfondo = 0;
float rprato = 127.0 / 255, gprato = 1, bprato = 0;
float angolo = 0, angolops = 0, angolopd = 0;
float traslax = 0;
float tempo = 1;
typedef struct { float x, y; } Point;
Point pointsArray[30], pointsArray1[30];
#define PI 3.14159265
Point *cerchio();

typedef struct { float x, y, z; } PointVBO;


typedef struct { double r, g, b, a; } ColorRGBA; // in formato (255,255,255,255)
#define nvertices 24

//Prova VBO
int VerticiUfo = 200;
float angoloUfo = 0;

//Variabili VBO
GLuint vbo_ufo, vbo_colore_ufo;


PointVBO* ufo;
ColorRGBA *coloreC;

//Colori corpo ufo
ColorRGBA color_metaldark = { 110.0 / 255.0 ,110.0 / 255.0 ,110.0 / 255.0,255.0 / 255.0 };
ColorRGBA color_metallight = { 184.0 / 255.0,184.0 / 255.0,184.0 / 255.0,255.0 / 255.0 };

ColorRGBA color_cabindark = { 66.0 / 255.0,134.0 / 255.0,244.0 / 255.0,255.0 / 255.0 };
ColorRGBA color_cabinlight = { 158.0 / 255.0,195.0 / 255.0,255.0 / 255.0,255.0 / 255.0 };



int w[200], h[200];

int lives = 3;

int points = 0;

int playerPos = 0;

int invadersMovex = 0;
int invadersMovey = 0;

boolean fire = false;
int shotPositionx;
int shotPositiony;

boolean invader1hit = false;
boolean invader2hit = false;
boolean invader3hit = false;
boolean invader4hit = false;
boolean invader5hit = false;
boolean invader6hit = false;
boolean invader7hit = false;
boolean invader8hit = false;
boolean invader9hit = false;


float textLamp = 1;
void Idle();

void display();
void myinit();




int random(int m)
{
	return rand() % m;
}

void stroke_output(GLfloat x, GLfloat y, char *format)
{
	va_list args;
	char buffer[200], *p;
	va_start(args, format);
	vsprintf_s(buffer, format, args);
	va_end(args);
	glPushMatrix();
	glTranslatef(x, y, 0);
	for (p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	glPopMatrix();
}

void bitmap_output(int x, int y, int z, char *string,
	void *font)
{
	int len, i;
	glRasterPos3f(x, y, 0);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, string[i]);
}


int timeInv = 0;
int risk = 0;
int risked = 5;

bool line3 = false;
bool line2 = false;

bool win = false;
bool gameOver = false;

//Animation function
void Idle()
{
	//Con questa funzione gestiamo l'incremento dei parametri
	// che caratterizzano l'animazione

	//Menù
	if (!start && !gameOver) {
		textLamp = 1;
		is = is + 1;
		if (is > 60)
			is = 0;
		if (is > 30)
			textLamp = 0;
		if (random(1000) < 500) {
			if (playerPos < larghezza / 2 * 0.8 + 5)
				playerPos += 5;
		} else
		if (playerPos>-larghezza / 2 * 0.8 - 5)
			playerPos -= 5;
	}

	//Game started
	else if(start && !gameOver){
		anim += 1;
		if (anim % 10 == 0)
			offInvaders += 2;

		else if (anim % 5 == 0)
			offInvaders -= 2;

		if(!win)
			timeInv++;

		if (timeInv < larghezza*0.1) {
			invadersMovex += 3;
		}
		if (timeInv == larghezza / 10 ) {
			invadersMovey -= altezza * 0.1;
			risk++;
		}

		if (timeInv > larghezza*0.1) {
			invadersMovex -= 3;
		}
		if (timeInv > larghezza*0.2) {
			invadersMovey -= altezza * 0.1;
			risk++;
			timeInv = 0;
		}

		//Third enemy line
		if (invader9hit && invader8hit && !line2) {
			line2 = true;
			risked += 1;
		}
		//Second enemy line
		if (invader5hit && invader6hit && invader7hit && !line3 && line2) {
			line3 = true;
			risked += 1;
		}

		//Manages game win
		if (invader1hit && invader2hit && invader3hit && invader4hit && invader5hit && invader6hit && invader7hit && invader8hit && invader9hit)
			win = true;

		if (risk >= risked) {
			gameOver = true;
		}

		if (fire) {
			shotPositiony+=15;
		}

		if (shotPositiony > altezza) {
			shotPositiony = altezza * 0.1;
			fire = false;
		}
		
	}

	glutSetWindow(win_id);  //Setta la finestra su cui rifare il disegno
	glutPostRedisplay();    //Forza il ridisegno ad ogni refresh
}


void playerShoot() {
	glBegin(GL_POLYGON);
	glVertex2i(0, 2);
	glVertex2i(1, 1);
	glVertex2i(1, -1);
	glVertex2i(-1, -1);
	glVertex2i(-1, 1);
	glEnd();
}

float shoot[] = { 0,2,0, 1,0,0,
			      1,1,0, 1,0,0,
		   	      1,-1,0, 1,0,0,
			     -1,-1,0, 1,0,0,
		   	     -1,1,0, 1,0,0 };

float binvader1[] = { 5,0,0, 1,0,0,
					5,-3,0, 1,0,0,
					4,-3,0, 1,0,0,
					4,-1,0, 1,0,0,
					3,-1,0, 1,0,0,
					3,-3,0, 1,0,0,
					2,-4,0, 1,0,0,
					0,-4,0, 1,0,0,
					0,-3,0, 1,0,0,
					2,-2,0, 1,0,0,
					2,-1,0, 1,0,0,
					-3,-1,0, 1,0,0,
					-3,-2,0, 1,0,0,
					-1,-3,0, 1,0,0,
					-1,-4,0, 1,0,0,
					-3,-4,0, 1,1,0,
					-4,-3,0, 1,0,0,
					-4,-1,0, 1,0,0,
					-5,-1,0, 1,0,0,
					-5,-3,0, 1,0,0,
					-6,-3,0, 1,0,0,
					-6,0,0, 1,0,0,
					-4,2,0, 1,0,0,
					-3,2,0, 1,0,0,
					-4,3,0, 1,0,0,
					-4,4,0, 1,0,0,
					-3,4,0, 1,0,0,
					-2,2,0, 1,0,0,
					1,2,0, 1,0,0,
					2,4,0, 1,0,0,
					3,4,0, 1,0,0,
					3,3,0, 1,0,0,
					2,2,0, 1,0,0,
					3,2,0, 1,0,0};
unsigned int vbo_binvader1;

void invader1() {
	glBegin(GL_LINE_LOOP);

	glVertex2i(5, 0);
	glVertex2i(5, -3);
	glVertex2i(4, -3);
	glVertex2i(4, -1);
	glVertex2i(3, -1);
	glVertex2i(3, -3);
	glVertex2i(2, -4);
	glVertex2i(0, -4);
	glVertex2i(0, -3);
	glVertex2i(2, -2);
	glVertex2i(2, -1);
	glVertex2i(-3, -1);
	glVertex2i(-3, -2);
	glVertex2i(-1, -3);
	glVertex2i(-1, -4);
	glVertex2i(-3, -4);
	glVertex2i(-4, -3);
	glVertex2i(-4, -1);
	glVertex2i(-5, -1);
	glVertex2i(-5, -3);
	glVertex2i(-6, -3);
	glVertex2i(-6, 0);
	glVertex2i(-4, 2);
	glVertex2i(-3, 2);
	glVertex2i(-4, 3);
	glVertex2i(-4, 4);
	glVertex2i(-3, 4);
	glVertex2i(-2, 2);
	glVertex2i(1, 2);
	glVertex2i(2, 4);
	glVertex2i(3, 4);
	glVertex2i(3, 3);
	glVertex2i(2, 2);
	glVertex2i(3, 2);
	glEnd();

	//Disegno gli Occhi
	
	glBegin(GL_POLYGON);
	glVertex2i(1, 1);
	glVertex2i(2, 1);
	glVertex2i(2, 0);
	glVertex2i(1, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2i(-2, 0);
	glVertex2i(-3, 0);
	glVertex2i(-3, 1);
	glVertex2i(-2, 1);
	glEnd();
}

void invader2() {
	glBegin(GL_LINE_LOOP);
	glVertex2i(4, -1);
	glVertex2i(2, -1);
	glVertex2i(4, -3);
	glVertex2i(4, -4);
	glVertex2i(3, -4);
	glVertex2i(2, -3);
	glVertex2i(2, -4);
	glVertex2i(1, -4);
	glVertex2i(1, -3);
	glVertex2i(-1, -3);
	glVertex2i(-1, -4);
	glVertex2i(-2, -4);
	glVertex2i(-2, -3);
	glVertex2i(-3, -4);
	glVertex2i(-4, -4);
	glVertex2i(-4, -3);
	glVertex2i(-2, -1);
	glVertex2i(-4, -1);
	glVertex2i(-4, 1);
	glVertex2i(-1, 4);
	glVertex2i(1, 4);
	glVertex2i(4, 1);
	glEnd();

	//Disegno gli occhi
	glBegin(GL_POLYGON);
	glVertex2i(1, 0);
	glVertex2i(1, 1);
	glVertex2i(2, 1);
	glVertex2i(2, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2i(-1, 0);
	glVertex2i(-1, 1);
	glVertex2i(-2, 1);
	glVertex2i(-2, 0);
	glEnd();
}

void invader3() {
	glBegin(GL_LINE_LOOP);
	glVertex2i(8, -1);
	glVertex2i(6, -1);
	glVertex2i(6, -2);
	glVertex2i(5, -3);
	glVertex2i(3, -1);
	glVertex2i(1, -1);
	glVertex2i(0, -2);
	glVertex2i(-1, -1);
	glVertex2i(-3, -1);
	glVertex2i(-5, -3);
	glVertex2i(-6, -2);
	glVertex2i(-6, -1);
	glVertex2i(-8, -1);
	glVertex2i(-5, 3);
	glVertex2i(-3, 4);
	glVertex2i(3, 4);
	glVertex2i(5, 3);
	glEnd();

	//Disegno gli oblò
	glBegin(GL_POLYGON);
	glVertex2i(1, 0);
	glVertex2i(1, 1);
	glVertex2i(2, 1);
	glVertex2i(2, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2i(4, 0);
	glVertex2i(5, 0);
	glVertex2i(5, 1);
	glVertex2i(4, 1);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2i(-1, 0);
	glVertex2i(-2, 0);
	glVertex2i(-2, 1);
	glVertex2i(-1, 1);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2i(-4, 0);
	glVertex2i(-5, 0);
	glVertex2i(-5, 1);
	glVertex2i(-4, 1);
	glEnd();
}

void player() {
	glBegin(GL_LINE_LOOP);
	glVertex2i(3, 1);
	glVertex2i(3, -1);
	glVertex2i(-3, -1);
	glVertex2i(-3, 1);
	glVertex2i(-1, 2);
	glVertex2i(0, 3);
	glVertex2i(1, 2);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2i(1, 1);
	glVertex2i(1, 0);
	glVertex2i(-1, 0);
	glVertex2i(-1, 1);
	glPopMatrix();
	glEnd();

}

void stelle() {

	glBegin(GL_POINTS);
	for (int i = 0; i < 200; i++) {
		glVertex2i(w[i], h[i]);
	}
	glEnd();
}

Point *cerchio()
{

	//Costruisce un cerchio facendo uso delle formule parametriche
	int i;

	glBegin(GL_POLYGON);
	for (i = 0; i<nvertices; i++)
	{
		pointsArray[i].x = cos((2 * PI)*i / nvertices);
		pointsArray[i].y = sin((2 * PI)*i / nvertices);

		glVertex2f(pointsArray[i].x, pointsArray[i].y);
	}
	glEnd();

	return pointsArray;
}


//Resets the game variables
void restart() {
	start = false;
	gameOver = false;
	win = false;
	timeInv = 0;
	risk = 0;
	lives = 3;

	points = 0;

	playerPos = 0;

	invadersMovex = 0;
	invadersMovey = 0;
	shotPositiony = altezza * 0.1;
	fire = false;

	invader1hit = false;
	invader2hit = false;
	invader3hit = false;
	invader4hit = false;
	invader5hit = false;
	invader6hit = false;
	invader7hit = false;
	invader8hit = false;
	invader9hit = false;

	int risked = 5;

	bool line3 = false;
	bool line2 = false;
}

void myKeyboard(unsigned char key, int w, int h) {

	switch (key) {
	case ENTER:
		if(start==false)
			start = true;
		else if(gameOver || win){
			restart();
		}
		break;

	case SPACEBAR:
		if (!fire)
			fire = true;
		break;

	case 'a':
		if (playerPos>-larghezza / 2 * 0.8 + 5)
			playerPos-=5;
		glutPostRedisplay();
		break;

	case 'd':
		if (playerPos<larghezza / 2 * 0.8 + 5)
			playerPos += 5;
		glutPostRedisplay();
		break;
	}
}

void mySpecial(int key, int w, int h) {
	switch (key) {
	case GLUT_KEY_LEFT:
		if(playerPos>-larghezza/2*0.8+5)
			playerPos -= 5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		if(playerPos<larghezza/2*0.8+5)
			playerPos += 5;
		glutPostRedisplay();
		break;
	}
}

float navetta[] = {3,1,0, 1,0,0,
			3,-1,0, 0,1,0,
			-3,-1,0, 1,0,0,
			-3,1,0, 1,1,1,
			-1,2,0, 0,1,1,
			 0,3,0, 1,1,0,
			 1,2,0, 1,0,0
};
unsigned int vbo_navetta, vbo_shoot;

void display()

{
	int i, j;
	
	glClearColor(rsfondo, gsfondo, bsfondo, 0.0); //colore
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	if (!start && !gameOver) {

		//Scrivo il nome in verde usando il font stroke
		glColor3f(0.0, 1.0, 0.0);
		glPushMatrix();
		glLineWidth(3.0);

		char scritta1[] = "SPACE";

		//Porto la scritta  nel punto 0,0,0
		glTranslatef(larghezza*0.3, altezza*0.8, 0);

		//Eseguo le trasformazioni
		glScalef(1, 2, 0);
		glRotatef(-45, 1, 0, 0);
		//Riporto la scritta nel punto desiderato
		glTranslatef(-larghezza * 0.3, -altezza * 0.8, 0);
		stroke_output(larghezza*0.3, altezza*0.8, scritta1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(larghezza*0.3, altezza*0.55, 0);
		glScalef(1, 2, 0);
		glRotatef(-45, 1, 0, 0);
		glTranslatef(-larghezza * 0.3, -altezza * 0.55, 0);
		char scritta2[] = "INVADERS";
		stroke_output(larghezza*0.3, altezza* 0.55, scritta2);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, textLamp, 0.0);
		glLineWidth(1.5);
		glPointSize(1.0);
		glTranslatef(larghezza*0.3, altezza*0.4, 0);
		glScalef(0.3, 0.2, 0);
		glTranslatef(-larghezza * 0.3, -altezza * 0.4, 0);
		char scritta3[] = "PRESS ENTER TO START";
		stroke_output(larghezza*0.3, altezza*0.4, scritta3);
		glPopMatrix();
		glLineWidth(2.0);

		

		//Disegno gli invaders
		glColor3f(0.0, 0.0, 1.0);
		glPushMatrix();
		glTranslatef(larghezza*0.2, altezza*0.5, 0);
		glScalef(5, 5, 0);
		//glRotatef(20, 0, 0, 1);
		invader1();
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 1.0, 0.0);
		glTranslatef(larghezza*0.3, altezza*0.5, 0);
		glScalef(5, 5, 0);
		invader1();
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(larghezza*0.4, altezza*0.5, 0);
		glScalef(5, 5, 0);
		invader1();
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(larghezza*0.7, altezza*0.5, 0);
		glScalef(5, 5, 0);
		invader2();
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 1.0);
		glTranslatef(larghezza*0.6, altezza*0.5, 0);
		glScalef(5, 5, 0);
		invader3();
		glPopMatrix();

		glPushMatrix();
		glColor3f(7.0 / 255, 186.0 / 255, 46.0 / 255);
		glTranslatef(larghezza*0.8, altezza*0.5, 0);
		glScalef(5, 5, 0);
		//invader1();
		glBindBuffer(GL_ARRAY_BUFFER, vbo_binvader1);
		glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), 0);
		glColorPointer(3, GL_FLOAT, 3 * sizeof(float), NULL);
		glDrawArrays(GL_LINE_LOOP, 0, 34);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glPopMatrix();

		//Disegno il giocatore
		glPushMatrix();
		glColor3f(0.0, 1.0, 0.0);
		glTranslatef(larghezza*0.5 + playerPos, altezza*0.1, 0);
		glScalef(5, 5, 0);
		//player();
		glBindBuffer(GL_ARRAY_BUFFER, vbo_navetta);
		glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), 0);
		glColorPointer(3, GL_FLOAT, 6 * sizeof(float), NULL);
		glDrawArrays(GL_LINE_LOOP, 0, 7);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glPopMatrix();


		//Disegno le stelle
		glColor3f(1.0, 1.0, 1.0);
		glPointSize(1.0);
		stelle();

		glPointSize(4.0);
	}
	else if(start && !gameOver && !win){ 

		//Disegno le stelle
		glColor3f(1.0, 1.0, 1.0);
		glPointSize(1.0);
		stelle();

		glPointSize(4.0);
		/////////////////////
		// Game is started //
		/////////////////////
		glColor3f(0.0, 1.0, 0.0);
		glPushMatrix();
		glLineWidth(1.5);
		glPointSize(1.0);
		glTranslatef(larghezza*0.05, altezza*0.95, 0);
		glScalef(0.3, 0.2, 0);
		glTranslatef(-larghezza * 0.05, -altezza * 0.95, 0);
		
		char scritta3[12] = "POINTS: ";
		stroke_output(larghezza*0.05, altezza*0.95, scritta3);
		glPopMatrix();

		
		glPushMatrix();
		glTranslatef(larghezza*0.2, altezza*0.95, 0);
		glScalef(0.3, 0.2, 0);
		glTranslatef(-larghezza * 0.2, -altezza * 0.95, 0);
		
		char punti[5];
		if (points > 9999)
			points = 9999;
		sprintf_s(punti, "%ld", points);

		stroke_output(larghezza*0.2, altezza*0.95, punti);
		glPopMatrix();
		glLineWidth(2.0);


		//Player position

		glColor3f(0.0, 1.0, 0.0);
		glPushMatrix();
		glTranslatef(larghezza*0.5+playerPos, altezza*0.1, 0);
		glScalef(5, 5, 0);
		//player();
		glBindBuffer(GL_ARRAY_BUFFER, vbo_navetta);
		glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), 0);
		glColorPointer(3, GL_FLOAT, 6 * sizeof(float), NULL);
		glDrawArrays(GL_LINE_LOOP, 0, 7);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glPopMatrix();

		//Shot position
		if (!fire) {
			shotPositionx = playerPos;
			glColor3f(1.0, 0.0, 0.0);
			glPushMatrix();
			glTranslatef(larghezza*0.5 + shotPositionx, altezza*0.1, 0);
			glScalef(3, 3, 0);
			//playerShoot();
			glBindBuffer(GL_ARRAY_BUFFER, vbo_shoot);
			glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), 0);
			glColorPointer(3, GL_FLOAT, 0, NULL);
			glDrawArrays(GL_LINE_LOOP, 0, 5);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glPopMatrix();
		}
		else {
			glColor3f(1.0, 0.0, 0.0);
			glPushMatrix();
			glTranslatef(larghezza*0.5 + shotPositionx, altezza*0.1 + shotPositiony, 0);
			glScalef(3, 3, 0);
			//playerShoot();
			glBindBuffer(GL_ARRAY_BUFFER, vbo_shoot);
			glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), 0);
			glColorPointer(3, GL_FLOAT, 3 * sizeof(float), NULL);
			glDrawArrays(GL_LINE_LOOP, 0, 5);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glPopMatrix();
		}

		/////////////////////
		//Invaders position//
		/////////////////////

		//Invader1 hit
		if (larghezza*0.5 + shotPositionx > larghezza*0.2 + invadersMovex - 10 &&
			larghezza*0.5 + shotPositionx < larghezza*0.2 + invadersMovex + 10 &&
			altezza*0.1 + shotPositiony < altezza*0.8 + invadersMovey + 10 &&
			altezza*0.1 + shotPositiony > altezza*0.8 + invadersMovey - 10 && invader1hit == false && fire == true) {
			
				invader1hit = true;
				points += 10;
				shotPositiony = altezza * 0.1;
				fire = false;
		}
		
		if (!invader1hit) {
			glColor3f(176.0, 183.0, 193.0);
			glPushMatrix();
			glTranslatef(larghezza*0.2 + invadersMovex, altezza*0.8 + offInvaders + invadersMovey, 0);
			glScalef(3, 3, 0);
			invader2();
			glPopMatrix();
		}

		//Invader2hit
		if (larghezza*0.5 + shotPositionx > larghezza*0.35 + invadersMovex - 10 &&
			larghezza*0.5 + shotPositionx < larghezza*0.35 + invadersMovex + 10 &&
			altezza*0.1 + shotPositiony < altezza*0.8 + invadersMovey + 10 &&
			altezza*0.1 + shotPositiony > altezza*0.8 + invadersMovey - 10 && invader2hit==false && fire == true) {

			invader2hit = true;
			points += 10;
			shotPositiony = altezza * 0.1;
			fire = false;
		}
		if (!invader2hit) {
			glColor3f(0.0, 1.0, 0.0);
			glPushMatrix();
			glTranslatef(larghezza*0.35 + invadersMovex, altezza*0.8 + offInvaders + invadersMovey, 0);
			glScalef(3, 3, 0);
			invader1();
			glPopMatrix();
		}
		
		//Invader3hit
		if (larghezza*0.5 + shotPositionx > larghezza*0.45 + invadersMovex - 15 &&
			larghezza*0.5 + shotPositionx < larghezza*0.45 + invadersMovex + 15 &&
			altezza*0.1 + shotPositiony < altezza*0.8 + invadersMovey + 10 &&
			altezza*0.1 + shotPositiony > altezza*0.8 + invadersMovey - 10 && invader3hit == false && fire == true) {

			invader3hit = true;
			points += 10;
			shotPositiony = altezza * 0.1;
			fire = false;
		}
		if (!invader3hit) {
			glColor3f(0.0, 1.0, 0.0);
			glPushMatrix();
			glTranslatef(larghezza*0.45 + invadersMovex, altezza*0.8 + offInvaders + invadersMovey, 0);
			glScalef(3, 3, 0);
			invader1();
			glPopMatrix();
		}

		//Invader4hit
		if (larghezza*0.5 + shotPositionx > larghezza*0.6 + invadersMovex - 15 &&
			larghezza*0.5 + shotPositionx < larghezza*0.6 + invadersMovex + 15 &&
			altezza*0.1 + shotPositiony < altezza*0.8 + invadersMovey + 10 &&
			altezza*0.1 + shotPositiony > altezza*0.8 + invadersMovey - 10 && invader4hit == false && fire == true) {

			invader4hit = true;
			points += 10;
			shotPositiony = altezza * 0.1;
			fire = false;
		}
		if (!invader4hit) {
			glColor3f(206.0/255, 53.0/255, 191.0/255);
			glPushMatrix();
			glTranslatef(larghezza*0.6 + invadersMovex, altezza*0.8 + offInvaders + invadersMovey, 0);
			glScalef(3, 3, 0);
			invader2();
			glPopMatrix();
		}

		//Invader5hit
		if (larghezza*0.5 + shotPositionx > larghezza*0.3 + invadersMovex - 15 &&
			larghezza*0.5 + shotPositionx < larghezza*0.3 + invadersMovex + 15 &&
			altezza*0.1 + shotPositiony < altezza*0.7 + invadersMovey + 10 &&
			altezza*0.1 + shotPositiony > altezza*0.7 + invadersMovey - 10 && invader5hit == false && fire == true) {

			invader5hit = true;
			points += 10;
			shotPositiony = altezza * 0.1;
			fire = false;
		}
		if (!invader5hit) {
			glColor3f(225.0 / 255, 229.0 / 255, 13.0 / 255);
			glPushMatrix();
			glTranslatef(larghezza*0.3 + invadersMovex, altezza*0.7 + offInvaders + invadersMovey, 0);
			glScalef(3, 3, 0);
			invader2();
			glPopMatrix();
		}

		//Invader6hit
		if (larghezza*0.5 + shotPositionx > larghezza*0.4 + invadersMovex - 15 &&
			larghezza*0.5 + shotPositionx < larghezza*0.4 + invadersMovex + 15 &&
			altezza*0.1 + shotPositiony < altezza*0.7 + invadersMovey + 10 &&
			altezza*0.1 + shotPositiony > altezza*0.7 + invadersMovey - 10 && invader6hit == false && fire == true) {

			invader6hit = true;
			points += 10;
			shotPositiony = altezza * 0.1;
			fire = false;
		}
		if (!invader6hit) {
			glColor3f(27.0 / 255, 28.0 / 255, 30.0 / 255);
			glPushMatrix();
			glTranslatef(larghezza*0.4 + invadersMovex, altezza*0.7 + offInvaders + invadersMovey, 0);
			glScalef(3, 3, 0);
			invader3();
			glPopMatrix();
		}

		
		//Invader7hit
		if (larghezza*0.5 + shotPositionx > larghezza*0.5 + invadersMovex - 15 &&
			larghezza*0.5 + shotPositionx < larghezza*0.5 + invadersMovex + 15 &&
			altezza*0.1 + shotPositiony < altezza*0.7 + invadersMovey + 10 &&
			altezza*0.1 + shotPositiony > altezza*0.7 + invadersMovey - 10 && invader7hit == false && fire == true) {

			invader7hit = true;
			points += 10;
			shotPositiony = altezza * 0.1;
			fire = false;
		}
		if (!invader7hit) {
			glColor3f(193.0 / 255, 45.0 / 255, 11.0 / 255);
			glPushMatrix();
			glTranslatef(larghezza*0.5 + invadersMovex, altezza*0.7 + offInvaders + invadersMovey, 0);
			glScalef(3, 3, 0);
			invader2();
			glPopMatrix();
		}


		//Invader8hit
		if (larghezza*0.5 + shotPositionx > larghezza*0.35 + invadersMovex - 15 &&
			larghezza*0.5 + shotPositionx < larghezza*0.35 + invadersMovex + 15 &&
			altezza*0.1 + shotPositiony < altezza*0.6 + invadersMovey + 10 &&
			altezza*0.1 + shotPositiony > altezza*0.6 + invadersMovey - 10 && invader8hit == false && fire==true) {

			invader8hit = true;
			points += 10;
			shotPositiony = altezza * 0.1;
			fire = false;
		}
		if (!invader8hit) {
			glColor3f(78.0 / 255, 186.0 / 255, 196.0 / 255);
			glPushMatrix();
			glTranslatef(larghezza*0.35 + invadersMovex, altezza*0.6 + offInvaders + invadersMovey, 0);
			glScalef(3, 3, 0);
			invader1();
			glPopMatrix();
		}

		//Invader9hit
		if (larghezza*0.5 + shotPositionx > larghezza*0.45 + invadersMovex - 15 &&
			larghezza*0.5 + shotPositionx < larghezza*0.45 + invadersMovex + 15 &&
			altezza*0.1 + shotPositiony < altezza*0.6 + invadersMovey + 10 &&
			altezza*0.1 + shotPositiony > altezza*0.6 + invadersMovey - 10 && invader9hit == false && fire == true) {

			invader9hit = true;
			points += 10;
			shotPositiony = altezza * 0.1;
			fire = false;
		}
		if (!invader9hit) {
			glColor3f(78.0 / 255, 186.0 / 255, 196.0 / 255);
			glPushMatrix();
			glTranslatef(larghezza*0.45 + invadersMovex, altezza*0.6 + offInvaders + invadersMovey, 0);
			glScalef(3, 3, 0);
			invader1();
			glPopMatrix();
		}

	}
	else if (start && gameOver) {

		//Disegno le stelle
		glColor3f(1.0, 1.0, 1.0);
		glPointSize(1.0);
		stelle();

		glPointSize(1.0);

		glColor3f(0.0, 1.0, 0.0);

		glPushMatrix();
		glTranslatef(larghezza*0.4, altezza*0.8, 0);
		glScalef(0.2, 0.2, 0);
		glTranslatef(-larghezza * 0.4, -altezza * 0.8, 0);

		char yourScore[] = "Your Score: ";
		stroke_output(larghezza*0.4, altezza*0.8, yourScore);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(larghezza*0.6, altezza*0.8, 0);
		glScalef(0.2, 0.2, 0);
		glTranslatef(-larghezza * 0.6, -altezza * 0.8, 0);

		char punti[5];
		if (points > 9999)
			points = 9999;
		sprintf_s(punti, "%ld", points);

		stroke_output(larghezza*0.6, altezza*0.8, punti);
		glPopMatrix();




		glPushMatrix();
		glTranslatef(larghezza*0.4, altezza*0.5, 0);
		glScalef(0.3, 0.3, 0);
		glTranslatef(-larghezza * 0.4, -altezza * 0.5, 0);

		char gameOverText[] = "GAME OVER";
		stroke_output(larghezza*0.4, altezza*0.5, gameOverText);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(larghezza*0.3, altezza*0.3, 0);
		glScalef(0.3, 0.3, 0);
		glTranslatef(-larghezza * 0.3, -altezza * 0.3, 0);
		
		char returnToMenu[] = "PRESS ENTER TO RESTART";
		stroke_output(larghezza*0.3, altezza*0.3, returnToMenu);
		glPopMatrix();

		
	}
	else if (win) {

		//Disegno le stelle
		glColor3f(1.0, 1.0, 1.0);
		glPointSize(1.0);
		stelle();


		glColor3f(0.0, 1.0, 0.0);

		glPushMatrix();
		glTranslatef(larghezza*0.4, altezza*0.8, 0);
		glScalef(0.2, 0.2, 0);
		glTranslatef(-larghezza * 0.4, -altezza * 0.8, 0);

		char yourScore[] = "Your Score: ";
		stroke_output(larghezza*0.4, altezza*0.8, yourScore);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(larghezza*0.6, altezza*0.8, 0);
		glScalef(0.2, 0.2, 0);
		glTranslatef(-larghezza * 0.6, -altezza * 0.8, 0);
		
		char punti[5];
		if (points > 9999)
			points = 9999;
		sprintf_s(punti, "%ld", points);

		stroke_output(larghezza*0.6, altezza*0.8, punti);
		glPopMatrix();
		



		glPushMatrix();
		glTranslatef(larghezza*0.4, altezza*0.6, 0);
		glScalef(0.3, 0.3, 0);
		glTranslatef(-larghezza * 0.4, -altezza * 0.6, 0);

		char gameWon[] = "GREAT!";
		stroke_output(larghezza*0.4, altezza*0.6, gameWon);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(larghezza*0.2, altezza*0.5, 0);
		glScalef(0.3, 0.3, 0);
		glTranslatef(-larghezza * 0.2, -altezza * 0.5, 0);

		char gameWon2[] = "YOU SAVED US FROM SPACE INVADERS";
		stroke_output(larghezza*0.2, altezza*0.5, gameWon2);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(larghezza*0.3, altezza*0.3, 0);
		glScalef(0.2, 0.2, 0);
		glTranslatef(-larghezza * 0.3, -altezza * 0.3, 0);

		char returnToMenu[] = "PRESS START TO RETURN TO MENU";
		stroke_output(larghezza*0.3, altezza*0.3, returnToMenu);
		glPopMatrix();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glutSwapBuffers();  //al posto di glFlush(), serve per avviarela pipeline del rendering
						//che porta a disegnare le primitive sullo schermo

}



void myinit(void)
{

	glColor3f(0.0, 0.0, 0.0); //colore disegno
	glPointSize(4.0);
	glLineWidth(2.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, larghezza, 0.0, altezza);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Creo le coordinate per le stelle
	for (int i = 0; i < 200; i++) {
		w[i] = random(larghezza);
		h[i] = random(altezza);
	}

	//Navetta
	glGenBuffers(1, &vbo_navetta);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_navetta);
	glBufferData(GL_ARRAY_BUFFER, sizeof(navetta), navetta, GL_STATIC_DRAW);
	
	//shoot
	glGenBuffers(1, &vbo_shoot);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_shoot);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shoot), shoot, GL_STATIC_DRAW);


	//Invader1
	glGenBuffers(1, &vbo_binvader1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_binvader1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(binvader1), binvader1, GL_STATIC_DRAW);


}




void main(int argc, char** argv)
{
	//For VBO
	GLboolean GlewInitResult;

	glutInit(&argc, argv);
	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	altezza = glutGet(GLUT_SCREEN_HEIGHT);
	larghezza = glutGet(GLUT_SCREEN_WIDTH);
	glutInitWindowSize(larghezza, altezza);
	glutInitWindowPosition(0, 0);
	win_id = glutCreateWindow("Space Invaders");
	glutDisplayFunc(display);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecial);
	glutIdleFunc(Idle);
	

	
	//VBO
	GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
		);
		exit(EXIT_FAILURE);
	}

	myinit();

	

	fprintf(
		stdout,
		"INFO: OpenGLVersion: %s\n",
		glGetString(GL_VERSION)
	);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutMainLoop();
}

