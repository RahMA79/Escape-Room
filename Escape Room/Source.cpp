#define STB_IMAGE_IMPLEMENTATION
#include<iostream>
#include<Windows.h>
#include<GL/stb_image.h>
#include<GL/glut.h>

GLuint textures[7];  // VERY IMPORTANT: CHANGE NUMBER BASED ON NUMBER OF TEXTURES


unsigned int texture;
int width, height, nrChannels;
unsigned char* data = NULL;
bool fullScreenMode = true, lightOn = false;
float ratio, eyey = 10, eyez = 40, eyex = 0;
float centerX, centerY, centerZ = 20;


const float PI = 3.14159265;				// Start of Camera Variables
bool firstMouse = true;
float yaw = -80, pitch = -20;
float lastX = 400, lastY = 300;
float sensitivity = 0.05f;
bool skipNextMouseMovement = true;	//End of Camera Variables


float fanRotationAngle = 0.0f;
float matamb[] = { 1.0f,1.0f,1.0f,1.0f },
matdiff[] = { 1.0f,1.0f,1.0f,1.0f },
matspec[] = { 0.64f,1.0f,1.0f,1.0f },
matshin[] = { 4 },
lightamb[] = { 1,0,1,1 },
lightdiff[] = { 0,.6,.6,1 },
lightspec[] = { .2,.2,.2,1 },
lightPos[] = { 1.0, 1.0, 1.0, 1.0 };

float TABLE_TOP_WIDTH = 8, TABLE_TOP_LENGTH = 10, TABLE_TOP_THICKNESS = 1;	// Start of Table Variables
float TABLE_LEG_HEIGHT = 7, TABLE_LEG_WIDTH = 1;
float tableX = 10, tableY, tableZ;											//End of Table Variables


void init_textures();
void use_texture(int);

void background();
void mydraw();
void reshape(int, int);
void fanTimer(int);
void lightTimer(int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
//void load(int imgnum);
//void check(unsigned char* data);
float toRad(float);
void mouseMovement(int, int);

void rightWall();
void leftWall();
void frontWall();
void backWall();
void floor();
void roof();
void Room();
void drawChair();
void chair();
void drawTable();
void drawFan();

void drawCube(float, float, float);
void drawTableLeg(float, float);
void drawTable2();

void main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutCreateWindow(" 3D shapes and animation");
	glutFullScreen();

	background();

	init_textures();
	glutSetCursor(GLUT_CURSOR_NONE);
	glutPassiveMotionFunc(mouseMovement);
	glutDisplayFunc(mydraw);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, fanTimer, 0); // fan
	glutTimerFunc(0, lightTimer, 0); // light
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);

	glutMainLoop();
}

void background() {
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	// Lighting
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightspec);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiff);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT, GL_AMBIENT, matamb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matdiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matspec);
	glMaterialfv(GL_FRONT, GL_SHININESS, matshin);

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

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glRotatef(fanRotationAngle, 0.0f, 1.0f, 0.0f);
	drawFan();
	glPopMatrix();
	glEnable(GL_LIGHTING);

	Room();
	chair();

	drawTable();
	drawTable2();
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
// Light Toggle Timer
void lightTimer(int v) {
	toggleLight(v);
	glutPostRedisplay();
	glutTimerFunc(10000, lightTimer, 0);
}
// fan movement timer
void fanTimer(int v) {
	fanRotationAngle += 20.0f;
	if (fanRotationAngle > 360.0f)
		fanRotationAngle -= 360.0f;

	glutPostRedisplay();
	glutTimerFunc(100, fanTimer, 0);
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
	//if (key == GLUT_KEY_RIGHT) {
	//	centerX += .9;
	//	centerZ += .001;
	//}
	//if (key == GLUT_KEY_LEFT) {
	//	centerX -= .9;
	//	centerZ -= .001;
	//}
	//if (key == GLUT_KEY_DOWN) {
	//	eyez += 1;
	//	centerZ +=1;
	//	
	//}
	//if (key == GLUT_KEY_UP) {
	//	eyez -= 1;
	//	centerZ -=1;
	//}
	//if (key == GLUT_KEY_F3) {
	//	if (centerY > -2) { // Don't go lower than floor
	//		centerY -= 0.5;
	//	}
	//}
	//if (key == GLUT_KEY_F4) {
	//	if (centerY < 20) { // Don't go higher than roof
	//		centerY += 0.5;
	//	}
	//}
	float speed = 1.5;
	if (key == GLUT_KEY_RIGHT) {
		eyex += cos(toRad(yaw) + PI / 2) * speed;
		eyez += sin(toRad(yaw) + PI / 2) * speed;
		centerX += cos(toRad(yaw) + PI / 2) * speed;
		centerZ += sin(toRad(yaw) + PI / 2) * speed;
	}
	if (key == GLUT_KEY_LEFT) {
		eyex -= cos(toRad(yaw) + PI / 2) * speed;
		eyez -= sin(toRad(yaw) + PI / 2) * speed;
		centerX -= cos(toRad(yaw) + PI / 2) * speed;
		centerZ -= sin(toRad(yaw) + PI / 2) * speed;
	}
	if (key == GLUT_KEY_DOWN) {
		eyex -= cos(toRad(yaw)) * speed;
		eyez -= sin(toRad(yaw)) * speed;
		centerX -= cos(toRad(yaw)) * speed;
		centerZ -= sin(toRad(yaw)) * speed;
	}
	if (key == GLUT_KEY_UP) {
		eyex += cos(toRad(yaw)) * speed;
		eyez += sin(toRad(yaw)) * speed;
		centerX += cos(toRad(yaw)) * speed;
		centerZ += sin(toRad(yaw)) * speed;
	}
	glutPostRedisplay();
}

