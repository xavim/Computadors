#include <math.h>
#include <values.h>
#include <stdio.h>

struct FLOAT {
	unsigned int mantissa:23;
	unsigned int exp:8;
	unsigned int sign:1;
};

union FLT {
	struct FLOAT vals;
	float        flt;
};

void shownum(union FLT val)
{
   unsigned int bit;
   //printf ("%4.15f\n", val.flt);
   printf ("%e %f\n", val.flt, val.flt);
   printf ("   %u     %u               %u\n", val.vals.sign, val.vals.exp, val.vals.mantissa);
   printf ("   %u  ", val.vals.sign);
   for (bit=0x80; bit>0;  bit >>= 1)  printf ("%u", (val.vals.exp & bit)?1:0);
   printf ("  ");
   for (bit=0x400000; bit>0;  bit >>= 1)  printf ("%u", (val.vals.mantissa & bit)?1:0);
   printf("\n");
}


int main()
{
	union FLT val1, val2, val;


	// -46
	val1.vals.sign = 1;
	val1.vals.exp = 132;
	val1.vals.mantissa = ((32+8+4+2)-32) << 23-5;
	shownum(val1);
	// +73
	val2.vals.sign = 0;
	val2.vals.exp = 133;
	val2.vals.mantissa = ((64+8+1)-64) << 23-6;
	shownum(val2);

	val.flt = val1.flt + val2.flt;
	shownum(val);

   return 0;
}
