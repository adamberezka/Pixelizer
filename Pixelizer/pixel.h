#include <Windows.h>

const int minX = 300;
const int maxX = 1255;
const int minY = 8;
const int maxY = 670;

const int width = maxX - minX;
const int height = maxY - minY;

void drawImage(HWND hWnd, HBITMAP hBMP, RECT bmpWindowField, RECT bmpSourceField);

void setRects(RECT& bmpWindowField, RECT& bmpSourceField, float zoom, int bmWidth, int bmHeight);

void initRects(RECT& bmpWindowField, RECT& bmpSourceField, int bmWidth, int bmHeight);