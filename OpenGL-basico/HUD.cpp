#include "HUD.h"


void renderTime(float tiempo) {
	switch ((int)tiempo % 10) {
	case 0:
		render0(90, 0);
		break;
	case 1:
		render1(90, 0);
		break;
	case 2:
		render2(90, 0);
		break;
	case 3:
		render3(90, 0);
		break;
	case 4:
		render4(90, 0);
		break;
	case 5:
		render5(90, 0);
		break;
	case 6:
		render6(90, 0);
		break;
	case 7:
		render7(90, 0);
		break;
	case 8:
		render8(90, 0);
		break;
	case 9:
		render9(90, 0);
		break;
	}
	switch (((int)tiempo % 100 - (int)tiempo % 10) / 10) {
	case 0:
		render0(60, 0);
		break;
	case 1:
		render1(60, 0);
		break;
	case 2:
		render2(60, 0);
		break;
	case 3:
		render3(60, 0);
		break;
	case 4:
		render4(60, 0);
		break;
	case 5:
		render5(60, 0);
		break;
	case 6:
		render6(60, 0);
		break;
	case 7:
		render7(60, 0);
		break;
	case 8:
		render8(60, 0);
		break;
	case 9:
		render9(60, 0);
		break;
	}
	switch (((int)tiempo % 1000 - (int)tiempo % 100) / 100) {
	case 0:
		render0(30, 0);
		break;
	case 1:
		render1(30, 0);
		break;
	case 2:
		render2(30, 0);
		break;
	case 3:
		render3(30, 0);
		break;
	case 4:
		render4(30, 0);
		break;
	case 5:
		render5(30, 0);
		break;
	case 6:
		render6(30, 0);
		break;
	case 7:
		render7(30, 0);
		break;
	case 8:
		render8(30, 0);
		break;
	case 9:
		render9(30, 0);
		break;
	}
	switch (((int)tiempo % 10000 - (int)tiempo % 1000) / 1000) {
	case 0:
		render0(0, 0);
		break;
	case 1:
		render1(0, 0);
		break;
	case 2:
		render2(0, 0);
		break;
	case 3:
		render3(0, 0);
		break;
	case 4:
		render4(0, 0);
		break;
	case 5:
		render5(0, 0);
		break;
	case 6:
		render6(0, 0);
		break;
	case 7:
		render7(0, 0);
		break;
	case 8:
		render8(0, 0);
		break;
	case 9:
		render9(0, 0);
		break;
	}
}

void renderScore(float score, int x, int y) {
	switch ((int)score % 10) {
	case 0:
		render0(x, y);
		break;
	case 1:
		render1(x, y);
		break;
	case 2:
		render2(x, y);
		break;
	case 3:
		render3(x, y);
		break;
	case 4:
		render4(x, y);
		break;
	case 5:
		render5(x, y);
		break;
	case 6:
		render6(x, y);
		break;
	case 7:
		render7(x, y);
		break;
	case 8:
		render8(x, y);
		break;
	case 9:
		render9(x, y);
		break;
	}
	switch (((int)score % 100 - (int)score % 10) / 10) {
	case 0:
		render0(x - 30, y);
		break;
	case 1:
		render1(x - 30, y);
		break;
	case 2:
		render2(x - 30, y);
		break;
	case 3:
		render3(x - 30, y);
		break;
	case 4:
		render4(x - 30, y);
		break;
	case 5:
		render5(x - 30, y);
		break;
	case 6:
		render6(x - 30, y);
		break;
	case 7:
		render7(x - 30, y);
		break;
	case 8:
		render8(x - 30, y);
		break;
	case 9:
		render9(x - 30, y);
		break;
	}
	switch (((int)score % 1000 - (int)score % 100) / 100) {
	case 0:
		render0(x - 60, y);
		break;
	case 1:
		render1(x - 60, y);
		break;
	case 2:
		render2(x - 60, y);
		break;
	case 3:
		render3(x - 60, y);
		break;
	case 4:
		render4(x - 60, y);
		break;
	case 5:
		render5(x - 60, y);
		break;
	case 6:
		render6(x - 60, y);
		break;
	case 7:
		render7(x - 60, y);
		break;
	case 8:
		render8(x - 60, y);
		break;
	case 9:
		render9(x - 60, y);
		break;
	}
	switch (((int)score % 10000 - (int)score % 1000) / 1000) {
	case 0:
		render0(x - 90, y);
		break;
	case 1:
		render1(x - 90, y);
		break;
	case 2:
		render2(x - 90, y);
		break;
	case 3:
		render3(x - 90, y);
		break;
	case 4:
		render4(x - 90, y);
		break;
	case 5:
		render5(x - 90, y);
		break;
	case 6:
		render6(x - 90, y);
		break;
	case 7:
		render7(x - 90, y);
		break;
	case 8:
		render8(x - 90, y);
		break;
	case 9:
		render9(x - 90, y);
		break;
	}
	switch (((int)score % 100000 - (int)score % 10000) / 10000) {
	case 0:
		render0(x - 120, y);
		break;
	case 1:
		render1(x - 120, y);
		break;
	case 2:
		render2(x - 120, y);
		break;
	case 3:
		render3(x - 120, y);
		break;
	case 4:
		render4(x - 120, y);
		break;
	case 5:
		render5(x - 120, y);
		break;
	case 6:
		render6(x - 120, y);
		break;
	case 7:
		render7(x - 120, y);
		break;
	case 8:
		render8(x - 120, y);
		break;
	case 9:
		render9(x - 120, y);
		break;
	}
}

