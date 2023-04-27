#include "font.h"

void render0(float x, float y, GLuint textura) {
	glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_QUADS);
    glTexCoord2f(0.005, 0.8);
    glVertex2f(x,y);
    glTexCoord2f(0.04, 0.8);
    glVertex2f(x+30.0, y);
    glTexCoord2f(0.04, 0.7);
    glVertex2f(x+30.0, y+50.0);
    glTexCoord2f(0.005, 0.7);
    glVertex2f(x, y+50.0);
    glEnd();
}
void render1(float x, float y, GLuint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_QUADS);
    glTexCoord2f(0.065, 0.8);
    glVertex2f(x, y);
    glTexCoord2f(0.09, 0.8);
    glVertex2f(x + 30.0, y);
    glTexCoord2f(0.09, 0.7);
    glVertex2f(x + 30.0, y + 50.0);
    glTexCoord2f(0.065, 0.7);
    glVertex2f(x, y + 50.0);
    glEnd();
}
void render2(float x, float y, GLuint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_QUADS);
    glTexCoord2f(0.13, 0.8);
    glVertex2f(x, y);
    glTexCoord2f(0.16, 0.8);
    glVertex2f(x + 30.0, y);
    glTexCoord2f(0.16, 0.7);
    glVertex2f(x + 30.0, y + 50.0);
    glTexCoord2f(0.13, 0.7);
    glVertex2f(x, y + 50.0);
    glEnd();
}
void render3(float x, float y, GLuint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_QUADS);
    glTexCoord2f(0.19, 0.8);
    glVertex2f(x, y);
    glTexCoord2f(0.22, 0.8);
    glVertex2f(x + 30.0, y);
    glTexCoord2f(0.22, 0.7);
    glVertex2f(x + 30.0, y + 50.0);
    glTexCoord2f(0.19, 0.7);
    glVertex2f(x, y + 50.0);
    glEnd();
}
void render4(float x, float y, GLuint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_QUADS);
    glTexCoord2f(0.255, 0.8);
    glVertex2f(x, y);
    glTexCoord2f(0.285, 0.8);
    glVertex2f(x + 30.0, y);
    glTexCoord2f(0.285, 0.7);
    glVertex2f(x + 30.0, y + 50.0);
    glTexCoord2f(0.255, 0.7);
    glVertex2f(x, y + 50.0);
    glEnd();
}
void render5(float x, float y, GLuint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_QUADS);
    glTexCoord2f(0.32, 0.8);
    glVertex2f(x, y);
    glTexCoord2f(0.35, 0.8);
    glVertex2f(x + 30.0, y);
    glTexCoord2f(0.35, 0.7);
    glVertex2f(x + 30.0, y + 50.0);
    glTexCoord2f(0.32, 0.7);
    glVertex2f(x, y + 50.0);
    glEnd();
}
void render6(float x, float y, GLuint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_QUADS);
    glTexCoord2f(0.385, 0.8);
    glVertex2f(x, y);
    glTexCoord2f(0.415, 0.8);
    glVertex2f(x + 30.0, y);
    glTexCoord2f(0.415, 0.7);
    glVertex2f(x + 30.0, y + 50.0);
    glTexCoord2f(0.385, 0.7);
    glVertex2f(x, y + 50.0);
    glEnd();
}
void render7(float x, float y, GLuint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_QUADS);
    glTexCoord2f(0.45, 0.8);
    glVertex2f(x, y);
    glTexCoord2f(0.475, 0.8);
    glVertex2f(x + 30.0, y);
    glTexCoord2f(0.475, 0.7);
    glVertex2f(x + 30.0, y + 50.0);
    glTexCoord2f(0.45, 0.7);
    glVertex2f(x, y + 50.0);
    glEnd();
}
void render8(float x, float y, GLuint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_QUADS);
    glTexCoord2f(0.51, 0.8);
    glVertex2f(x, y);
    glTexCoord2f(0.54, 0.8);
    glVertex2f(x + 30.0, y);
    glTexCoord2f(0.54, 0.7);
    glVertex2f(x + 30.0, y + 50.0);
    glTexCoord2f(0.51, 0.7);
    glVertex2f(x, y + 50.0);
    glEnd();
}
void render9(float x, float y, GLuint textura) {
    glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_QUADS);
    glTexCoord2f(0.57, 0.8);
    glVertex2f(x, y);
    glTexCoord2f(0.6, 0.8);
    glVertex2f(x + 30.0, y);
    glTexCoord2f(0.6, 0.7);
    glVertex2f(x + 30.0, y + 50.0);
    glTexCoord2f(0.57, 0.7);
    glVertex2f(x, y + 50.0);
    glEnd();
}