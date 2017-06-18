#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>

#define CELLSIZE 2

#ifndef TRUE
#define TRUE 1
#endif

#ifndef  FALSE
#define FALSE 0
#endif

typedef struct {
	char ch;
	char style;
} cell_t;


typedef cell_t video_row[80];

void blinkCursor();
void writeAtCursor(char ch, char style);
void writeScreen(uint8_t x, uint8_t y, char ch, char style);
void updateCursor(uint8_t x, uint8_t y);
void updateMouse(uint8_t x, uint8_t y);
void selectTo(uint8_t x, uint8_t y);
void toggleCursors();

#endif
