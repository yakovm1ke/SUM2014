/* MY6, 03.06.2014 */

#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;

    /* сохраняем старый размер окна */
    GetWindowRect(hWnd, &SaveRC);

    /* переходим в полный экран */
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);

    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'FlipFullScreen' function */

VOID DrawArrow( HDC hDC, INT Xc, INT Yc, INT L, INT W, FLOAT Angle )
{
  INT i;
  POINT pts[] =
  {
    {0, -W}, {-W, 0}, {0, L}, {W, 0}
  }, pts_draw[sizeof pts / sizeof pts[0]];
  FLOAT si = sin(Angle), co = cos(Angle);

  for (i = 0; i < sizeof pts / sizeof pts[0]; i++)
  {
    pts_draw[i].x = Xc + (pts[i].x * co - pts[i].y * si);
    pts_draw[i].y = Yc - (pts[i].x * si + pts[i].y * co);
  }
  //SelectObject(hDC, GetStockObject(DC_BRUSH));
  //SelectObject(hDC, GetStockObject(DC_PEN));
  //SetDCPenColor(hDC, RGB(0, 0, 0));
  //SetDCBrushColor(hDC, RGB(255, 255, 255));
  Polygon(hDC, pts_draw, sizeof pts / sizeof pts[0]);
} /* End of 'DrawArrow' function */

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
  HDC hDC, hMemDC, hMemDCLogo;
  PAINTSTRUCT ps;
  SYSTEMTIME st;
  FLOAT PI = 3.14159265358979323846;
  BITMAP bm;
  static INT W = 0, H = 0;
  static HBITMAP hBm, hBmLogo;
  INT r = 5;
  INT i = 0;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    hBmLogo = LoadImage(NULL, "cf.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return 0;
  case WM_CHAR:
    if ((CHAR)wParam == 27)
        DestroyWindow(hWnd);
    if ((CHAR)wParam == 'f' || 'F')
        FlipFullScreen( hWnd );
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    hDC = GetDC(hWnd);
    if (hBm != NULL)
      DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBm);
    SelectObject(hMemDCLogo, hBmLogo);
   
    GetLocalTime(&st);
    GetObject(hBmLogo, sizeof(bm), &bm);
    
    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCCOPY);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SelectObject(hDC, GetStockObject(DC_PEN));
    SetDCPenColor(hDC, RGB(0, 0, 0));
    SetDCBrushColor(hDC, RGB(0, 0, 255));
    DrawArrow(hDC, 300, 300 , 100, 50, (-(st.wHour % 12 + st.wMinute / 60.0) / 12.0) * 2 * PI);
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(0, 255, 0));
    DrawArrow(hDC, 300, 300, 140, 45, (-(st.wMinute + st.wSecond / 60.0) / 60.0) * 2 * PI);
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(255, 0, 0));
    DrawArrow(hDC, 300, 300, 200, 30, (-(st.wSecond + st.wMilliseconds / 1000.0) / 60.0) * 2 * PI);
    //DrawArrow(hDC, 300, 300, 250, 20, (-st.wMilliseconds / 1000.0) * 2 * PI);
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(0, 0, 0));
    Ellipse(hDC, 300 - r, 300 - r, 300 + r, 300 + r);
    
    DeleteDC(hMemDCLogo);
    DeleteDC(hMemDC);
    DeleteDC(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, 0);
    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteObject(hBmLogo);
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg ,wParam, lParam);
}
