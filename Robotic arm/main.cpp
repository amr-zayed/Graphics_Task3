#include "imageloader.h"
#include <math.h>
#include<iostream>
#include <glut.h>



#define PI 3.14
using namespace std;
double eye[] = { 0, 0, 3 };
double center[] = { 0, 0, 0 };
double up[] = { 0, 1, 0 };
static int Rshoulder = 0, Relbow = 0, Lshoulder = 0, Lelbow = 0, Lshoulderx = 0, Rshoulderx = 0, RLegx = 0, LLegx = 0, RLegz = 0, LLegz = 0, RKnee = 0, LKnee = 0;
int WallVertex = 5;

// RGBA
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 0.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// x , y, z, w
GLfloat light_position[] = { 0, 5.0, -100.0, 1.0 };
GLfloat lightPos1[] = { 0, -5.0, 100.0, 1.0 };
GLfloat lightPos2[] = { 100.0, 5.0, 0, 1.0 };
GLfloat lightPos3[] = { -100.0, 5.0, 0, 1.0 };
GLfloat lightPos4[] = { 0, 50.0, 0, 1.0 };

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image)
{
	GLuint textureId;
	glGenTextures(1, &textureId);			 //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,				  //Always GL_TEXTURE_2D
		0,							  //0 for now
		GL_RGB,					  //Format OpenGL uses for image
		image->width, image->height, //Width and height
		0,							  //The border of the image
		GL_RGB,					  //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,			  //GL_UNSIGNED_BYTE, because pixels are stored
									 //as unsigned numbers
		image->pixels);			  //The actual pixel data
	return textureId;						  //Returns the id of the texture
}

GLuint _textureId; //The id of the texture
GLuint _textureId1;
//Initializes 3D rendering
void initRendering()
{
	Image* image = loadBMP("images/floor.bmp");
	Image* image1 = loadBMP("images/ceramic.bmp");
	_textureId = loadTexture(image);
	_textureId1 = loadTexture(image1);
	cout << _textureId;
	delete image;
	// Turn on the power
	glEnable(GL_LIGHTING);
	// Flip light switch
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glDisable(GL_LIGHT4);
	// assign light parameters
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular);
	// Material Properties
	GLfloat lightColor1[] = { 0.08f, 0.08f, 0.08f, 0.08f };
	GLfloat lightColor2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT4, GL_DIFFUSE, lightColor2);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);

	// Enable Depth buffer
	glEnable(GL_DEPTH_TEST);
}

void rotatePoint(double a[], double theta, double p[])
{

	double temp[3];
	temp[0] = p[0];
	temp[1] = p[1];
	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];
	temp[1] = a[2] * p[0] - a[0] * p[2];
	temp[2] = -a[1] * p[0] + a[0] * p[1];

	temp[0] *= sin(theta);
	temp[1] *= sin(theta);
	temp[2] *= sin(theta);

	temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta) * p[0];
	temp[1] += cos(theta) * p[1];
	temp[2] += cos(theta) * p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];
}

void crossProduct(double a[], double b[], double c[])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}
void turnLeft()
{
	double theta = -PI / 100;
	rotatePoint(up, theta, eye);
}

void turnRight()
{
	double theta = PI / 100;
	rotatePoint(up, theta, eye);
}

void moveUp()
{
	double horizontal[3];
	double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, PI / 100, eye);
	rotatePoint(horizontal, PI / 100, up);
}


void moveDown()
{

	double horizontal[3];
	double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, -PI / 100, eye);
	rotatePoint(horizontal, -PI / 100, up);
}

void moveForward()
{

	double speed = 0.1;
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	normalize(direction);

	eye[0] += direction[0] * speed;
	eye[1] += direction[1] * speed;
	eye[2] += direction[2] * speed;

	/*center[0] += direction[0] * speed;
	center[2] += direction[2] * speed;*/
}

void moveBack()
{

	double speed = -0.1;
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];

	normalize(direction);

	eye[0] += direction[0] * speed;
	eye[1] += direction[1] * speed;
	eye[2] += direction[2] * speed;
}

