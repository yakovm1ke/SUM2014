/* MY6, 02.06.2014 */

#include <math.h>

#include <windows.h>


#define WND_CLASS_NAME "My window class"
#define r0 ((float)rand() / RAND_MAX)
#define SQT(x) ( x * x )

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wPAram, LPARAM lParam );
/*Xc, Yc - center of eye, XX, YY - the position of mouse */
void DrawEye( HDC hDC, INT Xc, INT Yc, INT XX, INT YY, INT W, INT H )
{
  FLOAT r = 20, l, cos, sin, xe, ye, ratio = W / H;

  Ellipse( hDC, Xc - W / 4, 0, Xc + W / 4, H );
  l = sqrt(SQT(XX - Xc) + SQT(YY - Yc));
  cos = (XX - Xc) / l;
  sin = (YY - Yc) / l;
  xe = l * cos;
  ye = l * sin * ratio;
  Ellipse(hDC, xe - r, ye - r, xe + r, ye + r);  
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
  HDC hDC;
  PAINTSTRUCT ps;
  RECT rc;
  POINT pt;

  switch(Msg)
  {
  case WM_TIMER :
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_CREATE :
    SetTimer( hWnd, 30, 10, NULL );
    return 0;
  case WM_CHAR :
    if((CHAR)wParam == 27)
      DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    KillTimer( hWnd, 30);
    PostQuitMessage(0);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint( hWnd, &ps );

    GetClientRect(hWnd, &rc);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    /*SetDCBrushColor(hDC, RGB(0, 0, 0));
    DrawEye(hDC, rc.right / 4, rc.bottom / 2, pt.x , pt.y , rc.right , rc.bottom);
    DrawEye(hDC, rc.right * 3 / 4, rc.bottom / 2, pt.x , pt.y , rc.right , rc.bottom);    
    SetDCPenColor(hDC, RGB(0, 0, 0));
    DrawEye(hDC, rc.right / 4, rc.bottom / 2, pt.x , pt.y , rc.right , rc.bottom);    
    EndPaint(hWnd, &ps);*/
    return 0;
  }
  return DefWindowProc( hWnd, Msg, wParam, lParam);
}