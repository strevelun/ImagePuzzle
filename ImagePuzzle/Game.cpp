#include "Game.h"
#include <random>

void Game::InitBoard(HDC hdc, HWND hWnd)
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    int omit = st.wSecond % (BOARD_SIZE * BOARD_SIZE);
    int k = 0;
    boardState[omit / BOARD_SIZE][omit % BOARD_SIZE] = 1; // ����ִ� ��

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            int curX = j * IMAGE_SIZE;
            int curY = i * IMAGE_SIZE;
            images.push_back(Image(curX, curY));

            board[i][j].x = (curX * 1.6) + 600;
            board[i][j].y = (curY * 1.6) + 100;

            if (boardState[i][j] != 1)
                continue;

            xIdx = j;
            yIdx = i;

            // �����¿��� �̹����� available = true
            ChangeState();

            //BitBlt(board[i][j].dc, curX, curY, IMAGE_SIZE, IMAGE_SIZE, memdc, curX, curY, SRCCOPY);
        }
    }

    //RandomizeBoard(hdc);
    RandomizeBoardTemp(hdc);

}

void Game::DrawBackground(HDC hdc, HWND hWnd)
{
    if (backgroundDrawn)
        return;


    // �ڽ� �׵θ� �׸���
    HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 127, 0));
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    Rectangle(hdc, 7, 7, 512, 512);
    Rectangle(hdc, 600, 100, 1400, 900);
    SelectObject(hdc, oldPen);

    // �̸����� ���� �׸���
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
    if (!updateBoard)
        return;
    updateBoard = false;

    // Ŭ���� ��ġ�� ��� �簢������ ����, ������ 1�̾��� ���� ���� Ŭ���� ��ġ �̹��� ����
    Rectangle(hdc, board[yIdx][xIdx].x, board[yIdx][xIdx].y, board[yIdx][xIdx].x + IMAGE_SIZE * 1.6, board[yIdx][xIdx].y + IMAGE_SIZE * 1.6);

    memdc = CreateCompatibleDC(hdc);
    SelectObject(memdc, hBitmap);
    int availX = board[yIdx][xIdx].availPos.x;
    int availY = board[yIdx][xIdx].availPos.y;
    StretchBlt(hdc, board[availY][availX].x, board[availY][availX].y, IMAGE_SIZE * 1.6, IMAGE_SIZE * 1.6, memdc, mixedBoard[yIdx][xIdx].x, mixedBoard[yIdx][xIdx].y, IMAGE_SIZE, IMAGE_SIZE, SRCCOPY); // 1.6�� ũ��
    Image temp = mixedBoard[availY][availX];
    mixedBoard[availY][availX] = mixedBoard[yIdx][xIdx];
    mixedBoard[yIdx][xIdx] = temp;

    DeleteDC(memdc);

}

void Game::UpdateTimer(HWND hWnd)
{
    if (victory) 
    {
        KillTimer(hWnd, 1);
        return;
    }

    minute = second == 59 ? minute + 1 : minute;
    second = (second + 1) % 60;
    _stprintf_s(sTime, _T("%d:%d"), minute, second);
    InvalidateRgn(hWnd, NULL, FALSE);
}

void Game::RandomizeBoard(HDC hdc)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    int random;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            int size = images.size();

            if (size <= 0)
                break;

            std::uniform_int_distribution<int> d1(0, size - 1);
            random = d1(gen);


            Image image;
            image = images[random];
            images.erase(images.begin() + random);
            mixedBoard[i][j].x = image.x;
            mixedBoard[i][j].y = image.y;

            if (boardState[i][j] == 1)
            {
                StretchBlt(hdc, 1500, 100, IMAGE_SIZE * 1.6, IMAGE_SIZE * 1.6, memdc, mixedBoard[i][j].x, mixedBoard[i][j].y, IMAGE_SIZE, IMAGE_SIZE, SRCCOPY);
                continue;
            }

            StretchBlt(hdc, board[i][j].x, board[i][j].y, IMAGE_SIZE * 1.6, IMAGE_SIZE * 1.6, memdc, image.x, image.y, IMAGE_SIZE, IMAGE_SIZE, SRCCOPY); // 1.6�� ũ��

        }
    }
}