void init_textures() {
	const char* filenames[7] = {
		"", // dummy for index 0
		"floor.jpg",
		"roof.jpg",
		"chair-wood.jpg",
		"table_texture.jpg",
		"fan_txt.PNG",
		"wood.jpg"
	};

	for (int i = 1; i <= 6; ++i) {
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filenames[i], &width, &height, &nrChannels, 0);
		if (data) {
			glGenTextures(1, &textures[i]);
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			std::cout << "Failed to load texture: " << filenames[i] << std::endl;
		}
	}
}

void use_texture(int imgnum) {
	glBindTexture(GL_TEXTURE_2D, textures[imgnum]);
}
//void load(int imgnum) {
//	if (imgnum == 1) {
//
//		data = stbi_load("floor.jpg", &width, &height, &nrChannels, 0);
//		check(data);
//	}
//	else if (imgnum == 2) {
//
//		data = stbi_load("roof.jpg", &width, &height, &nrChannels, 0);
//		check(data);
//	}
//	else if (imgnum == 3) {
//		data = stbi_load("chair-wood.jpg", &width, &height, &nrChannels, 0);
//		check(data);
//	}
//	else if (imgnum == 4) {
//
//		data = stbi_load("table_texture.jpg", &width, &height, &nrChannels, 0);
//		check(data);
//	}
//	else if (imgnum == 5) {
//
//		data = stbi_load("fan_txt.PNG", &width, &height, &nrChannels, 0);
//		check(data);
//	}
//	else if (imgnum == 6) {
//
//		data = stbi_load("wood.jpg", &width, &height, &nrChannels, 0);
//		check(data);
//	}
//}
//
//void check(unsigned char* data) {
//	if (data)
//	{
//		glGenTextures(1, &texture);
//		glBindTexture(GL_TEXTURE_2D, texture);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data);
//}

float toRad(float deg) {
	return (deg * PI) / 180;
}

//void mouseMovement(int horizontalPos, int verticalPos) {
//	if (firstMouse)
//	{
//		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
//		lastX = horizontalPos;
//		lastY = verticalPos;
//		firstMouse = false;
//	}
//	if (skipNextMouseMovement) {
//		skipNextMouseMovement = false;
//		//lastX = glutGet(GLUT_WINDOW_WIDTH) / 2;
//		//lastY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
//		return;
//	}
//	float xoffset = horizontalPos - lastX;
//	float yoffset = lastY - verticalPos;
//	lastX = horizontalPos;
//	lastY = verticalPos;
//
//	xoffset *= sensitivity;
//	yoffset *= sensitivity;
//
//	yaw += xoffset;
//	if (yaw > 360.0f) yaw -= 360.0f;
//	if (yaw < 0.0f) yaw += 360.0f;
//
//	pitch += yoffset;
//	if (pitch > 89)
//		pitch = 89;
//	if (pitch < -89)
//		pitch = -89;
//
//	centerX = eyex + cos(toRad(yaw)) * cos(toRad(pitch));
//	centerY = eyey + sin(toRad(pitch));
//	centerZ = eyez + sin(toRad(yaw)) * cos(toRad(pitch));
//
//	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
//	skipNextMouseMovement = true;
//	glutPostRedisplay();
//}

void mouseMovement(int horizontalPos, int verticalPos) {
	int centerScreenX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int centerScreenY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	if (horizontalPos == centerScreenX && verticalPos == centerScreenY) {
		return;
	}

	float xoffset = horizontalPos - centerScreenX;
	float yoffset = centerScreenY - verticalPos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	if (yaw > 360.0f) yaw -= 360.0f;
	if (yaw < 0.0f) yaw += 360.0f;

	pitch += yoffset;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	centerX = eyex + cos(toRad(yaw)) * cos(toRad(pitch));
	centerY = eyey + sin(toRad(pitch));
	centerZ = eyez + sin(toRad(yaw)) * cos(toRad(pitch));

	glutWarpPointer(centerScreenX, centerScreenY);
	glutPostRedisplay();
}