void renderFrames(float frames) {
	switch ((int)frames % 10) {
	case 0:
		render0(1250, 500);
		break;
	case 1:
		render1(1250, 500);
		break;
	case 2:
		render2(1250, 500);
		break;
	case 3:
		render3(1250, 500);
		break;
	case 4:
		render4(1250, 500);
		break;
	case 5:
		render5(1250, 500);
		break;
	case 6:
		render6(1250, 500);
		break;
	case 7:
		render7(1250, 500);
		break;
	case 8:
		render8(1250, 500);
		break;
	case 9:
		render9(1250, 500);
		break;
	}
	switch (((int)frames % 100 - (int)frames % 10) / 10) {
	case 0:
		render0(1220, 500);
		break;
	case 1:
		render1(1220, 500);
		break;
	case 2:
		render2(1220, 500);
		break;
	case 3:
		render3(1220, 500);
		break;
	case 4:
		render4(1220, 500);
		break;
	case 5:
		render5(1220, 500);
		break;
	case 6:
		render6(1220, 500);
		break;
	case 7:
		render7(1220, 500);
		break;
	case 8:
		render8(1220, 500);
		break;
	case 9:
		render9(1220, 500);
		break;
	}
	switch (((int)frames % 1000 - (int)frames % 100) / 100) {
	case 0:
		render0(1190, 500);
		break;
	case 1:
		render1(1190, 500);
		break;
	case 2:
		render2(1190, 500);
		break;
	case 3:
		render3(1190, 500);
		break;
	case 4:
		render4(1190, 500);
		break;
	case 5:
		render5(1190, 500);
		break;
	case 6:
		render6(1190, 500);
		break;
	case 7:
		render7(1190, 500);
		break;
	case 8:
		render8(1190, 500);
		break;
	case 9:
		render9(1190, 500);
		break;
	}
}

void renderShieldTime(float score) {
	switch ((int)score % 10) {
	case 0:
		render0(30, 670);
		break;
	case 1:
		render1(30, 670);
		break;
	case 2:
		render2(30, 670);
		break;
	case 3:
		render3(30, 670);
		break;
	case 4:
		render4(30, 670);
		break;
	case 5:
		render5(30, 670);
		break;
	case 6:
		render6(30, 670);
		break;
	case 7:
		render7(30, 670);
		break;
	case 8:
		render8(30, 670);
		break;
	case 9:
		render9(30, 670);
		break;
	}
	switch (((int)score % 100 - (int)score % 10) / 10) {
	case 0:
		render0(0, 670);
		break;
	case 1:
		render1(0, 670);
		break;
	case 2:
		render2(0, 670);
		break;
	case 3:
		render3(0, 670);
		break;
	case 4:
		render4(0, 670);
		break;
	case 5:
		render5(0, 670);
		break;
	case 6:
		render6(0, 670);
		break;
	case 7:
		render7(0, 670);
		break;
	case 8:
		render8(0, 670);
		break;
	case 9:
		render9(0, 670);
		break;
	}

}

void renderBackground() {
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(1280, 0);
	glVertex2f(1280, 720);
	glVertex2f(0, 720);
	glEnd();
	glColor3f(1, 1, 1);
}

void renderButton(float x, float y) {
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(x, y);
	glTexCoord2f(1, 1);
	glVertex2f(x + 300, y);
	glTexCoord2f(1, 0);
	glVertex2f(x + 300, y + 75);
	glTexCoord2f(0, 0);
	glVertex2f(x, y + 75);
	glEnd();
}

void renderButtonGameOver(float x, float y) {
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(x, y);
	glTexCoord2f(1, 1);
	glVertex2f(x + 370, y);
	glTexCoord2f(1, 0);
	glVertex2f(x + 370, y + 100);
	glTexCoord2f(0, 0);
	glVertex2f(x, y + 100);
	glEnd();
}

void renderValue(float x, float y) {
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(x, y);
	glTexCoord2f(1, 1);
	glVertex2f(x + 200, y);
	glTexCoord2f(1, 0);
	glVertex2f(x + 200, y + 75);
	glTexCoord2f(0, 0);
	glVertex2f(x, y + 75);
	glEnd();
}