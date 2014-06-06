/* FILENAME: DEF.H
 * PROGRAMMER: MY6
 * PURPOSE: Globe Application
 * LAST UPDATE: 06.06.2014
 */

#include <stdlib.h>
#include <math.h>
#include "def.h"

#define PI 3.14159265358979323846
#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

VOID DrawGlobe( HDC hDC, INT Xc, INT Yc, DOUBLE t)
{
  DOUBLE Theta, Phi;
  INT x, y, z;
  DOUBLE r = 400 ;

  for (Phi = 0; Phi < 2 * PI; Phi += 0.07) 
    for (Theta = 0; Theta <  PI; Theta += 0.07)
    {
      x = Xc + r * sin(Theta) * sin (Phi + t / 200);
      y = Yc + r * cos(Theta);
      z = r * sin(Theta) * cos(Phi + t / 200);
      if (z > 0)
        SetPixel(hDC, x, y, RGB(255, 0, 255));
    }
}
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = WindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error while registering window class", "Error", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "Clocks", WS_OVERLAPPEDWINDOW, 1920,0,  1280, 1024, NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}
LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  HDC hDC, hMemDC;
  PAINTSTRUCT ps;
  SYSTEMTIME st;
  RECT rc;
  static INT W = 0, H = 0;
  BITMAP bm;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    hDC = GetDC(hWnd);
    if (hBm != NULL)
      DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    return 0;
  case WM_CHAR:
    if ((CHAR)wParam == 27)
        DestroyWindow(hWnd);
    return 0;
  case WM_TIMER:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBm);
   
    GetLocalTime(&st);
    
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDC, RGB(0, 0, 0));
    SetDCBrushColor(hMemDC, RGB(0, 0, 0));

    Rectangle(hMemDC, 0, 0, W, H);

    DrawGlobe(hMemDC, W / 2, H / 2, st.wMilliseconds / 10);

    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    DeleteDC(hMemDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg ,wParam, lParam);
}
