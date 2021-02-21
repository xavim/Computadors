#include <stdio.h>
#include <stdlib.h>

// //
// generation of U+unicode characters
// with gencodes.c
// can be checked with
//
// https://en.wikipedia.org/wiki/List_of_Unicode_characters
// // 

int main()
{
   int res ;

   unsigned int unicode;
   unsigned char c0, c1, c2, c3;

   res = scanf("U+%x\n", &unicode);
   if (res == 0) printf ("No input...\n");
   while (res > 0) {
      c0 = unicode >> 24;
      c1 = (unicode >> 16) & 0x000000ff;
      c2 = (unicode >>  8) & 0x000000ff;
      c3 = (unicode      ) & 0x000000ff;

      if (c0) {
         fprintf (stderr, "%02x ", c0);
         printf ("%c", c0);
      }
      if (c0 || c1) {
         fprintf (stderr, "%02x ", c1);
         printf ("%c", c1);
      }
      if (c0 || c1 || c2) {
         fprintf (stderr, "%02x ", c2);
         printf ("%c", c2);
      }
      fprintf (stderr, "%02x, ", c3);
      printf ("%c", c3);
     res = scanf("U+%x\n", &unicode);
   }
   if (res < 0 && ferror(stdin)) { perror ("scanf"); exit(1); }
   printf ("\n");

   return 0;
}

