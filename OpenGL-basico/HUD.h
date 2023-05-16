#pragma once
#include "font.h"

void renderTime(float tiempo,GLuint textura);
void renderScore(float score, GLuint textura, int x, int y);
void renderShieldTime(float time, GLuint textura);
void renderBackground();
void renderButton(float x, float y, GLuint textura);
void renderButtonGameOver(float x, float y, GLuint textura);
void renderValue(float x, float y, GLuint textura);
void renderFrames(float frames, GLuint textura);