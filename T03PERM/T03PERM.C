/* MY6, 04.06.2014 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include  <windows.h>
#define N 4

int P[N], pp = 0;

void Swap(int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void Write( void )
{
  int i;
  FILE *F;     

  if ((F = fopen("a.log", "a")) != NULL)
  {
    for (i = 0; i < N; i++)
      fprintf(F, "%i ", P[i]);
    fprintf(F, " - %i\n", pp);
    fclose(F);
  }
}

void Go( int Pos )
{
  int i;

  if (Pos == N)
    Write();
  else
  {
    Go(Pos + 1);
    for (i = Pos + 1; i < N; i++ )      	
    {
      Swap(&P[Pos], &P[i]);
      pp = !pp;
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
      pp = !pp;
    }
  }
}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  int i;

  remove("a.log");
  for (i = 0; i < N; i++)
    P[i] = i + 1;
  Go(0);
  return 0;
}