#pragma once

#include "framework.h"
#include "Resource.h"
#include <vector>

#define IMAGE_SIZE		100

typedef struct _image
{
	int x, y;
	_image() : x(0), y(0) {}
	_image(int _x, int _y) : x(_x), y(_y) {}
} Image;

class Game
{
private:
	TCHAR sTime[10] = _T("");
	int minute = 0;
	int second = 0;
	HFONT hFont;
	HDC memdc;
	PAINTSTRUCT ps;
	HBITMAP hBitmap;
	HINSTANCE hInst;

	bool backgroundDrawn = false;

	Image board[5][5]; 
	std::vector<Image> images;

public:
	Game(HINSTANCE inst) : hInst(inst)
	{
		hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("±Ã¼­"));
		hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	}

	void InitBoard(HDC hd, HWND hWnd);

	void DrawBackground(HDC hdc, HWND hWnd);
	void DrawTimer(HDC hdc, HWND hWnd);
	void DrawBoard(HDC hdc, HWND hWnd);

	void UpdateTimer();

};

