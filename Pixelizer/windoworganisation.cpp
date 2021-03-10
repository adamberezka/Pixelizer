#include "windoworganisation.h"


void drawWindow(HWND hWnd) {

    CreateWindowW(L"Button", L"Choose colour", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 10, 120, 110, hWnd, (HMENU)GROUP, nullptr, nullptr);
    CreateWindowW(L"Button", L"Choose colour1", WS_CHILD | WS_VISIBLE, 10, 30, 120, 110, hWnd, (HMENU)1, nullptr, nullptr);
}