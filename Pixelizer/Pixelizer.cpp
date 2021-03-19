// Pixelizer.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "Pixelizer.h"
#include "bmpread.h"
#include "windoworganisation.h"
#include "pixel.h"

#define MAX_LOADSTRING 100

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: W tym miejscu umieść kod.

    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PIXELIZER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PIXELIZER));

    MSG msg;

    // Główna pętla komunikatów:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PIXELIZER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PIXELIZER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, 1280, 720, nullptr, nullptr, hInstance, nullptr);

   ::SetMenu(hWnd, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

BYTE* pixels = nullptr;
BYTE* output = nullptr;
BYTE* header = nullptr;

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static RECT bmpWindowField;
    static RECT bmpSourceField;
    static float zoom = 0;
	static HBITMAP hBMP;
	static POINT displace = { 0, 0 };
	static POINT vecStart = { 0, 0 };
	static POINT prevDisplace = { 0, 0 };
	static bool click = false;

    static BITMAP bmp;

    switch (message)
    {
    case WM_CREATE:
    {
        drawWindow(hWnd);
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizuj zaznaczenia menu:
            switch (wmId)
            {
			case READFILE:
				hBMP = (HBITMAP)LoadImage(NULL, L"testH.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				//hBMP = (HBITMAP)LoadImage(NULL, L"testV.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				GetObject(hBMP, sizeof(BITMAP), &bmp);
				initRects(bmpWindowField, bmp.bmWidth, bmp.bmHeight);
				drawImage(hWnd, hBMP, bmpWindowField, displace);
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_LBUTTONDOWN:
		click = true;
		GetCursorPos(&vecStart);
		RECT rect;
		GetWindowRect(hWnd, &rect);
		vecStart = { vecStart.x - rect.left , vecStart.y - rect.top };
		break;
	case WM_LBUTTONUP:
		click = false;
		prevDisplace = displace;
		break;
	case WM_MOUSEMOVE:
		if (click == true) {
			GetCursorPos(&displace);
			RECT rect;
			GetWindowRect(hWnd, &rect);
			displace = { prevDisplace.x-(displace.x - vecStart.x - rect.left) ,prevDisplace.y -(displace.y - vecStart.y - rect.top)  };
			drawImage(hWnd, hBMP, bmpWindowField, displace);
		}
		break;
    case WM_MOUSEWHEEL:
        {
            zoom = GET_WHEEL_DELTA_WPARAM(wParam) / 120;

            setRects(bmpWindowField, bmpSourceField, zoom, bmp.bmWidth, bmp.bmHeight);
            drawImage(hWnd, hBMP, bmpWindowField, displace);
        }
        break;
    case WM_ERASEBKGND:
        return (LRESULT)1;
        break;
    case WM_PAINT:
        {
        
        setRects(bmpWindowField, bmpSourceField, zoom, bmp.bmWidth, bmp.bmHeight);
        drawImage(hWnd, hBMP, bmpWindowField, displace);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Procedura obsługi komunikatów dla okna informacji o programie.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
