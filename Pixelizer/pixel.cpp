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
        bmpWindowField.top = dif;
        bmpWindowField.bottom = dif + newHeight;
    } else {
        // match height
        bmpWindowField.top = 0;
        bmpWindowField.bottom = height;
        float ratio = (float)height / (float)bmHeight;
        int newWidth = ratio * bmWidth;
        float dif = (float)(width - newWidth) / 2;
        bmpWindowField.left = dif;
        bmpWindowField.right = dif + newWidth;
    }
}

void setRects(RECT& bmpWindowField, RECT& bmpSourceField, float zoom, int bmWidth, int bmHeight) {
    if ((float)bmWidth / width > (float)bmHeight /  height) {
		float ratio = (float)width / (float)bmWidth;
		int newHeight = ratio * bmHeight;
		float dif = (float)(height - newHeight) / 2;
		bmpWindowField.left = -float(zoom * width * 0.05);
		bmpWindowField.right = width + float(zoom * width * 0.05);
		bmpWindowField.top = dif-float(zoom * newHeight * 0.05);
		bmpWindowField.bottom = newHeight + dif + float(zoom * newHeight * 0.05);
    } else {
		float ratio = (float)height / (float)bmHeight;
		int newWidth = ratio * bmWidth;
		float dif = (float)(width - newWidth) / 2;
		bmpWindowField.left = dif-float(zoom * newWidth * 0.05);
		bmpWindowField.right = newWidth + dif + float(zoom * newWidth * 0.05);
		bmpWindowField.top = -float(zoom * height * 0.05);
		bmpWindowField.bottom = height + float(zoom * height * 0.05);
    }
}

PWSTR openFile() {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	PWSTR pszFilePath = { 0 };

	COMDLG_FILTERSPEC fileTypes[] =
	{
		{ L"Bitmaps", L"*.bmp" },
	};

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		hr = pFileOpen->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
		if (SUCCEEDED(hr))
		{
			// Set the selected file type index to Word Docs for this example.
			hr = pFileOpen->SetFileTypeIndex(1);
			if (SUCCEEDED(hr))
			{
				// Set the default extension to be ".bmp" file.
				hr = pFileOpen->SetDefaultExtension(L"bmp");
				if (SUCCEEDED(hr))
				{
					if (SUCCEEDED(hr))
					{
						// Show the Open dialog box.
						hr = pFileOpen->Show(NULL);

						// Get the file name from the dialog box.
						if (SUCCEEDED(hr))
						{
							IShellItem* pItem;
							hr = pFileOpen->GetResult(&pItem);
							if (SUCCEEDED(hr))
							{
								hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

								// Display the file name to the user.
								if (SUCCEEDED(hr))
								{
									CoUninitialize();
									return pszFilePath;
								}
								pItem->Release();
							}
						}
						pFileOpen->Release();
					}
					
				}
			}
		}
	}
	CoUninitialize();
	return NULL;
}

PWSTR saveFile() {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	PWSTR pszFilePath = { 0 };

	COMDLG_FILTERSPEC fileTypes[] =
	{
		{ L"Bitmaps", L"*.bmp" },
	};

	if (SUCCEEDED(hr))
	{
		IFileSaveDialog* pFileSave;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

		hr = pFileSave->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
		if (SUCCEEDED(hr))
		{
			// Set the selected file type index to Word Docs for this example.
			hr = pFileSave->SetFileTypeIndex(1);
			if (SUCCEEDED(hr))
			{
				// Set the default extension to be ".bmp" file.
				hr = pFileSave->SetDefaultExtension(L"bmp");
				if (SUCCEEDED(hr))
				{
					if (SUCCEEDED(hr))
					{
						// Show the Open dialog box.
						hr = pFileSave->Show(NULL);

						// Get the file name from the dialog box.
						if (SUCCEEDED(hr))
						{
							IShellItem* pItem;
							hr = pFileSave->GetResult(&pItem);
							if (SUCCEEDED(hr))
							{
								hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

								// Display the file name to the user.
								if (SUCCEEDED(hr))
								{
									MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
									CoTaskMemFree(pszFilePath);
								}
								pItem->Release();
							}
						}
						pFileSave->Release();
					}
				}
			}
		}
	}
	CoUninitialize();
	return NULL;
}