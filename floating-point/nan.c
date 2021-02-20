#define _GNU_SOURCE
#include <fenv.h>
#include <math.h>
#include <values.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

struct FLOAT {
	unsigned int mantissa:23;
	unsigned int exp:8;
	unsigned int sign:1;
};

union FLT {
	struct FLOAT vals;
	float        flt;
};

void shownum(char * msg, union FLT val)
{
   int res;
   unsigned int bit;

   printf ("%s: ", msg);
   printf ("%e %f\n", (float) val.flt, (float) val.flt);
   printf ("%s: ", msg);
   printf ("   %u     %u               %u\n", val.vals.sign, val.vals.exp, val.vals.mantissa);
   printf ("%s: ", msg);
   printf ("   %u  ", val.vals.sign);
   for (bit=0x80; bit>0;  bit >>= 1)  printf ("%u", (val.vals.exp & bit)?1:0);
   printf ("  ");
   for (bit=0x400000; bit>0;  bit >>= 1)  printf ("%u", (val.vals.mantissa & bit)?1:0);
   printf("\n");
}

void check_exceptions(void)
{
  int res;
  res = fetestexcept(FE_ALL_EXCEPT);
  if (res < 0) perror ("fetestexcept");
  else         printf ("Active exceptions : 0x%x %s %s %s %s %s %s\n", res,
		  (res & FE_INVALID)? "INVALID" : "",
		  (res & __FE_DENORM)? "DENORM" : "",
		  (res & FE_DIVBYZERO)? "DIVBYZERO" : "",
		  (res & FE_OVERFLOW)? "OVERFLOW" : "",
		  (res & FE_UNDERFLOW)? "UNDERFLOW" : "",
		  (res & FE_INEXACT)? "INEXACT" : ""
		  );
  res = feclearexcept(FE_ALL_EXCEPT);
  if (res < 0) perror ("feclearexcept");
}



int main()
{
  int res;

        check_exceptions();

	union FLT val1, val2, val;

	// +infinite
	val1.vals.sign = 0;
	val1.vals.exp = 255;
	val1.vals.mantissa = 0;
	shownum("+inf", val1);
        check_exceptions();

	printf ("press <enter>\n");
	getchar();

	// -infinite
	val2.vals.sign = 1;
	val2.vals.exp = 255;
	val2.vals.mantissa = 0;
	shownum("-inf", val2);
        check_exceptions();

	printf ("press <enter>\n");
	getchar();

	// nan
	val.flt = val2.flt + val1.flt;
	shownum(" nan: ", val);
        check_exceptions();

	printf ("press <enter>\n");
	getchar();

	// nan
	val.flt = val1.flt + val2.flt;
	shownum(" nan: ", val);
        check_exceptions();

	printf ("press <enter>\n");
	getchar();

	// snan
        val1.vals.sign = 1;
	val1.vals.exp = 255;
	val1.vals.mantissa = 1;
	shownum("snan", val1);
        check_exceptions();

	printf ("press <enter>\n");
	getchar();

        // maxfloat
        val1.flt = MAXFLOAT;
	shownum("maxfloat: ", val1);
	val2.flt = 1.11412044995943e31;
	shownum("   +1.1e31: ", val2);
	//val1.flt += 1.014120475e31;
	val1.flt += val2.flt;
	shownum("      =   ", val1);
        check_exceptions();

	printf ("press <enter>\n");
	getchar();


	val1.flt = 2.0f*atanf(1.0f);
	shownum("2*atanf(1)", val1);
	val2.flt = tanf(val1.flt);
	shownum("tanf(val1)", val2);
        check_exceptions();

	printf ("press <enter>\n");
	getchar();

	val1.flt=0.5f*3.1415926535897932384626433832795028841971f;
	shownum("PI/2      ", val1);
	val2.flt = tanf(val1.flt);
	shownum("tanf(PI/2)", val2);
        check_exceptions();

	printf ("press <enter>\n");
	getchar();

	// +infinite
	val1.vals.sign = 0;
	val1.vals.exp = 255;
	val1.vals.mantissa = 0;
	shownum("      +inf ", val1);
	val2.flt = atanf(val1.flt);
	shownum("atanf(+inf)", val2);
        check_exceptions();

	printf ("press <enter>\n");
	getchar();

	// -infinite
	val1.vals.sign = 1;
	val1.vals.exp = 255;
	val1.vals.mantissa = 0;
	shownum("      -inf ", val1);
	val2.flt = atanf(val1.flt);
	shownum("atanf(-inf)", val2);
        check_exceptions();

	printf ("press <enter>\n");
	getchar();

	// qnan
        val1.vals.sign = 1;
	val1.vals.exp = 255;
	val1.vals.mantissa = 1U<<22;
	shownum("qnan", val1);
	val2.vals.sign = 1;
	val2.vals.exp = 127;
	val2.vals.mantissa = 1U<<22;
	shownum("-1.5", val2);
	val2.flt -= val1.flt;
	//val2.vals.exp = 127;
	shownum("  = ", val2);
        check_exceptions();

	printf ("press <enter>\n");
	getchar();

	// snan
        val1.vals.sign = 1;
	val1.vals.exp = 255;
	val1.vals.mantissa = 1U<<11;
	shownum("snan", val1);
	val2.vals.sign = 1;
	val2.vals.exp = 127;
	val2.vals.mantissa = 1U<<22;
	shownum("-1.5", val2);
	val2.flt -= val1.flt;
	//val2.vals.exp = 127;
	shownum("  = ", val2);
        check_exceptions();

   return 0;
}
