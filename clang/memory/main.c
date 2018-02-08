#include <stdio.h>

#define NALLOC 1024 /* минимальное число единиц памяти для запросов */

typedef long Align; /* для выравнивания по границе long */

union header { /* заголовок блока */
  struct {
    union header *ptr; /* следующий блок в списке свободных */
    unsigned size; /* размер этого блока */
  } s;
  Align x; /* принудельное выравнивание блока */
};

typedef union header Header;

static Header base; /* пустой список для начальных значений */
static Header *freep = NULL; /* начало в списке свободых блоков */

void freec(void *);

/* запрашивает у системы дополнительную память */
static Header *morecorec(unsigned nu)
{
  char *cp, *sbrk(int);
  Header *up;

  if (nu < NALLOC)
    nu = NALLOC;

  cp = sbrk(nu * sizeof(Header));

  if (cp == (char *)-1) /* больше памяти нет */
    return NULL;

  up = (Header *) cp;
  up->s.size = nu;
  freec((void *)(up+1));
  return freep;
}

/* универсальный распределитель памяти */
void *mallocc(unsigned nbytes)
{
  Header *p, *prevp;
  Header *morecorec(unsigned);
  unsigned nunits;

  nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
  if ((prevp = freep) == NULL) { /* списка свободной памяти еще нет */
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }

  for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
    if (p->s.size >= nunits) { /* достаточно большой */
      if (p->s.size == nunits) /* точно нужного размера */
        prevp->s.ptr = p->s.ptr;
      else { /* отрезаем хвостовую часть */
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp;
      
      return (void *)(p + 1);
    }
    if (p == freep) /* прошли полный цикл по списку */
      if ((p = morecorec(nunits)) == NULL)
        return NULL; /* больше памяти нет */
  }
}

/* включает блок в список свободной памяти */
void freec(void *ap)
{
  Header *bp, *p;

  bp = (Header *) ap - 1; /* указатель на заголовок блока */
  for (p=freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break; /* освобождаем блок в начале или в конце */

  if (bp + bp->s.size == p->s.ptr) { /* слить с верхним соседом */
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;

  if (p + p->s.size == bp) { /* слить с нижним соседом */
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;

  freep = p;
}

int main() 
{
  int *k = mallocc(10);

  for (int i = 0; i < 10; i++) k[i] = i;
  for (int i = 0; i < 10; i++) printf("%d\n", k[i]);

  
  return 0;
}
