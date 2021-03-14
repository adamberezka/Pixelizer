#include "windoworganisation.h"


void drawWindow(HWND hWnd) {

    RECT rect;
    GetWindowRect(hWnd, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;


    CreateWindowW(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 0, 280, 670, hWnd, (HMENU)GROUP, nullptr, nullptr);
    CreateWindowW(L"Button", L"Wczytaj plik", WS_CHILD | WS_VISIBLE, 20, 15, 260, 25, hWnd, (HMENU)READFILE, nullptr, nullptr);
    CreateWindowW(L"Button", L"Zapisz plik", WS_CHILD | WS_VISIBLE, 20, 635, 260, 25, hWnd, (HMENU)SAVEFILE, nullptr, nullptr);
}