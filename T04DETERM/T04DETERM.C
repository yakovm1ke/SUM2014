/* MY6, 04.06.2014 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include <windows.h>

#define MAX 10
double A[MAX][MAX];
double GlobalDeterminant = 0;
int P[MAX], Parity = 0;
int N;

void Write( void )
{
  int i;
  FILE *F;     

  if ((F = fopen("a.log", "a")) != NULL)
  {
    fprintf(F, "%f\n", GlobalDeterminant);
    fclose(F);
  }
}
void Swap(int *a, int*b)
{
  int tmp  = *a;
  *a = *b;
  *b = tmp;
}
void Go( int Pos )
{
  int i;

  if (Pos == N)
  {
    double prod = 1;
    for (i = 0; i < N; i++)
      prod *= A[i][P[i]];
    if(Parity % 2 == 0)
      GlobalDeterminant  += prod;
    else
      GlobalDeterminant  -= prod;
  }
  else
  {
    Go(Pos + 1);
    for (i = Pos + 1; i < N; i++ )      	
    {
      Swap(&P[Pos], &P[i]);
      Parity = !Parity;
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
      Parity = !Parity;
    }
  }
}


void Load( char *FileName )
{
  FILE *F;
  int i, j;

  N = 0;
  F = fopen(FileName, "r");
  if (F == NULL)
    return;
  fscanf(F, "%i", &N);
  if (N < 0)
    N = 0;
  else
    if (N > MAX)
      N = MAX;
  
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
}
/*
{
  INT maxi = i, maxj = i, ki, kj;
  for (ki = i + 1; ki < N; ki++)
    for(kj = i + 1 ; kj < N; kj++)
      if (A[ki][kj] > A[maxi][maxj])
        maxi = ki, maxj = kj;  
}
if(i != maxi)
  for(kj = 0; kj < N; kj++)
    Swap( &A[i][kj], &A[maxi][kj]);
  sign = -sign;
if(j != maxj)
  for(ki = 0; ki < N; ki++)
   Swap( &A[ki][j], &A[ki][maxj]);
  sign = -sign;


for(k = i + 1; k < N; k++)
  for(j = i + 1; j < N; j++)
    A[k][j] -= A[i][j] * A[k][i] / A[i][i];
*/
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  int i;

  Load("r");
  for(i = 0; i < N; i++)
    P[i] = i ;
  Go(0);
  Write();
  return 0;
}