#include <math.h>
#include <stdio.h>
#include <stdlib.h>

 // veure la implementació de les "union" i
 // l'ús de big- i little-endian a ieee754.h
#include <ieee754.h>


// https://en.wikipedia.org/wiki/Fixed-point_arithmetic
//
//
void printb(int bits, int num, char * const post)
{
   int i;
   for (i=bits-1; i >= 0; i--)
      printf ("%c", (num & (1<<i)) ? '1' : '0');
   printf (post);
}

void display_number(int sign, int integer_bits, int integer,
	                      int fraction_bits, int fraction)
{
	union ieee754_float fl;

            printb (1, sign, " ");
	    printb (integer_bits, integer, " ");
	    printb (fraction_bits, fraction, ": ");

	    int mantissa = (integer << fraction_bits) | fraction;
	    int bit = 31;
	    while((bit > 0) && (mantissa & (1<<bit)) == 0) --bit;
	    printf ("(first bit %d) ", bit);

	    if (bit > 23) { 
		    printf ("trunc'ed ");
	    }

	    fl.ieee.negative = sign;
	    if ((bit == 0) && (mantissa==0))
               fl.ieee.exponent = 0;
	    else
               fl.ieee.exponent = IEEE754_FLOAT_BIAS + bit - fraction_bits;
	    // perdem el bit amagat
	    if ((23-bit) >= 0)
	       fl.ieee.mantissa = (mantissa << (23-bit));
	    else
	       fl.ieee.mantissa = (mantissa >> (bit-23));
            printb (1, fl.ieee.negative, " ");
	    // bit amagat
	    printb (8, fl.ieee.exponent, 
			     fl.ieee.exponent? " 1 " : " 0 ");
	    printb (23, fl.ieee.mantissa, " ");

	    printf ("%.10f\n", fl.f);
}

#define INTEGER_BITS 2
#define FRACTION_BITS 3

int main(int argc, char * argv[])
{
   int integer_bits, fraction_bits;
   int integer_mask, fraction_mask;

   int sign, integer, fraction;

   if (argc == 1) {
      fraction_bits = FRACTION_BITS;
      integer_bits = INTEGER_BITS;
   }
   else if (argc > 1) {
      integer_bits = 0;
      fraction_bits = strtol(argv[1], NULL, 0);
   }
   if (argc > 2) {
      integer_bits = fraction_bits;
      fraction_bits = strtol(argv[2], NULL, 0);
   }
   integer_mask = ((1<<integer_bits)-1);
   fraction_mask = ((1<<fraction_bits)-1);

   sign = 0;

   //for (sign = 0; sign <= 1; sign++) {
      for (integer = integer_mask; integer >= 0; integer--) {
         for (fraction = fraction_mask; fraction >= 0; fraction--) {

            display_number(sign, integer_bits, integer, 
			         fraction_bits, fraction);
         }
      }
   //}
   sign = 1;
      for (integer = 0; integer <= integer_mask; integer++) {
         for (fraction = 0; fraction <= fraction_mask; fraction++) {

            display_number(sign, integer_bits, integer, 
			         fraction_bits, fraction);
         }
      }
   return 0;
}

