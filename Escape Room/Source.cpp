#define STB_IMAGE_IMPLEMENTATION
#include<iostream>
#include<Windows.h>
#include<GL/stb_image.h>
#include<GL/glut.h>

unsigned int texture;
int width, height, nrChannels;
unsigned char* data = NULL;
bool fullScreenMode = true, lightOn = false;
float fanRotationAngle = 0.0f;
float ratio, eyey = 10, eyez = 40, eyex = 0, centerX, centerY, centerZ = -20;
float matamb[] = { 1.0f,1.0f,1.0f,1.0f },
matdiff[] = { 1.0f,1.0f,1.0f,1.0f },
matspec[] = { 0.64f,1.0f,1.0f,1.0f },
matshin[] = { 4 },
lightamb[] = { 1,0,1,1 },
lightdiff[] = { 0,.6,.6,1 },
lightspec[] = { .2,.2,.2,1 },
lightPos[] = { 1.0, 1.0, 1.0, 1.0 };
void background();
void mydraw();
void reshape(int, int);
void timer(int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void load(int imgnum);
void check(unsigned char* data);

void rightWall();
void leftWall();
void frontWall();
void backWall();
void floor();
void roof();
void Room();
void drawTable();
void drawFan();

void main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutCreateWindow(" 3D shapes and animation");
	glutFullScreen();

	background();
	glutDisplayFunc(mydraw);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);

	glutMainLoop();
}

void background() {
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	// Lighting
	/*glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightspec);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiff);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT, GL_AMBIENT, matamb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matdiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matspec);
	glMaterialfv(GL_FRONT, GL_SHININESS, matshin);*/

}
void toggleLight(int value) {
	lightOn = !lightOn;

	if (lightOn) {
		glEnable(GL_LIGHT0);  // ON
	}
	else {
		glDisable(GL_LIGHT0); // OFF
	}
	//glutPostRedisplay();
}

void mydraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(eyex, eyey, eyez,
		centerX, centerY, centerZ,
		0, 1, 0);

	Room();

	drawTable();
	
	drawFan();
	

	glutSwapBuffers();
}

void reshape(int w, int h) {
	if (h == 0) h = 1;
	ratio = w / (float)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void timer(int v) {
	//toggleLight(v);
	fanRotationAngle += 20.0f; // Adjust speed here
	if (fanRotationAngle > 360.0f)
		fanRotationAngle -= 360.0f;
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 27)
		exit(0);
}

void specialKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_F1) {
		fullScreenMode = !fullScreenMode;
		if (fullScreenMode) {
			glutFullScreen();
		}
		else {
			glutReshapeWindow(GetSystemMetrics(SM_CXSCREEN) / 2,
				GetSystemMetrics(SM_CYSCREEN) / 2);
			glutPositionWindow(GetSystemMetrics(SM_CXSCREEN) / 4,
				GetSystemMetrics(SM_CYSCREEN) / 4);
		}
	}
	if (key == GLUT_KEY_RIGHT) {
		centerX += .9;
		centerZ += .001;
	}
	if (key == GLUT_KEY_LEFT) {
		centerX -= .9;
		centerZ -= .001;
	}
	if (key == GLUT_KEY_DOWN) {
		eyez += 1;
		centerZ += 1;

	}
	if (key == GLUT_KEY_UP) {
		eyez -= 1;
		centerZ -= 1;
	}
	if (key == GLUT_KEY_F3) {
		if (centerY > -2) { // Don't go lower than floor
			centerY -= 0.5;
		}
	}
	if (key == GLUT_KEY_F4) {
		if (centerY < 20) { // Don't go higher than roof
			centerY += 0.5;
		}
	}
	glutPostRedisplay();
}


void load(int imgnum) {
	if (imgnum == 1) {

		data = stbi_load("floor.jpg", &width, &height, &nrChannels, 0);
		check(data);
	}
	else if (imgnum == 2) {

		data = stbi_load("roof.jpg", &width, &height, &nrChannels, 0);
		check(data);
	}
	else if (imgnum == 3) {

		data = stbi_load("table_texture.jpg", &width, &height, &nrChannels, 0);
		check(data);
	}
	else if (imgnum == 4) {

		data = stbi_load("fan_txt.PNG", &width, &height, &nrChannels, 0);
		check(data);
	}
	
}