void Leg(float pos, float legx, float legz, float knee) {
	glPushMatrix();
	glTranslatef((GLfloat)pos, -5.25, 0);
	glTranslatef(0, 1.2, 0);
	glRotatef((GLfloat)legz, 0, 0, 1);
	glRotatef((GLfloat)legx, 1, 0, 0);
	glTranslatef(0, -1.2, 0);
	glPushMatrix();
	glPushMatrix();
	glScalef(0.8, 2.5, 1);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -1.25, 0);
	glRotatef((GLfloat)knee, 1, 0, 0);
	glTranslatef(0, 1.25, 0);
	glPushMatrix();
	glTranslatef(0, -2.5, 0);
	glPushMatrix();
	glScalef(0.8, 2.5, 1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -4, -0.7);
	glPushMatrix();
	glScalef(1, 0.8, 2.5);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
void Arm(float shoulder, float elbow, float shoulderx) {
	glRotatef((GLfloat)shoulder, 1.0, 0.0, 0.0);
	//glRotatef((GLfloat)shoulder, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)shoulderx, 0.0, 1.0, 0.0);

	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutSolidCube(1);
	glPopMatrix();
	glTranslatef(1.0, 0.0, 0.0);
	glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutSolidCube(1);
	glPopMatrix();

	//Draw finger flang 1 
	glPushMatrix();
	glTranslatef(1.0, -0.25, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	//Draw finger flang 1
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang 2 
	glPushMatrix();
	glTranslatef(1, 0.25, 0.45);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0, 0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	//Draw finger flang 2
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang 3
	glPushMatrix();
	glTranslatef(1.0, 0.25, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	//Draw finger flang 3 
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang 4
	glPushMatrix();
	glTranslatef(1.0, 0.25, -0.45);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0, 0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	//Draw finger flang 4 
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();
}


void screen_menu(int value)
{
	Image* image;
	switch (value) {
	case '1':
		image = loadBMP("images/floor.bmp");
		_textureId = loadTexture(image);
		delete image;
		break;
	case '2':

		image = loadBMP("images/red.bmp");
		_textureId = loadTexture(image);
		delete image;
		break;

	case '3':

		image = loadBMP("images/ceramic.bmp");
		_textureId = loadTexture(image);
		delete image;
		break;
	case '4':

		image = loadBMP("images/carpet.bmp");
		_textureId = loadTexture(image);
		delete image;
		break;

	}
	//reset();
	glutPostRedisplay();
}

void attachMenu()
{
	glutCreateMenu(screen_menu);
	glutAddMenuEntry("floor", '1');
	//glutAddMenuEntry("", 0);
	glutAddMenuEntry("Asphalt", '2');

	glutAddMenuEntry("ceramic", '3');

	glutAddMenuEntry("carpet ", '4');

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int poses[5][12] = { {-95,65,0,-85,-70,0,0,5,0,0,-5,0},{-125,90,105,-15,-90,20,-30,0,-20,0,0,0 },{0,90,0,-105,-75,110,0,0,0,-50,0,20} 
 ,{-5,55,0,-65,-40,0,-30,0,0,0,-70,-40},{0,45,0,10,110,0,-5,0,0,105,-45,0 } };

void setposes(int framenum) {
	Rshoulder = poses[framenum][0];
	Rshoulderx = poses[framenum][1];
	Relbow= poses[framenum][2];
	Lshoulder= poses[framenum][3];
	Lshoulderx =poses[framenum][4];
	Lelbow= poses[framenum][5];
	RLegx= poses[framenum][6];
	RLegz= poses[framenum][7];
	RKnee= poses[framenum][8];
	LLegx= poses[framenum][9];
	LLegz= poses[framenum][10];
	LKnee= poses[framenum][11];

	// and so on
}

static int f = 0;
void timer(int val) {
	f = f % 5;
	setposes(f);
	f++;
	glutPostRedisplay();
	glutTimerFunc(1500, timer, 0);
}


void display(void)
{

	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Clear Depth and Color buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glPushMatrix();
	glLightfv(GL_LIGHT4, GL_POSITION, lightPos4);
	glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_LIGHT4);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-WallVertex, -1, WallVertex);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(WallVertex, -1, WallVertex);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(WallVertex, -1, -WallVertex);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(-WallVertex, -1, -WallVertex);
	glEnd();
	glDisable(GL_LIGHT4);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_LIGHT4);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-WallVertex, WallVertex, WallVertex);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(WallVertex, WallVertex, WallVertex);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(WallVertex, WallVertex, -WallVertex);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(-WallVertex, WallVertex, -WallVertex);
	glEnd();
	glDisable(GL_LIGHT4);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_LIGHT4);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-WallVertex, -1, WallVertex);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(-WallVertex, WallVertex, WallVertex);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(-WallVertex, WallVertex, -WallVertex);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(-WallVertex, -1, -WallVertex);
	glEnd();
	glDisable(GL_LIGHT4);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_LIGHT4);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(WallVertex, -1, WallVertex);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(WallVertex, WallVertex, WallVertex);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(WallVertex, WallVertex, -WallVertex);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(WallVertex, -1, -WallVertex);
	glEnd();
	glDisable(GL_LIGHT4);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_LIGHT4);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-WallVertex, -1, -WallVertex);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(-WallVertex, WallVertex, -WallVertex);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(WallVertex, WallVertex, -WallVertex);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(WallVertex, -1, -WallVertex);
	glEnd();
	glDisable(GL_LIGHT4);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_LIGHT4);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-WallVertex, -1, WallVertex);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(-WallVertex, WallVertex, WallVertex);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(WallVertex, WallVertex, WallVertex);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(WallVertex, -1, WallVertex);
	glEnd();
	glDisable(GL_LIGHT4);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	/*glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);*/
	glScalef(0.1, 0.1, 0.1);
	glRotatef(180, 0.0, 1.0, 0.0);

	//Chest
	glPushMatrix();
	glTranslatef(0, -2, 0);
	glScalef(2, 4, 1);
	glutSolidCube(1);
	glPopMatrix();

	//Right Arm 
	glPushMatrix();
	glTranslatef(1, -0.3, 0);
	Arm(Rshoulder, Relbow, Rshoulderx);
	glPopMatrix();

	//Left Arm
	glPushMatrix();
	glTranslatef(-1, -0.3, 0);
	Arm(Lshoulder, Lelbow, Lshoulderx - 180);
	glPopMatrix();
	//Left Legs
	Leg(-0.6, LLegx, LLegz, LKnee);

	//Right Leg
	Leg(0.6, RLegx, RLegz, RKnee);

	//Head
	glPushMatrix();
	glTranslatef(0, 1.2, 0);
	glutSolidSphere(0.9, 10, 10);
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		turnLeft();
		break;
	case GLUT_KEY_RIGHT:
		turnRight();
		break;
	case GLUT_KEY_UP:
		if (center[1] <= 1.5)
			moveUp();
		break;
	case GLUT_KEY_DOWN:
		if (center[1] >= -1.5)
			moveDown();
		break;
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char Key, int x, int y)
{
	switch (Key)
	{
	/*case 'a':
		Rshoulderx = (Rshoulderx + 5) % 360;
		cout << "Rshoulderx: " << Rshoulderx << endl;
		glutPostRedisplay();
		break;
	case 'A':
		Rshoulderx = (Rshoulderx - 5) % 360;
		cout << "Rshoulderx: " << Rshoulderx << endl;
		glutPostRedisplay();
		break;
	case 's':
		Rshoulder = (Rshoulder + 5) % 360;
		cout << "Rshoulder: " << Rshoulder << endl;
		glutPostRedisplay();
		break;
	case 'S':
		Rshoulder = (Rshoulder - 5) % 360;
		cout << "Rshoulder: " << Rshoulder << endl;
		glutPostRedisplay();
		break;
	case 'e':
		Relbow = (Relbow + 5) % 360;
		cout << "Relbow: " << Relbow << endl;
		glutPostRedisplay();
		break;
	case 'E':
		Relbow = (Relbow - 5) % 360;
		cout << "Relbow: " << Relbow << endl;
		glutPostRedisplay();
		break;
	case 'd':
		Lshoulder = (Lshoulder - 5) % 360;
		cout << "Lshoulder: " << Lshoulder << endl;
		glutPostRedisplay();
		break;
	case 'D':
		Lshoulder = (Lshoulder + 5) % 360;
		cout << "Lshoulder: " << Lshoulder << endl;
		glutPostRedisplay();
		break;
	case 'r':
		Lelbow = (Lelbow - 5) % 360;
		cout << "Lelbow: " << Lelbow << endl;
		glutPostRedisplay();
		break;
	case 'R':
		Lelbow = (Lelbow + 5) % 360;
		cout << "Lelbow: " << Lelbow << endl;
		glutPostRedisplay();
		break;
	case 'g':
		Lshoulderx = (Lshoulderx - 5) % 360;
		cout << "Lshoulderx: " << Lshoulderx << endl;
		glutPostRedisplay();
		break;
	case 'G':
		Lshoulderx = (Lshoulderx + 5) % 360;
		cout << "Lshoulderx: " << Lshoulderx << endl;
		glutPostRedisplay();
		break;
	case 'v':
		LLegx = (LLegx + 5) % 360;
		cout << "LLegx: " << LLegx << endl;
		glutPostRedisplay();
		break;
	case 'V':
		LLegx = (LLegx - 5) % 360;
		cout << "LLegx: " << LLegx << endl;
		glutPostRedisplay();
		break;
	case 'h':
		LLegz = (LLegz + 5) % 360;
		cout << "LLegz: " << LLegz << endl;
		glutPostRedisplay();
		break;
	case 'H':
		LLegz = (LLegz - 5) % 360;
		cout << "LLegz: " << LLegz << endl;
		glutPostRedisplay();
		break;
	case 'n':
		LKnee = (LKnee + 5) % 360;
		cout << "LKnee: " << LKnee << endl;
		glutPostRedisplay();
		break;
	case 'N':
		LKnee = (LKnee - 5) % 360;
		cout << "LKnee: " << LKnee << endl;
		glutPostRedisplay();
		break;
	case 'z':
		RLegx = (RLegx + 5) % 360;
		cout << "RLegx: " << RLegx << endl;
		glutPostRedisplay();
		break;
	case 'Z':
		RLegx = (RLegx - 5) % 360;
		cout << "RLegx: " << RLegx << endl;
		glutPostRedisplay();
		break;
	case 'x':
		RLegz = (RLegz + 5) % 360;
		cout << "RLegz: " << RLegz << endl;
		glutPostRedisplay();
		break;
	case 'X':
		RLegz = (RLegz - 5) % 360;
		cout << "RLegz: " << RLegz << endl;
		glutPostRedisplay();
		break;
	case 'c':
		RKnee = (RKnee + 5) % 360;
		cout << "RKnee: " << RKnee << endl;
		glutPostRedisplay();
		break;
	case 'C':
		RKnee = (RKnee - 5) % 360;
		cout << "RKnee: " << RKnee << endl;
		glutPostRedisplay();
		break;*/
	case 'f':
		moveForward();
		break;
	case 'b':
		moveBack();
		break;
	}
}

void reset()
{
	double e[] = { 0.0, 0.0, 1.0 };
	double c[] = { 0.0, 0.0, 0.0 };
	double u[] = { 0.0, 1.0, 0.0 };
	for (int i = 0; i < 3; i++)
	{
		eye[i] = e[i];
		center[i] = c[i];
		up[i] = u[i];
	}
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutCreateWindow("floor");
	initRendering();
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60, 1.0, 0.1, 15);
	attachMenu();
	glutTimerFunc(0, timer, 0);
	glutDisplayFunc(display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(specialKeys);

	glutMainLoop();
	return 0;
}