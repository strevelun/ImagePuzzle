#include "Game.h"
#include <random>

void Game::InitBoard(HDC hdc, HWND hWnd)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int curX = j * IMAGE_SIZE;
            int curY = i * IMAGE_SIZE;
            board[i][j].x = curX;
            board[i][j].y = curY;

            images.push_back(Image(curX, curY));

            //BitBlt(board[i][j].dc, curX, curY, IMAGE_SIZE, IMAGE_SIZE, memdc, curX, curY, SRCCOPY);
        }
    }
}

void Game::DrawBackground(HDC hdc, HWND hWnd)
{
    if (backgroundDrawn)
        return;


    // 박스 테두리 그리기
    HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 127, 0));
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    Rectangle(hdc, 7, 7, 512, 512);
    Rectangle(hdc, 600, 100, 1400, 900);
    SelectObject(hdc, oldPen);

    // 미리보기 사진 그리기
    memdc = CreateCompatibleDC(hdc);
    SelectObject(memdc, hBitmap);
    
    BitBlt(hdc, 10, 10, 800, 800, memdc, 0, 0, SRCCOPY);
    
    backgroundDrawn = true;

    InitBoard(hdc, hWnd);
    DrawBoard(hdc, hWnd);

    DeleteDC(memdc);
}

void Game::DrawTimer(HDC hdc, HWND hWnd)
{
    SelectObject(hdc, hFont);
    TextOut(hdc, 100, 600, L"          ", _tcslen(L"          "));
    TextOut(hdc, 100, 600, sTime, _tcslen(sTime));
}

void Game::DrawBoard(HDC hdc, HWND hWnd)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    int random;
    SYSTEMTIME st;
    GetLocalTime(&st);
    int omit = st.wSecond % 25;
    int k = 0;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (images.size() <= 1)
                break;

            std::uniform_int_distribution<int> d1(0, images.size() - 1);
            random = d1(gen);
      
            if (k++ == omit)
                continue;
            
            Image image;
            image = images[random];
            images.erase(images.begin() + random);

            StretchBlt(hdc, (board[i][j].x * 1.6) + 600, (board[i][j].y * 1.6) + 100, IMAGE_SIZE * 1.6, IMAGE_SIZE * 1.6, memdc, image.x, image.y, IMAGE_SIZE, IMAGE_SIZE, SRCCOPY);


        }
    }
}

void Game::UpdateTimer()
{
    minute = second == 59 ? minute + 1 : minute;
    second = (second+1) % 60;
    _stprintf_s(sTime, _T("%d:%d"), minute, second);
}
