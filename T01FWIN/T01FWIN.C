/* MY6, 02.06.2014 */

#include <math.h>

#include <windows.h>


#define WND_CLASS_NAME "My window class"
#define r0 ((float)rand() / RAND_MAX)

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

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wPAram, LPARAM lParam );
/*Xc, Yc - center of eye, XX, YY - the position of mouse */
void DrawEye( HDC hDC, FLOAT Xc, FLOAT Yc, FLOAT XX, FLOAT YY, FLOAT W, FLOAT H )
{
  FLOAT r = 20, l, co, si, xe, ye;

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  Ellipse( hDC, Xc - W / 2, Yc - H / 2, Xc + W / 2, Yc + H / 2 );
  l = sqrt((XX - Xc) * (XX - Xc) + (YY - Yc) * (YY - Yc));
  if (l == 0)
  {
    co = 0;
    si = 0;
  }
  else
  {
    co = (XX - Xc )/ l;
    si = (YY - Yc )/ l;
  }
  if (l > W / 2 - r)
  {
    l = W / 2 - r;
    xe = co * l + l + Xc - W / 2 + r;
    ye = si * l + l + Yc - H / 2 + r;
  }
  else
  {
    xe = XX;
    ye = YY;
  }
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, xe - r , ye - r , xe + r, ye + r);  
}                                                    
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class","Error" , MB_OK);
    return 0;
  }
  hWnd =
    CreateWindow(WND_CLASS_NAME,
    "Title",
    WS_OVERLAPPEDWINDOW,
    1920, 0, 1280, 1024,
    NULL,
    NULL,
    hInstance,
    NULL);
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC, hMemDC;      //hMemDC - *
  static HBITMAP hBm;   //*
  BITMAP bm;            //*
  PAINTSTRUCT ps;
  RECT rc;
  POINT pt;
  static INT W = 0, H = 0;

  switch(Msg)
  {
  //*************
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    hDC = GetDC(hWnd);
    if (hBm != NULL)
      DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    return 0;
  //*****************
  case WM_TIMER :
    InvalidateRect( hWnd, NULL, TRUE );
    return 0;
  //****************
  case WM_ERASEBKGND:
    return 1;
  //******************
  case WM_CREATE :
    SetTimer( hWnd, 30, 10, NULL );
    return 0;
  case WM_CHAR :
    if ((CHAR)wParam == 27)
      DestroyWindow( hWnd );
    if ((CHAR)wParam =='F' || 'f')
      FlipFullScreen( hWnd );
    return 0;
  case WM_DESTROY:
    DeleteObject( hBm );  //*
    KillTimer( hWnd, 30 );
    PostQuitMessage(0);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint( hWnd, &ps );
    hMemDC = CreateCompatibleDC(hDC); //*
    SelectObject(hMemDC, hBm); //*

    GetClientRect(hWnd, &rc);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    Rectangle(hMemDC, 0, 0, W, H);

    DrawEye(hMemDC, 400, 500, pt.x, pt.y, 300, 300);   //*
    DrawEye(hMemDC, 800, 500, pt.x, pt.y, 300, 300);   //* 

    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY); //*
    DeleteDC(hMemDC); //*
    EndPaint(hWnd, &ps);                                                               
    return 0;
  }
  return DefWindowProc( hWnd, Msg, wParam, lParam);
}