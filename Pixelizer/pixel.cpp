#include "pixel.h"

void drawImage(HWND hWnd, HBITMAP hBMP, RECT bmpWindowField, RECT bmpSourceField) {

    PAINTSTRUCT     ps;
    HDC             hdc;
    BITMAP          bitmap;
    HDC             hdcMem;
    HGDIOBJ         oldBitmap;

    hdc = BeginPaint(hWnd, &ps);

    hdcMem = CreateCompatibleDC(hdc);
    oldBitmap = SelectObject(hdcMem, hBMP);

    GetObject(hBMP, sizeof(bitmap), &bitmap);
    SetStretchBltMode(hdc, HALFTONE);
    StretchBlt(hdc, bmpWindowField.left, bmpWindowField.top, bmpWindowField.right, bmpWindowField.bottom,
        hdcMem, bmpSourceField.left, bmpSourceField.top, bmpSourceField.right, bmpSourceField.bottom, SRCCOPY);

    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);

    EndPaint(hWnd, &ps);
}

void setRects(RECT& bmpWindowField, RECT& bmpSourceField, float zoom, int bmWidth, int bmHeight) {

    const int minX = 300;
    const int maxX = 955;
    const int minY = 8;
    const int maxY = 661;

    const int width = maxX;
    const int height = maxY;

    if ((float)bmWidth / width > (float)bmHeight /  height) {
        //dopsauj do X
        bmpWindowField.left = minX;
        bmpWindowField.right = maxX;
        float ratio = (float)width / (float)bmWidth;
        int newHeight = ratio * bmHeight;
        int dif = (height - newHeight);
        bmpWindowField.top = minY + ((float)dif / 2);
        bmpWindowField.bottom = newHeight;

    } else {
        // dopasuj do Y
        bmpWindowField.top = minY;
        bmpWindowField.bottom = maxY;
        float ratio = (float)height / (float)bmHeight;
        int newWidth = ratio * bmWidth;
        int dif = (width - newWidth);
        bmpWindowField.left = minX + ((float)dif / 2);
        bmpWindowField.right = newWidth;
    }

    bmpSourceField.left = 0 + bmWidth * zoom * 0.1;
    bmpSourceField.top = 0 + bmHeight * zoom * 0.1;
    bmpSourceField.right = bmWidth - bmWidth * zoom * 0.1;
    bmpSourceField.bottom = bmHeight - bmHeight * zoom * 0.1;
}