void check(unsigned char* data) {
	if (data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void floor() {
	glEnable(GL_TEXTURE_2D);
	load(1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(15, -2, 20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-15, -2, 20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(-15, -2, -20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(15, -2, -20);
	glEnd();
}


void roof() {
	load(2);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(15, 20, -20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(15, 20, 20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(-15, 20, 20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(-15, 20, -20);
	glEnd();
}

void rightWall() {
	load(2);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(15, -2, -20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(15, -2, 20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(15, 20, 20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(15, 20, -20);
	glEnd();
}

void leftWall() {
	load(2);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-15, -2, -20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-15, -2, 20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(-15, 20, 20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(-15, 20, -20);
	glEnd();
}

void frontWall() {
	load(2);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-15, -2, -20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(15, -2, -20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(15, 20, -20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(-15, 20, -20);
	glEnd();
}

void backWall() {
	load(2);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-15, -2, 20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(15, -2, 20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(15, 20, 20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(-15, 20, 20);

	glEnd();
}

void Room()
{
	floor();
	roof();
	frontWall();
	//backWall();
	rightWall();
	leftWall();
	glDisable(GL_TEXTURE_2D);
}

void drawTable() {
	glEnable(GL_TEXTURE_2D);
	load(3);  // Load table texture (Capture.PNG)

	// Draw Tabletop
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -8.0f); // Move the table deeper into the room (away from front)

	glBegin(GL_QUADS);

	// Top Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, 0.5f, -3.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0f, 0.5f, -3.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0f, 0.5f, 3.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0f, 0.5f, 3.0f);

	// Bottom Face (optional, depending on your preference for texturing the bottom side)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, -0.5f, -3.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0f, -0.5f, -3.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0f, -0.5f, 3.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0f, -0.5f, 3.0f);

	// Front Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, -0.5f, 3.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0f, -0.5f, 3.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0f, 0.5f, 3.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0f, 0.5f, 3.0f);

	// Back Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, -0.5f, -3.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0f, -0.5f, -3.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0f, 0.5f, -3.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0f, 0.5f, -3.0f);

	// Left Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, -0.5f, -3.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0f, -0.5f, 3.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0f, 0.5f, 3.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0f, 0.5f, -3.0f);

	// Right Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, -0.5f, -3.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0f, -0.5f, 3.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0f, 0.5f, 3.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0f, 0.5f, -3.0f);

	glEnd();
	glPopMatrix();

	// Disable texture if legs don't need textures
	glDisable(GL_TEXTURE_2D);

	// Draw Table Legs
	float legHeight = 2.0f;
	float legSize = 0.3f;
	float offsetX = 4.0f;
	float offsetZ = 2.0f;
	float tableZ = -8.0f; // same as table

	// Enable texture for the legs
	glEnable(GL_TEXTURE_2D);
	load(3);  // Load texture for legs if it's different

	for (int dx = -1; dx <= 1; dx += 2) {
		for (int dz = -1; dz <= 1; dz += 2) {
			glPushMatrix();
			glTranslatef(dx * offsetX, -1.6f, dz * offsetZ + tableZ);
			glScalef(legSize, legHeight, legSize);

			// Draw each leg as a cube with texture coordinates
			glBegin(GL_QUADS);

			// Front Face of Leg
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

			// Back Face of Leg
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);

			// Left Face of Leg
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);

			// Right Face of Leg
			glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

			// Top Face of Leg
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

			// Bottom Face of Leg
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

			glEnd();
			glPopMatrix();
		}
	}

	// Disable texture after drawing legs
	glDisable(GL_TEXTURE_2D);
}

// Function to draw the fan with textures and make it rotate
void drawFan() {
	glEnable(GL_TEXTURE_2D);
	load(4);

	float poleHeight = 3.0f;
	float poleSize = 0.3f;

	glPushMatrix();
	glTranslatef(0.0f, 17.0f, 0.0f); // Position at ceiling level

	// ==== Draw Pole ====
	glPushMatrix();
	glScalef(poleSize, poleHeight, poleSize);

	glBegin(GL_QUADS);
	// Pole faces
	glTexCoord2f(0, 0); glVertex3f(-0.5f, 0, 0.5f);
	glTexCoord2f(1, 0); glVertex3f(0.5f, 0, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, 1, 0.5f);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, 1, 0.5f);

	glTexCoord2f(0, 0); glVertex3f(-0.5f, 0, -0.5f);
	glTexCoord2f(1, 0); glVertex3f(0.5f, 0, -0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, 1, -0.5f);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, 1, -0.5f);

	glTexCoord2f(0, 0); glVertex3f(-0.5f, 0, -0.5f);
	glTexCoord2f(1, 0); glVertex3f(-0.5f, 0, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(-0.5f, 1, 0.5f);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, 1, -0.5f);

	glTexCoord2f(0, 0); glVertex3f(0.5f, 0, -0.5f);
	glTexCoord2f(1, 0); glVertex3f(0.5f, 0, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, 1, 0.5f);
	glTexCoord2f(0, 1); glVertex3f(0.5f, 1, -0.5f);
	glEnd();

	glPopMatrix(); // Done drawing pole

	// ==== Move down slightly ====
	glTranslatef(0.0f, 0.0f, 0.0f);

	// ==== Draw Fan Center ====
	float baseRadius = 1.2f;
	int numSegments = 50;

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5f, 0.5f);
	glVertex3f(0, 0, 0);

	for (int i = 0; i <= numSegments; i++) {
		float angle = i * 2.0f * 3.14159f / numSegments;
		float x = cos(angle) * baseRadius;
		float z = sin(angle) * baseRadius;
		glTexCoord2f(0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle));
		glVertex3f(x, 0, z);
	}
	glEnd();

	// ==== Rotate Blades ====
	glRotatef(fanRotationAngle, 0, 1, 0); 

	// ==== Draw Blades ====
	float bladeLength = 7.5f;
	float bladeWidth = 0.7f;
	float bladeHeight = 0.1f;

	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glRotatef(i * 90.0f, 0, 1, 0);
		glTranslatef(bladeLength / 2, 0, 0);

		glScalef(bladeLength, bladeHeight, bladeWidth);

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-0.5f, 0, 0.5f);
		glTexCoord2f(1, 0); glVertex3f(0.5f, 0, 0.5f);
		glTexCoord2f(1, 1); glVertex3f(0.5f, 0, -0.5f);
		glTexCoord2f(0, 1); glVertex3f(-0.5f, 0, -0.5f);
		glEnd();

		glPopMatrix();
	}

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

