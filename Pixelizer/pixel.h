#include <Windows.h>

void drawImage(HWND hWnd, HBITMAP hBMP, RECT bmpWindowField, RECT bmpSourceField);

void setRects(RECT& bmpWindowField, RECT& bmpSourceField, float zoom, int bmWidth, int bmHeight);