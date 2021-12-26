#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PI 3.14
//variable untuk kamera
float angle=0.0, deltaAngle = 0.0, ratio;
float x=-10.0f,y=25.0f,z=160.0f; // posisi awal kamera
float lx=0.0f,ly=0.0f,lz=-1.0f;
int deltaMove = 0,h,w;
//variable pesawat dan gedung
float posY = 0, posX = 0, posZ = 0, wingDownY=0, wingDownX=0, bodyDownX=0,bodyDownY=0, angleWing=0;
static int rotAngleX =0, rotAngleY =0, rotAngleZ =0;
int planeMove = 0, planeBankLeft = 1, crash = 0, planeIdle=1;
float angleZ = 0, angleX = 0, buildingAngle = 0,buildDown=0;
GLUquadricObj *IDquadric;
// Variable untuk pencahayaan
const GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 0.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 20.0f, 10.0f, 1.0f };
const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

void init(void)
{
	glEnable (GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	IDquadric=gluNewQuadric(); // Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(IDquadric, GLU_SMOOTH); // Create Smooth Normals ( NEW )
	gluQuadricTexture(IDquadric, GL_TRUE); // Create Texture Coords ( NEW )
}
void Reshape(int w1, int h1)
{
	// Fungsi reshape
	if(h1 == 0) h1 = 1;
	w = w1;
	h = h1;
	ratio = 1.0f * w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45,ratio,0.1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx,y + ly,z + lz, 0.0f,1.0f,0.0f);
}

void orientMe(float ang)
{
	// Fungsi ini untuk memutar arah kamera (tengok kiri/kanan)
	lx = sin(ang/10);
	lz = -cos(ang/10);
	glLoadIdentity();
	gluLookAt(x, y, z,x + lx,y + ly,z + lz, 0.0f,1.0f,0.0f);
}

void moveMeFlat(int i)
{
	// Fungsi ini untuk maju mundur kamera
	x = x + i*(lx)*0.1;
	z = z + i*(lz)*0.1;
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx,y + ly,z + lz, 0.0f,1.0f,0.0f);
}

void update(int value) {
    if(crash==1) {
    	planeIdle=0;
    	angleWing -=3;
    	wingDownX = -40-posX;
    	bodyDownX -= 2;
    	bodyDownY -= 1;
    	buildingAngle -= 3;
        if(angleWing < -25){
        	angleWing = -25;
        	wingDownY -= 1;

		}

		if(bodyDownY<=-25){
			bodyDownY=-25;
		}

		if(bodyDownX<=-75){
			bodyDownX=-75;

		}

		if(wingDownY <= -25){
        		wingDownY= -25;
        }

        if (buildingAngle <= -90) {
            buildingAngle = -90;
            buildDown-=1;
            if(buildDown <= -20){
            	buildDown = -20;
            }
        }
    }


	if(planeIdle){
	    if (planeBankLeft) {
	        angleX += 0.5;
	    } else {
	        angleX -= 0.5;
	    }


	    if (angleX > 4) {
	        planeBankLeft = 0;
	    } else if (angleX < -4) {
	        planeBankLeft = 1;
	    }
	}else{
		angleX=0;
	}


    if (posX <= -40) {
        crash = 1;
    }

    glutPostRedisplay();
    glutTimerFunc(50,update,0);//delay
}

void keyboard(unsigned char key, int x, int y)
{
	 switch (key)
	{
		case 'w': rotAngleX += 2; break;
		case 's': rotAngleX -= 2; break;
		case 'a': rotAngleY += 2; break;
		case 'd': rotAngleY -= 2; break;
		case 'q': rotAngleZ += 2; break;
		case 'e': rotAngleZ -= 2; break;
		case 'i': posX -= 1; break;
		case 'k': posX += 1; break;
		case 32 : rotAngleX=rotAngleY=rotAngleZ=0;posY = 0, posX = 0, posZ = 0; break;
		case 'z' : exit(0);
		default:
		break;
	}glutPostRedisplay();
}

