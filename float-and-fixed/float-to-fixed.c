#include <math.h>
#include <errno.h>
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

void printlb(int bits, long num, char * const post)
{
   int i;
   for (i=bits-1; i >= 0; i--)
      printf ("%c", (num & (1<<i)) ? '1' : '0');
   printf (post);
}

void display_number(float f, int integer_bits, int fraction_bits)
{
	int integer, fraction;
	union ieee754_float fl;
	printf ("%15.10f: ", f);
	fl.f = f;
            printb (1, fl.ieee.negative, " ");
	    // bit amagat
	    printb (8, fl.ieee.exponent, 
			     fl.ieee.exponent? " 1 " : " 0 ");
	    printb (23, fl.ieee.mantissa, ": ");

        int integer_mask = ((1<<integer_bits)-1);
        int fraction_mask = ((1<<fraction_bits)-1);

	int sign = fl.ieee.negative;

	if (fl.ieee.exponent == 0) {
           integer = 0;
	   fraction = 0;
	}
	else {
           int mantissa = (1 << 23) | fl.ieee.mantissa;
	   int shift = 23-(fl.ieee.exponent - IEEE754_FLOAT_BIAS);
	   //integer = mantissa >> 
	   //	     (24-integer_bits+(fl.ieee.exponent-IEEE754_FLOAT_BIAS));
#if 0
	   printf ("\nShift %d   exponent %d  Bias %d\n", 
			    shift, fl.ieee.exponent, IEEE754_FLOAT_BIAS);
#endif
	   if (shift > 31) integer = 0;
	   else integer = mantissa >> shift;
	   if (integer > integer_mask) {
              printf ("\n   overflow(");
	      printb(32, mantissa, ", ");
	      printb(32, shift, "\n            ");
	      printb(32, integer, ", ");
	      printb(32, integer_mask, ")\n");
              integer = integer_mask;
	      fraction = fraction_mask;
	   }
	   else {
	      integer &= integer_mask;
	      //int fraction1 = mantissa &
	       //(fraction_mask << (23 - integer_bits - fraction_bits));
	      //fraction = fraction1 >> fraction_bits;
              int shift_fract = shift - fraction_bits;
#if 0
	      printf ("shift_fract %d\n", shift_fract);
#endif
	      if (shift_fract > 31) fraction = 0;
	      else fraction = mantissa >> shift_fract;
	      fraction &= fraction_mask;
	   }
	}

            printb (1, sign, " ");
	    printb (integer_bits, integer, " ");
	    printb (fraction_bits, fraction, "\n");

	    int mantissa = (integer << fraction_bits) | fraction;
	    int bit = 31;
	    while((bit > 0) && (mantissa & (1<<bit)) == 0) --bit;
	    printf ("(first bit %4d) ", bit);

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
	    printb (23, fl.ieee.mantissa, ": ");

	    printf ("%15.10f\n", fl.f);

	    long long_mantissa = (integer << fraction_bits) | fraction;
	    if (sign) long_mantissa = 0-long_mantissa;
	    printf ("fixed: ");
	    printlb(1 + integer_bits + fraction_bits, long_mantissa, "\n");
}

int main(int argc, char * argv[])
{
   float f;
   int i;

   int integer_bits, fraction_bits;

   int sign, integer, fraction;

   if (argc < 2) {
      fprintf (stderr, "Us: %s <integerbits> <fractionbits> [fpnumbers]...\n",
		      argv[0]);
      exit (1);
   }
   if (argc > 2) {
      integer_bits = strtol(argv[1], NULL, 0);
      fraction_bits = strtol(argv[2], NULL, 0);
   }

   printf ("    fp number  : s    exp   h        mantissa        : s integer fractional\n");

   i = 3;
   while (i < argc) {
      errno = 0;
      f = (float) strtof(argv[i], NULL);
      if (errno != 0) { perror ("strtof"); exit(1); }
      display_number(f, integer_bits, fraction_bits);
    ++i;
   }

   return 0;
}

