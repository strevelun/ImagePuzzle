#pragma once

#include "framework.h"
#include "Resource.h"
#include <vector>

#define IMAGE_SIZE		100
#define BOARD_SIZE		5

typedef struct _pos
{
	int x, y;
	_pos() : x(0), y(0) {}
	_pos(int _x, int _y) : x(_x), y(_y) {}
} Pos;

typedef struct _image
{
	Pos availPos;
	bool available = false;
	int x, y;
	_image() : x(0), y(0) {}
	_image(int _x, int _y) : x(_x), y(_y) {}

	bool operator==(const _image& image) { return (((x - 600) / 1.6) == image.x) && (((y - 100) / 1.6) == image.y); }
	bool operator!=(const _image& image) { return (((x - 600) / 1.6) != image.x) || (((y - 100) / 1.6) != image.y); }
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
	bool updateBoard = false;

	Image board[BOARD_SIZE][BOARD_SIZE], mixedBoard[BOARD_SIZE][BOARD_SIZE];
	int boardState[BOARD_SIZE][BOARD_SIZE] = {0};
	std::vector<Image> images;
	int xIdx, yIdx;
	bool victory = false;

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

	void UpdateTimer(HWND hWnd);

	void RandomizeBoard(HDC hdc);
	void RandomizeBoardTemp(HDC hdc);

	void OnClick(int x, int y);
	void ChangeState(bool b = true);
	void CheckVictory();
};

