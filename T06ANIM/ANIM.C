/* FILENAME: ANIM.C
 * PROGRAMMER: MY6
 * PURPOSE: Base animation module
 * LAST UPDATE: 10.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"

#define MY6_JOYSTICK_THRESHOLD 64.0

#define MY6_GET_AXIS_VALUE(A) \
  ((INT)(MY6_JOYSTICK_THRESHOLD * (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / \
   (jc.w ## A ## max - jc.w ## A ##min - 1) - 1) + 0.5) / MY6_JOYSTICK_THRESHOLD)


/* Системный контекст анимации */
static my6ANIM MY6_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MY6_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  MY6_Anim.hWnd = hWnd;
  MY6_Anim.hDC = CreateCompatibleDC(hDC);
  MY6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  MY6_Anim.W = 30;
  MY6_Anim.H = 30;
  MY6_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* инициализируем таймер */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
} /* End of 'MY6_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MY6_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < MY6_Anim.NumOfUnits; i++)
  {
    MY6_Anim.Units[i]->Close(MY6_Anim.Units[i], &MY6_Anim);
    free(MY6_Anim.Units[i]);
  }
  DeleteObject(MY6_Anim.hBmFrame);
  DeleteDC(MY6_Anim.hDC);
  memset(&MY6_Anim, 0, sizeof(my6ANIM));
} /* End of 'MY6_AnimInit' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MY6_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(MY6_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(MY6_Anim.hBmFrame);
  MY6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(MY6_Anim.hDC, MY6_Anim.hBmFrame);

  /* Сохранение размера */
  MY6_Anim.W = W;
  MY6_Anim.H = H;

  ReleaseDC(MY6_Anim.hWnd, hDC);
} /* End of 'MY6_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MY6_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  MY6_AnimResponse();
  /* Обновление ввода */
  GetKeyboardState(MY6_Anim.Keys);
  for (i = 0; i < 256; i++)
    MY6_Anim.Keys[i] >>= 7;

  /* Обновление таймера */
  MY6_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* Обновление кадра */
  QueryPerformanceCounter(&li);

  /* глобальное время */
  MY6_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  MY6_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* локальное время */
  if (MY6_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    MY6_Anim.DeltaTime = 0;
  }
  else
    MY6_Anim.DeltaTime = MY6_Anim.GlobalDeltaTime;

  MY6_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    MY6_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  /* очистка фона */
  SelectObject(MY6_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(MY6_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(MY6_Anim.hDC, RGB(110, 55, 55));
  Rectangle(MY6_Anim.hDC, 0, 0, MY6_Anim.W, MY6_Anim.H);

  /* call for changing состояний объектов */
  for (i = 0; i < MY6_Anim.NumOfUnits; i++)
    MY6_Anim.Units[i]->Response(MY6_Anim.Units[i], &MY6_Anim);

  /* drawing objects*/
  for (i = 0; i < MY6_Anim.NumOfUnits; i++)
  {
    SelectObject(MY6_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(MY6_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(MY6_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(MY6_Anim.hDC, RGB(55, 155, 255));
    MY6_Anim.Units[i]->Render(MY6_Anim.Units[i], &MY6_Anim);
  }

  FrameCounter++;
} /* End of 'MY6_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MY6_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(MY6_Anim.hWnd);
  BitBlt(hDC, 0, 0, MY6_Anim.W, MY6_Anim.H, MY6_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(MY6_Anim.hWnd, hDC);
} /* End of 'MY6_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       my6UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MY6_AnimAddUnit( my6UNIT *Unit )
{
  if (MY6_Anim.NumOfUnits < MY6_MAX_UNITS)
  {
    MY6_Anim.Units[MY6_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &MY6_Anim);
  }
} /* End of 'MY6_AnimAddUnit' function */

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MY6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(MY6_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(MY6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    /* для одного монитора:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(MY6_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(MY6_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(MY6_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'MY6_AnimFlipFullScreen' function */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MY6_AnimSetPause( BOOL NewPauseFlag )
{
  MY6_Anim.IsPause = NewPauseFlag;
} /* End of 'MY6_AnimSetPause' function */

VOID MY6_AnimResponse( VOID )
{
  INT i;

  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(MY6_Anim.JButsOld, MY6_Anim.JButs, sizeof(MY6_Anim.JButs));
        for (i = 0; i < 32; i++)
          MY6_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          MY6_Anim.JButsClick[i] = MY6_Anim.JButs[i] && !MY6_Anim.JButsOld[i];
        if (MY6_Anim.JButsClick[3])
          DestroyWindow(MY6_Anim.hWnd);
        if (MY6_Anim.JButsClick[9])
          MY6_AnimFlipFullScreen();;
        /* Оси */
        MY6_Anim.JX = MY6_GET_AXIS_VALUE(X);
        MY6_Anim.JY = MY6_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          MY6_Anim.JZ = MY6_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASR)
          MY6_Anim.JR = MY6_GET_AXIS_VALUE(R);
        if (jc.wCaps & JOYCAPS_HASU)
          MY6_Anim.JU = MY6_GET_AXIS_VALUE(U);

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            MY6_Anim.JPOV = 0;
          else
            MY6_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }
}

/* END OF 'ANIM.C' FILE */