void floor() {
	use_texture(1);
	glEnable(GL_TEXTURE_2D);
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
	use_texture(2);
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
	use_texture(2);
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
	use_texture(2);
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
	use_texture(2);
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
	use_texture(2);
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

void drawChair()
{
	glEnable(GL_TEXTURE_2D);
	use_texture(3);
	// Surface
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.5f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.5f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.5f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.5f, -1.0f);
	glEnd();

	float legWidth = 0.2f;
	float legHeight = 1.5f;

	// Front Right Leg
	glBegin(GL_QUADS);
	// Front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, 0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, 0.9f);
	// Back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, 0.7f);
	// Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.9f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.9f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, 0.9f);
	// Left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.7f, legHeight, 0.9f);
	glEnd();

	// Front Left Leg
	glBegin(GL_QUADS);
	// Front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, 0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, 0.9f);
	// Back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, 0.7f);
	// Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.9f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.9f, legHeight, 0.9f);
	// Left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.7f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.7f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, 0.9f);
	glEnd();

	// Back Right Leg
	glBegin(GL_QUADS);
	// Front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, -0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, -0.7f);
	// Back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, -0.9f);
	// Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.9f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.9f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, -0.7f);
	// Left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.7f, legHeight, -0.7f);
	glEnd();

	// Back Left Leg
	glBegin(GL_QUADS);
	// Front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, -0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, -0.7f);
	// Back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, -0.9f);
	// Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.9f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.9f, legHeight, -0.7f);
	// Left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.7f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.7f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, -0.7f);
	glEnd();

	// Back support
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 3.5f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 3.5f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.5f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.5f, -1.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void chair()
{
	glPushMatrix();
	glTranslatef(12, -2, -16);
	glScalef(2.0f, 2.0f, 2.0f);
	drawChair();
	glPopMatrix();
}

void drawTable() {
	glEnable(GL_TEXTURE_2D);
	use_texture(4);  // Load table texture (Capture.PNG)

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
	use_texture(4);  // Load texture for legs if it's different

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

void drawFan() {
	glEnable(GL_TEXTURE_2D);

	use_texture(5);

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


void drawCube(float width, float height, float depth) {
	float w = width / 2, h = height / 2, d = depth / 2;
	glBegin(GL_QUADS);

	// Front
	glTexCoord2f(0, 0); glVertex3f(-w, -h, d);
	glTexCoord2f(1, 0); glVertex3f(w, -h, d);
	glTexCoord2f(1, 1); glVertex3f(w, h, d);
	glTexCoord2f(0, 1); glVertex3f(-w, h, d);

	// Back
	glTexCoord2f(0, 0); glVertex3f(w, -h, -d);
	glTexCoord2f(1, 0); glVertex3f(-w, -h, -d);
	glTexCoord2f(1, 1); glVertex3f(-w, h, -d);
	glTexCoord2f(0, 1); glVertex3f(w, h, -d);

	// Left
	glTexCoord2f(0, 0); glVertex3f(-w, -h, -d);
	glTexCoord2f(1, 0); glVertex3f(-w, -h, d);
	glTexCoord2f(1, 1); glVertex3f(-w, h, d);
	glTexCoord2f(0, 1); glVertex3f(-w, h, -d);

	// Right
	glTexCoord2f(0, 0); glVertex3f(w, -h, d);
	glTexCoord2f(1, 0); glVertex3f(w, -h, -d);
	glTexCoord2f(1, 1); glVertex3f(w, h, -d);
	glTexCoord2f(0, 1); glVertex3f(w, h, d);

	// Top
	glTexCoord2f(0, 0); glVertex3f(-w, h, d);
	glTexCoord2f(1, 0); glVertex3f(w, h, d);
	glTexCoord2f(1, 1); glVertex3f(w, h, -d);
	glTexCoord2f(0, 1); glVertex3f(-w, h, -d);

	// Bottom
	glTexCoord2f(0, 0); glVertex3f(-w, -h, -d);
	glTexCoord2f(1, 0); glVertex3f(w, -h, -d);
	glTexCoord2f(1, 1); glVertex3f(w, -h, d);
	glTexCoord2f(0, 1); glVertex3f(-w, -h, d);

	glEnd();
}


// Draw table leg
void drawTableLeg(float x, float z) {
	glPushMatrix();
	glTranslatef(x, TABLE_TOP_THICKNESS, z);
	drawCube(TABLE_LEG_WIDTH, TABLE_LEG_HEIGHT, TABLE_LEG_WIDTH);
	glPopMatrix();
}

// Draw the table
void drawTable2() {
	glPushMatrix();
	glTranslatef(tableX, tableY, tableZ);

	// Draw table top
	glEnable(GL_TEXTURE_2D);
	use_texture(6);
	glPushMatrix();
	glTranslatef(0.0f, TABLE_LEG_HEIGHT - 2, 0.0f);
	drawCube(TABLE_TOP_WIDTH, TABLE_TOP_THICKNESS, TABLE_TOP_LENGTH);
	glPopMatrix();

	// Draw four legs
	float legOffsetX = TABLE_TOP_WIDTH / 2.0f - TABLE_LEG_WIDTH / 2.0f;
	float legOffsetZ = TABLE_TOP_LENGTH / 2.0f - TABLE_LEG_WIDTH / 2.0f;

	drawTableLeg(-legOffsetX, -legOffsetZ);  // Back-left
	drawTableLeg(-legOffsetX, legOffsetZ);   // Front-left
	drawTableLeg(legOffsetX, -legOffsetZ);  // Back-right
	drawTableLeg(legOffsetX, legOffsetZ);   // Front-right
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