void pressKey(int k, int x, int y)
{
	// Fungsi ini akan dijalankan saat tombol keyboard ditekan dan belum dilepas
 	// Selama tombol ditekan, variabel angle dan move diubah => kamera  bergerak
	switch (k)
	{
		case GLUT_KEY_UP : deltaMove = 1;break;
		case GLUT_KEY_DOWN : deltaMove = -1;break;
		case GLUT_KEY_LEFT : deltaAngle = -0.01f;break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.01f;break;
	 }
}

void releaseKey(int key, int x, int y)
{
	// Fungsi ini akan dijalankan saat tekanan tombol keyboard dilepas
	// Saat tombol dilepas, variabel angle dan move diset nol => kamera berhenti
	switch (key)
	{
		case GLUT_KEY_UP : if (deltaMove > 0)
		deltaMove = 0;
		break;

		case GLUT_KEY_DOWN : if (deltaMove < 0)
		deltaMove = 0;
		break;

		case GLUT_KEY_LEFT : if (deltaAngle < 0.0f)
		deltaAngle = 0.0f;
		break;

		case GLUT_KEY_RIGHT : if (deltaAngle > 0.0f)
		deltaAngle = 0.0f;
		break;


 	}
}

void lighting(){
	// Fungsi mengaktifkan pencahayaan
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

void badanP()
{	glPushMatrix();
	glTranslatef(-7.0f,12.0f,0.0f);
	glRotated(90, 0, 1, 0);
	gluCylinder(IDquadric, 2.5f, 2.5f, 17.5f, 32, 32);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(10.0f,12.0f,0.0f);
	glRotated(53, 0, 1, 0);
	glRotated(55, 0, 1, 1);
	gluCylinder(IDquadric, 2.55f, 1.0f, 8.0f, 32, 32);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-11.8f,10.8f,0.0f);
	glRotated(53, 0, 1, 0);
	glRotated(55, 0, 1, 1);
	gluCylinder(IDquadric, 1.0f, 2.6f, 5.5f, 32, 32);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-11.5f,10.9f,0.0f);
	gluSphere(IDquadric, 1.0f, 32, 32);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(17.5f,13.7f,0.0f);
	gluSphere(IDquadric, 1.0f, 32, 32);
	glPopMatrix();

}
void triangularPrism()
{
    glBegin(GL_QUADS);
        glVertex3f(0.5, 0, 0.5);
        glVertex3f(0.5, 0, -0.5);
        glVertex3f(-0.5, 0, -0.5);
        glVertex3f(-0.5, 0, 0.5);
        glVertex3f(0.5,0,-0.5);
        glVertex3f(0.5,1,-0.5);
        glVertex3f(-0.5,1,-0.5);
        glVertex3f(-0.5,0,-0.5);
        glVertex3f(0.5,1,-0.5);
        glVertex3f(-0.5,1,-0.5);
        glVertex3f(-0.5,0,0.5);
        glVertex3f(0.5,0,0.5);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex3f(0.5,0,0.5);
        glVertex3f(0.5,1,-0.5);
        glVertex3f(0.5,0,-0.5);
        glVertex3f(-0.5,0,0.5);
        glVertex3f(-0.5,1,-0.5);
        glVertex3f(-0.5,0,-0.5);
    glEnd();
}

void wingL(){
	//wingLeft
	glPushMatrix();
	glTranslatef(1.0,11.0,0.0);
	glRotated(-90,0,1,0);
	glRotated(-90,0,0,1);
	glScalef(0.5,25.5,5.0);
	triangularPrism();
	glPopMatrix();
}

void wingR(){
	//wingRight
	glPushMatrix();
	glTranslatef(1.0,11.0,0.0);
	glRotated(-90,0,1,0);
	glRotated(90,0,0,1);
	glScalef(0.5,25.5,5.0);
	triangularPrism();
	glPopMatrix();
}

void tailP(){
	//Tail
	glPushMatrix();
	glTranslatef(15.0,13.0,0.0);
	glRotated(-90,0,1,0);
	glScalef(0.5,8.0,5.0);
	triangularPrism();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(15.0,13.0,0.0);
	glRotated(-90,0,1,0);
	glRotated(-90,0,0,1);
	glScalef(0.5,8.0,5.0);
	triangularPrism();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(15.0,13.0,0.0);
	glRotated(-90,0,1,0);
	glRotated(90,0,0,1);
	glScalef(0.5,8.0,5.0);
	triangularPrism();
	glPopMatrix();
}

