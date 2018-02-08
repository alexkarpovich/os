#include <stdio.h>

struct {
  unsigned int lime;
  unsigned int orange;
  unsigned int apple;
  unsigned int grape;
  unsigned int limon;
  unsigned int qiwi;
} fruit;

struct {
  unsigned int lime : 1;
  unsigned int orange : 1;
  unsigned int apple : 1;
  unsigned int grape : 1;
  unsigned int limon : 1;
  unsigned int qiwi: 1;
} bitfruit;

union test {
  int age;
  double weight;
} ut;

int main() 
{
  bitfruit.lime = 1;
  bitfruit.grape = 1;

  ut.age = 10;
  ut.weight= 12.3;
  
  printf("Size of simple structure is %lu\n", sizeof(fruit));
  printf("Size of bitfield structure is %lu\n", sizeof(bitfruit));
  printf("Size of bitfield structure is %d\n", bitfruit);
  printf("Size of union structure is %lu\n", sizeof(ut));
  printf("test %g\n", ut.weight);


  return 0;
}
