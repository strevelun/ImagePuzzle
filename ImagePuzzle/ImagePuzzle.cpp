#include "framework.h"
#include "ImagePuzzle.h"
#include "Game.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];            

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

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_IMAGEPUZZLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IMAGEPUZZLE));

    MSG msg;

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IMAGEPUZZLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_IMAGEPUZZLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Game game(hInst);
    PAINTSTRUCT ps;
    HDC hdc;
    static int x, y;
    static POINT mousePos;

    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 1000, NULL);
        SendMessage(hWnd, WM_TIMER, 1, 0);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        game.DrawBackground(hdc, hWnd);
        game.DrawTimer(hdc, hWnd);
        game.DrawBoard(hdc, hWnd);
        /*
        wchar_t str[100];
        swprintf_s(str, L"                          ");
        TextOut(hdc, 1000, 10, str, _tcslen(str));
        swprintf_s(str, L"%d, %d", x, y);
        TextOut(hdc, 1000, 10, str, _tcslen(str));

        wchar_t str1[100];
        swprintf_s(str1, L"                          ");
        TextOut(hdc, 1000, 70, str1, _tcslen(str1));
        swprintf_s(str1, L"%d, %d", mousePos.x, mousePos.y);
        TextOut(hdc, 1000, 70, str1, _tcslen(str1));
        */

        EndPaint(hWnd, &ps);
        break;

    case WM_LBUTTONUP:
        // 클릭한 자리 상하좌우에 클릭한 자리 좌표 저장. 상하좌우에서 그 자리로 이동할 수 있다는 뜻
        // available
        // 클릭하면 그 위치 상하좌우에 그 위치를 저장. 이전에 저장했던 상하좌우 좌표는 그대로 두고 available만 false
        GetCursorPos(&mousePos);
        ScreenToClient(hWnd, &mousePos);

        game.OnClick(mousePos.x, mousePos.y);
        //game.CheckVictory();
        InvalidateRgn(hWnd, NULL, FALSE);
        break;

    case WM_KEYDOWN:
        game.CheckVictory();
        break;

    case WM_TIMER:
        game.UpdateTimer();
        InvalidateRgn(hWnd, NULL, FALSE);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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