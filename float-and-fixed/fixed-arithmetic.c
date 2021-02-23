#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 // veure la implementació de les "union" i
 // l'ús de big- i little-endian a ieee754.h
#include <ieee754.h>

static int verbose;

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


void to_fixed_number(float f, int integer_bits, int fraction_bits,
                     int * sign, int * integer, int * fraction)
{
        union ieee754_float fl;
        printf ("%15.10f: ", f);
        fl.f = f;
            printb (1, fl.ieee.negative, " ");
            // bit amagat
            printb (8, fl.ieee.exponent,
                             fl.ieee.exponent? " 1 " : " 0 ");
            printb (23, fl.ieee.mantissa, "  ");

        int integer_mask = ((1<<integer_bits)-1);
        int fraction_mask = ((1<<fraction_bits)-1);

        *sign = fl.ieee.negative;

        if (fl.ieee.exponent == 0) {
           *integer = 0;
           *fraction = 0;
        }
        else {
           int mantissa = (1 << 23) | fl.ieee.mantissa;
           int shift = 23-(fl.ieee.exponent - IEEE754_FLOAT_BIAS);
           //integer = mantissa >>
           //        (24-integer_bits+(fl.ieee.exponent-IEEE754_FLOAT_BIAS));
           *integer = mantissa >> shift;
           if (*integer > integer_mask) printf ("overflow ");
           *integer &= integer_mask;
           //int fraction1 = mantissa &
                     //(fraction_mask << (23 - integer_bits - fraction_bits));
           //fraction = fraction1 >> fraction_bits;
           int shift_fract = shift - fraction_bits;
           *fraction = mantissa >> shift_fract;
           *fraction &= fraction_mask;
        }
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
	    if (verbose) printf ("(first bit %d) ", bit);

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
   int res;

   float arg1, arg2;

   char op;

   int integer_bits, fraction_bits;
   int integer_mask, fraction_mask;

   int sign, integer, fraction;

   int arg = 1;
   verbose = 0;

   fraction_bits = FRACTION_BITS;
   integer_bits = INTEGER_BITS;

   if ((arg < argc) && (0==strcmp(argv[arg], "-h"))) {
      fprintf(stderr, "Us: %s [-v] [ [integer-bits] fraction-bits]\n", argv[0]);
      exit(1);
   }
   else if ((arg < argc) && (0==strcmp(argv[1], "-v"))) verbose = 1, arg++;
   else if (arg < argc) {
      integer_bits = 0;
      fraction_bits = strtol(argv[arg], NULL, 0);
      arg++;
   }
   if (arg < argc) {
      integer_bits = fraction_bits;
      fraction_bits = strtol(argv[arg], NULL, 0);
   }
   integer_mask = ((1<<integer_bits)-1);
   fraction_mask = ((1<<fraction_bits)-1);

   res = fscanf (stdin, "%f %c %f", &arg1, &op, &arg2);
   while (res > 0) {
     if(verbose) printf("%d   %f %c %f ", res, arg1, op, arg2);
     to_fixed_number(arg1, integer_bits, fraction_bits,
                      &sign, &integer, &fraction);
     
     unsigned int op1 = integer << fraction_bits | fraction;
     if (verbose) printf ("(%c0x%x) ", sign?'-':'+', op1);
     printf ("%c ", op);
     if (sign) op1 = ~op1 + 1;

     printf ("\n");

     to_fixed_number(arg2, integer_bits, fraction_bits, 
                      &sign, &integer, &fraction);

     unsigned int op2 = integer << fraction_bits | fraction;
     if (verbose) printf ("(%c0x%x) ", sign?'-':'+', op2);
     printf ("=\n");
     if (sign) op2 = ~op2 + 1;

     unsigned int result, rsign;
     switch (op) {
      case '+': result = op1 + op2; break;
      case '-': result = op1 - op2; break;
      case '*': result = op1 * op2; break;
      case '/': result = op1 / op2; break;
      default:
        printf (" Unknown operation!!! ");
        result = 0xffffffff;
     }

     if (verbose) printf ("%d ", result);

     if (result & (1 <<(integer_bits+fraction_bits))) {
         // result is negative
         result = ~(result-1);
         sign = 1;
     }
     else
         sign = 0;

     //sign = result >> (integer_bits+fraction_bits) & 1;
     integer = (result >> fraction_bits) & integer_mask;
     fraction = result & fraction_mask;
     
     display_number(sign, integer_bits, integer, 
			  fraction_bits, fraction);

     res = scanf ("%f %c %f", &arg1, &op, &arg2);
   }




   return 0;
}

