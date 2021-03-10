#pragma once

#include <Windows.h>
#include<fstream>
#include <string>

void addControls(HWND hWnd);

bool readFile(BYTE*& input, BYTE*& output, LPWSTR fileName, BYTE*& headerPtr);

bool readFile(BYTE*& input, BYTE*& output, std::string fileName, BYTE*& headerPtr);

void saveFile(BYTE*& output, LPWSTR fileName, BYTE*& headerPtr);