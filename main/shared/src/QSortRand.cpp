// QSortRand.C

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <assert.h>
//#include <iostream.h>
#include "QSortRand.h"

static void rqsort(char* low, char* high );

typedef int (* PFI)(const void*, const void*);              /* Pointer to a function that */
                                   							/* returns an int. */
static  PFI    Comp;                /* Pointer to comparsion routine */
static  long   Width;               /* Width of an object in bytes */
/* ------------------------------------------------------------------ */

inline long random(long range) {
	return (long)((float)rand() / (float)RAND_MAX * range);
}


void qsortrand(void* base, size_t count, size_t width, int(*compare)(const void* d1, const void* d2) )
/* Perform a quick sort on an array starting at base. */
{

  srand((unsigned int)clock());

  Width = width;
  Comp = compare;

  if ( count > 1 )
    rqsort( (char*)base, (char*)base + ( ( count - 1 ) * width ) );

}

/* ------------------------------------------------------------------ */

static void rqsort(char* low, char* high )
//register char *low,
//              *high;
/* Workhorse function called by the access routine, qsort().
 */
{  char *pivot,
        *base;
   static char *a,         /* Used for exchange, will not be needed */
               *b;         /* during recusion, so they can be static.*/
   static int   tmp;       /* That way they will not take up stack   */
   static long  i;         /* space.                                 */
   static char*  switcher;  // swap this w/ pivot

   base = low ;            /* Remember base address of array. */
   pivot = high ;          /* Partition off the pivot.        */
   high -= Width;

	// swap pivot with a random element
//	assert(high >= low);
//	assert((long)RAND_MAX > 0);
//	long r = random(((long)high-(long)low)/(long)Width); // between 0 and count
//	if (r < 0) { cout << "DIFF: " << ((long)high-(long)low)/(long)Width << "\n";
//	cout << "R: " << r << " HIGH: " << (long)high << " LOW: " << (long)low << "\n";}
//	assert (r >= 0);
//	assert(r<=(high-low)/Width);
	switcher = low + random(((long)high-(long)low)/(long)Width) * Width;
//	assert (switcher >= low);
//	assert (switcher <= high);
   for ( b = switcher, a = pivot, i = Width; --i >= 0; a++, b++ )
   {  tmp = *b;          
      *b  = *a;
      *a  = tmp;
   }

 
   do
   {  while ( low < high  &&  (*Comp)(low,  pivot) <= 0 )
        low  += Width;
      while ( low < high  &&  (*Comp)(high, pivot) >= 0 )
        high -= Width;

      if ( low < high )      /* Exchange low & high */
      {
/*
         printf( "lev = %d: exchangeing high: <%s> & low <%s>\n",
                      Lev, *((char **)high), *((char **)low));
*/
         for ( b = low, a = high, i = Width; --i >= 0; a++, b++ )
         {  tmp = *b;           /* Exchange *low and *high */
            *b  = *a;
            *a  = tmp;
         }
      }
   } while ( low < high );

   if ( low < pivot  &&  (*Comp)(low,  pivot) > 0 )
      for ( b = low, a = pivot, i = Width; --i >= 0; a++, b++ )
      {  tmp = *b;           /* Exchange *low and *pivot */
         *b  = *a;
         *a  = tmp;
      }

   low += Width;

   if ( high - base < pivot - low )
   {
      if ( low  < pivot )
        rqsort( low,  pivot );
      if ( base < high  )
        rqsort( base, high );
   }
   else
   {
      if ( base < high  )
        rqsort( base, high );
      if ( low  < pivot )
        rqsort( low,  pivot );
   }
}
