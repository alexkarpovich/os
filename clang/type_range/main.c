#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <math.h>

int main() 
{
  printf("RANGES FROM LIMIT.H:\n");
  printf("|char|=[%d, %d], |uchar|=[%d, %d]\n", CHAR_MIN, CHAR_MAX, 0, UCHAR_MAX);
  printf("|short|=[%d, %d], |ushort|=[%d, %d]\n", SHRT_MIN, SHRT_MAX, 0, USHRT_MAX);
  printf("|int|=[%d, %d], |uint|=[%d, %u]\n", INT_MIN, INT_MAX, 0, UINT_MAX);
  printf("|long|=[%ld, %ld], |uint|=[%d, %lu]\n", LONG_MIN, LONG_MAX, 0, ULONG_MAX);
  printf("|float|=[%g, %g], |double|=[%g, %g]\n", FLT_MIN, FLT_MAX, DBL_MIN, DBL_MAX);
  printf("CALCULATED RANGES:\n");

  long max = ((((long)1 << (sizeof(long) * 8 - 2)) - 1) * 2) + 1;
  float fmax = 1.11111111111111111111111 * (pow(2, 24) - 1)*pow(2, 104);
  float size = (log10((double)10) / log10((double)2)) + 1.00000001;

  printf("char: %d\n", (1 << sizeof(char) * 8) - 1);
  printf("int: %d\n", (((1 << (sizeof(int) * 8 - 2)) - 1) * 2) + 1); 
  printf("long: %ld\n", max); 
  printf("float: %g, %g\n", size, fmax);
}
