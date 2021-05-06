/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 * robot.c
 * This program shows how to composite modeling transformations
 * to draw translated and rotated hierarchical models.
 * Interaction:  pressing the s and e keys (shoulder and elbow)
 * alters the rotation of the robot arm.
 */



#include <stdlib.h>
#include <glut.h>
#include <math.h>
using namespace std;

#define PI 3.14

static int Rshoulder = 0, Relbow = 0, Lshoulder = 0, Lelbow = 0, Lshoulderx = 0, Rshoulderx = 0, RLegx = 0, LLegx = 0, RLegz = 0, LLegz = 0, RKnee = 0, LKnee = 0;
int moving, startx, starty;
double eye[] = {0.0, -4.0, -10.0};
double center[] = {0.0, -4.0, 0.0};
double up[] = {0.0, 1.0, 0.0};
double down[] = {0.0, 1.0, 0.0};


GLfloat angle = 180.0;  
GLfloat angle2 = 0.0;  


void init(void)
{
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0,
				   1.0,
				   1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
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

// Rotation about vertical direction
void lookRight()
{
	rotatePoint(up, PI / 8, eye);
}

void lookLeft()
{
	rotatePoint(up, -PI / 8, eye);
}

// Rotation about horizontal direction

void lookUp()
{
	double horizontal[3];
	double look[] = {center[0] - eye[0], center[1] - eye[1], center[2] - eye[2]};
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, PI / 8, eye);
	rotatePoint(horizontal, PI / 8, up);
}

void lookDown()
{
	double horizontal[3];
	double look[] = {center[0] - eye[0], center[1] - eye[1], center[2] - eye[2]};
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, -PI / 8, eye);
	rotatePoint(horizontal, -PI / 8, up);
	//Write your code here
}

// Forward and Backward
void moveForward()
{
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	float speed = 0.1;
	eye[0] += direction[0] * speed;
	eye[1] += direction[1] * speed;
	eye[2] += direction[2] * speed;

	/*center[0] += direction[0] * speed;
	center[1] += direction[1] * speed;
	center[2] += direction[2] * speed;*/
}

void moveBackword()
{
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	float speed = -0.1;
	eye[0] += direction[0] * speed;
	eye[1] += direction[1] * speed;
	eye[2] += direction[2] * speed;

	/*center[0] += direction[0] * speed;
	center[1] += direction[1] * speed;
	center[2] += direction[2] * speed;*/
	//Write your code here
}
void reset()
{
	double e[] = {0.0, -4.0, -10.0};
	double c[] = {0.0, -4.0, 0.0};
	double u[] = {0.0, 1.0, 0.0};
	for (int i = 0; i < 3; i++)
	{
		eye[i] = e[i];
		center[i] = c[i];
		up[i] = u[i];
	}
	angle = 180;
	angle2 = 0;
}

