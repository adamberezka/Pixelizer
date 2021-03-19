#include "pixel.h"

void drawImage(HWND hWnd, HBITMAP hBMP, RECT bmpWindowField, POINT displacement) {

    PAINTSTRUCT     ps;
    HDC             hdc;
    BITMAP          bitmap;
    HDC             hdcMem;
    HGDIOBJ         oldBitmap;
    HBITMAP         hBitmapBuffer;

    RECT area = {minX, minY, maxX, maxY};

    InvalidateRect(hWnd, &area, TRUE);

    hdc = BeginPaint(hWnd, &ps);

    hdcMem = CreateCompatibleDC(hdc);
    HDC hdcBuffer = CreateCompatibleDC(hdc);

    hBitmapBuffer = CreateCompatibleBitmap(hdc, maxX - minX, maxY - minY);

    oldBitmap = SelectObject(hdcMem, hBMP);

    SelectObject(hdcBuffer, hBitmapBuffer);

    RECT fill = { 0 , 0, width, height };
    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdcBuffer, &fill, brush);
    DeleteObject(brush);

    GetObject(hBMP, sizeof(bitmap), &bitmap);

    SetStretchBltMode(hdcBuffer, HALFTONE);
    StretchBlt(hdcBuffer, bmpWindowField.left - displacement.x, bmpWindowField.top - displacement.y, bmpWindowField.right - bmpWindowField.left , bmpWindowField.bottom - bmpWindowField.top,
        hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
    
    BitBlt(hdc, minX, minY, maxX - minX, maxY - minY, hdcBuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcBuffer);
    DeleteDC(hdcMem);

    EndPaint(hWnd, &ps);
}

void initRects(RECT& bmpWindowField, int bmWidth, int bmHeight) {
    if ((float)bmWidth / width > (float)bmHeight / height) {
        //match width
        bmpWindowField.left = 0;
        bmpWindowField.right = width;
        float ratio = (float)width / (float)bmWidth;
        int newHeight = ratio * bmHeight;
        float dif = (float)(height - newHeight) / 2;
        bmpWindowField.top = 0 + dif;
        bmpWindowField.bottom = 0 + dif + newHeight;
    } else {
        // match height
        bmpWindowField.top = 0;
        bmpWindowField.bottom = height;
        float ratio = (float)height / (float)bmHeight;
        int newWidth = ratio * bmWidth;
        float dif = (float)(width - newWidth) / 2;
        bmpWindowField.left = 0 + dif;
        bmpWindowField.right = 0 + dif + newWidth;
    }
}

void setRects(RECT& bmpWindowField, RECT& bmpSourceField, float zoom, int bmWidth, int bmHeight) {
    if ((float)bmWidth / width > (float)bmHeight /  height) {
		float ratio = (float)width / (float)bmWidth;
		int newHeight = ratio * bmHeight;
		bmpWindowField.left -= (zoom * 0.05 * width);
		bmpWindowField.right += (zoom * 0.05 * width);
		bmpWindowField.top -= (zoom * 0.05 * newHeight);
		bmpWindowField.bottom += (zoom * 0.05 * newHeight);
    } else {
		float ratio = (float)height / (float)bmHeight;
		int newWidth = ratio * bmWidth;
		bmpWindowField.left -= (zoom * 0.05 * newWidth);
		bmpWindowField.right += (zoom * 0.05 * newWidth);
		bmpWindowField.top -= (zoom * 0.05 * height);
		bmpWindowField.bottom += (zoom * 0.05 * height);
    }
}