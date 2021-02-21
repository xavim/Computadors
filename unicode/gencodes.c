#include <stdio.h>
#include <stdlib.h>

// //
// generation of U+unicode characters
// with genchars.c
// can be checked with
//
// https://en.wikipedia.org/wiki/List_of_Unicode_characters
// //



int main(int argc, char * argv[])
{
   int res, i;
   int from = 0x1fbf0, to = 0x1fbf9;

   if (argc == 3) {
	  from = strtol(argv[1], NULL, 0);
	  to = strtol(argv[2], NULL, 0);
   }
   else if (argc != 1) {
	  fprintf(stderr, "Us: %s [from to]\n", argv[0]);
	  exit(1);
   }
   for (i=from; i <= to; i++) {
	   printf ("U+%04x\n", i);
   }
   return 0;
}