void Leg(float pos, float legx, float legz, float knee){
	glPushMatrix();
		glTranslatef((GLfloat) pos,-5.25,0);
		glTranslatef(0,1.2,0);
		glRotatef((GLfloat)legz,0,0,1);
		glRotatef((GLfloat)legx,1,0,0);
		glTranslatef(0,-1.2,0);
		glPushMatrix();
			glPushMatrix();
				glScalef(0.8,2.5,1);
				glutWireCube(1);
			glPopMatrix();	
			glPushMatrix();
				glTranslatef(0,-1.25,0);
				glRotatef((GLfloat)knee,1,0,0);
				glTranslatef(0,1.25,0);
				glPushMatrix();
					glTranslatef(0,-2.5,0);
					glPushMatrix();
						glScalef(0.8,2.5,1);
						glutWireCube(1);
					glPopMatrix();
				glPopMatrix();

				glPushMatrix();
					glTranslatef(0,-4,-0.7);
					glPushMatrix();
						glScalef(1,0.8,2.5);
						glutSolidCube(1);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();

	glPopMatrix();
}
void Arm(float shoulder, float elbow, float shoulderx){
	glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
	glRotatef ((GLfloat) shoulderx, 1.0, 0.0, 0.0);
	glTranslatef (1.0, 0.0, 0.0);
	glPushMatrix();
		glScalef (2.0, 0.6, 1.0);
		glutWireCube (1.0);
	glPopMatrix();
	glTranslatef (1.0, 0.0, 0.0);
	glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
	glTranslatef (1.0, 0.0, 0.0);
	glPushMatrix();
		glScalef (2.0, 0.6, 1.0);
		glutWireCube (1.0);
	glPopMatrix();

   //Draw finger flang 1 
   glPushMatrix();
	   glTranslatef(1.0, -0.25, 0.0);
	   glRotatef(0.0, 0.0, 0.0, 1.0);
	   glTranslatef(0.15, 0.0, 0.0);
	   glPushMatrix();
		   glScalef(0.3, 0.1, 0.1);
		   glutWireCube(1);
	   glPopMatrix();
	   //Draw finger flang 1
	   glTranslatef(0.15, 0.0, 0.0);
	   glRotatef(0.0, 0.0, 0.0, 1.0);
	   glTranslatef(0.15, 0.0, 0.0);
	   glPushMatrix();
		   glScalef(0.3, 0.1, 0.1);
		   glutWireCube(1);
	   glPopMatrix();
   glPopMatrix();
   
   //Draw finger flang 2 
   glPushMatrix();
	   glTranslatef(1, 0.25, 0.45);
	   glRotatef(0.0, 0.0, 0.0, 1.0);
	   glTranslatef(0.15, 0, 0);
	   glPushMatrix();
		   glScalef(0.3, 0.1, 0.1);
		   glutWireCube(1);
	   glPopMatrix();
	   //Draw finger flang 2
	   glTranslatef(0.15, 0.0, 0.0);
	   glRotatef(0.0, 0.0, 0.0, 1.0);
	   glTranslatef(0.15, 0.0, 0.0);
	   glPushMatrix();
		   glScalef(0.3, 0.1, 0.1);
		   glutWireCube(1);
	   glPopMatrix();
   glPopMatrix();

   //Draw finger flang 3
   glPushMatrix();
	   glTranslatef(1.0, 0.25, 0.0);
	   glRotatef(0.0, 0.0, 0.0, 1.0);
	   glTranslatef(0.15, 0, 0.0);
	   glPushMatrix();
		   glScalef(0.3, 0.1, 0.1);
		   glutWireCube(1);
	   glPopMatrix();
	   //Draw finger flang 3 
	   glTranslatef(0.15, 0.0, 0.0);
	   glRotatef(0.0, 0.0, 0.0, 1.0);
	   glTranslatef(0.15, 0.0, 0.0);
	   glPushMatrix();
		   glScalef(0.3, 0.1, 0.1);
		   glutWireCube(1);
	   glPopMatrix();
   glPopMatrix();

   //Draw finger flang 4
   glPushMatrix();
	   glTranslatef(1.0, 0.25, -0.45);
	   glRotatef(0.0, 0.0, 0.0, 1.0);
	   glTranslatef(0.15, 0, 0);
	   glPushMatrix();
		   glScalef(0.3, 0.1, 0.1);
		   glutWireCube(1);
	   glPopMatrix();
	   //Draw finger flang 4 
	   glTranslatef(0.15, 0.0, 0.0);
	   glRotatef(0.0, 0.0, 0.0, 1.0);
	   glTranslatef(0.15, 0.0, 0.0);
	   glPushMatrix();
		   glScalef(0.3, 0.1, 0.1);
		   glutWireCube(1);
	   glPopMatrix();
   glPopMatrix();
}
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2],
			  center[0], center[1], center[2],
			  up[0], up[1], up[2]);
	//glColor3f(0.0, 1.0, 0.0);
   glPushMatrix();
		glRotatef(angle2, 1.0, 0.0, 0.0);
		glRotatef(angle, 0.0, 1.0, 0.0);
		glRotatef(180, 0.0, 1.0, 0.0);

		//Chest
		glPushMatrix();
			glTranslatef(0,-2,0);
			glScalef(2,4,1);
			glutWireCube(1);
		glPopMatrix();

		//Right Arm 
		glPushMatrix();
			glTranslatef(1,-0.3,0);
			Arm(Rshoulder, Relbow, Rshoulderx);
		glPopMatrix();
		
		//Left Arm
		glPushMatrix();
			glTranslatef(-1,-0.3,0);
			Arm(Lshoulder+180, Lelbow, Lshoulderx);
		glPopMatrix();
		//Left Legs
		Leg(-0.6, LLegx, LLegz, LKnee);

		//Right Leg
		Leg(0.6, RLegx, RLegz, RKnee);

		//Head
		glPushMatrix();
		glTranslatef(0,1.2,0);
		glutWireSphere(0.9,10,10);
		glPopMatrix();
   glPopMatrix();
   glutSwapBuffers();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 0.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -5.0);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
	case 'a':
		if (Rshoulderx<90)
		{
			Rshoulderx = (Rshoulderx + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'A':
		if (Rshoulderx>0)
		{
			Rshoulderx = (Rshoulderx - 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 's':
		if (Rshoulder<50)
		{
			Rshoulder = (Rshoulder + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'S':
		if (Rshoulder>-50)
		{
			Rshoulder = (Rshoulder - 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'e':
		if (Relbow<0)
		{
			Relbow = (Relbow + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'E':
		if (Relbow>-90)
		{
			Relbow = (Relbow - 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'd':
		if (Lshoulder>-50)
		{
			Lshoulder = (Lshoulder - 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'D':
		if (Lshoulder<50)
		{
			Lshoulder = (Lshoulder + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'r':
		if (Lelbow>0)
		{
			Lelbow = (Lelbow - 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'R':
		if (Lelbow<90)
		{
			Lelbow = (Lelbow + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'f':
		if (Lshoulderx>-90)
		{
			Lshoulderx = (Lshoulderx - 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'F':
		if (Lshoulderx<0)
		{
			Lshoulderx = (Lshoulderx + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'v':
		if (LLegx<60)
		{
			LLegx = (LLegx + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'V':
		if (LLegx>-85)
		{
			LLegx = (LLegx - 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'b':
		if (LLegz<5)
		{
			LLegz = (LLegz + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'B':
		if (LLegz>-70)
		{
			LLegz = (LLegz - 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'n':
		if (LKnee<5)
		{
			LKnee = (LKnee + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'N':
		if (LKnee>-110)
		{
			LKnee = (LKnee - 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'z':
		if (RLegx<60)
		{
			RLegx = (RLegx + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'Z':
		if (RLegx>-85)
		{
			RLegx = (RLegx - 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'x':
		if (RLegz<70)
		{
			RLegz = (RLegz + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'X':
		if (RLegz>-5)
		{
			RLegz = (RLegz - 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'c':
		if (RKnee<5)
		{
			RKnee = (RKnee + 5) % 360;
		}
		glutPostRedisplay();
		break;
	case 'C':
		if (RKnee>-110)
		{
			RKnee = (RKnee - 5) % 360;
		}
		glutPostRedisplay();
		break;
   case 27:
      exit(0);
      break;
	case 'j':
		moveForward();
		break;
	case 'k':
		moveBackword();
		break;
	case 'l':
		reset();
		break;
	}
	glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		lookLeft();
		break;
	case GLUT_KEY_RIGHT:
		lookRight();
		break;
	case GLUT_KEY_UP:
		lookUp();
		break;
	case GLUT_KEY_DOWN:
		lookDown();
		break;
	}
	glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      moving = 1;
      startx = x;
      starty = y;
    }
    if (state == GLUT_UP) {
      moving = 0;
    }
  }
}


static void motion(int x, int y)
{
  if (moving) {
    angle = angle + (x - startx);
    angle2 = angle2 + (y - starty);
    startx = x;
    starty = y;
    glutPostRedisplay();
  }
}



int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialKeys);
   init();
   glutMainLoop();
   return 0;
}