void Game::RandomizeBoardTemp(HDC hdc)
{
    int blanki, blankj;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (boardState[i][j] == 1)
            {
                blanki = i;
                blankj = j;
                //continue;
            }

            mixedBoard[i][j].x = (board[i][j].x - 600) / 1.6;
            mixedBoard[i][j].y = (board[i][j].y - 100) / 1.6;
        }
    }

    // �� ���� �κ��� ��ǥ���� �����¿� Ȯ���� ���� ���� ó�� ��ü ������ ���� ��ü

    boardState[blanki][blankj] = 0;
    ChangeState(false);

    if (blanki - 1 >= 0)
    {
        yIdx = blanki - 1;
    }
    else if (blanki + 1 < BOARD_SIZE)
    {
        yIdx = blanki + 1;
    }
    else if (blankj - 1 >= 0)
    {
        xIdx = blankj - 1;
    }
    else if (blankj + 1 < BOARD_SIZE)
    {
        xIdx = blankj + 1;
    }

    ChangeState();
    boardState[yIdx][xIdx] = 1;
    //updateBoard = true;

    Image temp = mixedBoard[blanki][blankj];
    mixedBoard[blanki][blankj] = mixedBoard[yIdx][xIdx];
    mixedBoard[yIdx][xIdx] = temp;

    StretchBlt(hdc, 1500, 100, IMAGE_SIZE * 1.6, IMAGE_SIZE * 1.6, memdc, mixedBoard[yIdx][xIdx].x, mixedBoard[yIdx][xIdx].y, IMAGE_SIZE, IMAGE_SIZE, SRCCOPY);

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (boardState[i][j] == 1)
            {
               // StretchBlt(hdc, 1500, 100, IMAGE_SIZE * 1.6, IMAGE_SIZE * 1.6, memdc, mixedBoard[i][j].x, mixedBoard[i][j].y, IMAGE_SIZE, IMAGE_SIZE, SRCCOPY);
                continue;
            }
            StretchBlt(hdc, board[i][j].x, board[i][j].y, IMAGE_SIZE * 1.6, IMAGE_SIZE * 1.6, memdc, mixedBoard[i][j].x, mixedBoard[i][j].y, IMAGE_SIZE, IMAGE_SIZE, SRCCOPY);
        }
    }
}

void Game::OnClick(int x, int y)
{
    // ���� Ŭ���� ��ġ
    int curX = (x - 600) / 160;
    int curY = (y - 100) / 160;

    if (board[curY][curX].available == true)
    {
        // ���� ��ǥ �����¿� false

        boardState[yIdx][xIdx] = 0;
        ChangeState(false);
        xIdx = curX; // IMAGE_SIZE * 1.6
        yIdx = curY;
        ChangeState();
        //MessageBox(NULL, L"fdas", L"fdsa", MB_OK);
        // ������ ��ġ�� �̹����� availPos�� ��ġ�� �ű��.
        boardState[yIdx][xIdx] = 1;
        updateBoard = true;
    }
}

void Game::ChangeState(bool b)
{
    if (yIdx - 1 >= 0)
    {
        board[yIdx - 1][xIdx].available = b;
        board[yIdx - 1][xIdx].availPos = Pos(xIdx, yIdx);
    }
    if (yIdx + 1 < BOARD_SIZE)
    {
        board[yIdx + 1][xIdx].available = b;
        board[yIdx + 1][xIdx].availPos = Pos(xIdx, yIdx);
    }
    if (xIdx - 1 >= 0)
    {
        board[yIdx][xIdx - 1].available = b;
        board[yIdx][xIdx - 1].availPos = Pos(xIdx, yIdx);
    }
    if (xIdx + 1 < BOARD_SIZE)
    {
        board[yIdx][xIdx + 1].available = b;
        board[yIdx][xIdx + 1].availPos = Pos(xIdx, yIdx);
    }
}

void Game::CheckVictory()
{
    if (xIdx < 0 || xIdx >= BOARD_SIZE || yIdx < 0 || yIdx >= BOARD_SIZE)
        return;

    // �� ������ ������ ������ ��. �ϳ��� �� ���� ��� return


    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] != mixedBoard[i][j] && boardState[i][j] != 1)
                return;
        }
    }

    victory = true;
    MessageBox(NULL, L"����", L"���", MB_OK);
}