void Grid()
{
	 // Fungsi untuk membuat grid di "lantai"
	 double i;
	 const float Z_MIN = -150, Z_MAX = 150;
	 const float X_MIN = -150, X_MAX = 150;
	 const float gap = 2;
	 glColor3f(0.5, 0.5, 0.5);
	 glBegin(GL_LINES);
	 for(i=Z_MIN;i<Z_MAX;i+=gap)
	 {
		 glVertex3f(i, 0, Z_MIN);
		 glVertex3f(i, 0, Z_MAX);
	 }
	 for(i=X_MIN;i<X_MAX;i+=gap)
	 {
		 glVertex3f(X_MIN, 0, i);
		 glVertex3f(X_MAX, 0, i);
	 }
	 glEnd();
}

void Grid2()
{
	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_QUADS);
	glVertex3f(-150, 0, 150);
	glVertex3f(-150, 0, -150);
	glVertex3f(150, 0, -150);
	glVertex3f(150, 0, 150);
	glEnd();
}

void Balok(float panjang,float lebar,float tinggi)
{
	glPushMatrix();
	float p=panjang/2;
	float l=lebar/2;
	float t=tinggi/2;
	//depan
	glBegin(GL_QUADS);
	glVertex3f(-p,0,l);
	glVertex3f(p,0,l);
	glVertex3f(p,-t*2,l);
	glVertex3f(-p,-t*2,l);
	glEnd();
	// belakang
	glBegin(GL_QUADS);
	glVertex3f(-p,0,-l);
	glVertex3f(p,0,-l);
	glVertex3f(p,-t*2,-l);
	glVertex3f(-p,-t*2,-l);
	glEnd();
	// atas
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-p,0,-l);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(p,0,-l);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(p,0,l);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-p,0,l);
	glEnd();
	// bawah
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-p,-t*2,-l);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(p,-t*2,-l);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(p,-t*2,l);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-p,-t*2,l);
	glEnd();
	// kanan
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-p,-t*2,-l);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-p,-t*2,l);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-p,0,l);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-p,0,-l);
	glEnd();
	// kiri
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(p,-t*2,-l);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(p,-t*2,l);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(p,0,l);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(p,0,-l);
	glEnd();
	glPopMatrix();
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (deltaMove)
		moveMeFlat(deltaMove);
	if (deltaAngle) {
		angle += deltaAngle;
		orientMe(angle);
	}
	glPushMatrix();
		glRotated(rotAngleX+10, 1, 0, 0);
		glRotated(rotAngleY, 0, 1, 0);
		glRotated(rotAngleZ, 0, 0, 1);
		glRotated(-90,0,1,0);
		glPushMatrix();
			glColor3f(1,0.5,0.2);
			int tinggi = 50;
			glTranslatef(0,tinggi/2,-10);
			Balok(15,10,tinggi/2);
			glPushMatrix();
				glTranslatef(0,buildDown,0);
				glRotated( buildingAngle,1,0,0);
				glTranslatef(0,tinggi/2,0);
				Balok(15,10,tinggi/2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0,buildDown,0);
				glRotated( buildingAngle,0,0,1);
				glTranslatef(0,tinggi/2,20);
				Balok(15,10,tinggi/2);
			glPopMatrix();
			glTranslatef(0,0,20);
			Balok(15,10,tinggi/2);
		glPopMatrix();
		Grid();
		Grid2();
		glTranslatef(50,15,0);
		glTranslated(posX, posY, posZ);
        glRotated(angleX, 1, 0, 0);
		glPushMatrix();
			glTranslatef(bodyDownX,bodyDownY,0);
			badanP();
			tailP();
		glPopMatrix();
		glTranslatef(wingDownX,wingDownY,0);
		glRotated(-angleWing,0,1,0);
		wingL();
		glRotated(angleWing*2,0,1,0);
		wingR();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("Pesawat 3D");
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
	glutReshapeFunc(Reshape);
    glutTimerFunc(50, update, 0);
	lighting();
	init();
	glutMainLoop();
	return(0);
}
