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

fenv_t fenv;

void sig_fpe(int signo, siginfo_t * si, void * ctx)
{
   int res, disable;
   printf ("sig-fpe captured %d (code %d)\n", signo, si->si_code);
   switch (si->si_code) {
    case FPE_INTDIV: printf ("FPE_INTDIV(%d, %d)", FPE_INTDIV, 0); disable = 0; break;
    case FPE_INTOVF: printf ("FPE_INTOVF(%d, %d)", FPE_INTOVF, FE_OVERFLOW); disable = FE_OVERFLOW; break;
    case FPE_FLTDIV: printf ("FPE_FLTDIV(%d, %d)", FPE_FLTDIV, FE_DIVBYZERO); disable = FE_DIVBYZERO; break;
    case FPE_FLTOVF: printf ("FPE_FLTOVF(%d, %d)", FPE_FLTOVF, FE_OVERFLOW); disable = FE_OVERFLOW; break;
    case FPE_FLTUND: printf ("FPE_FLTUND(%d, %d)", FPE_FLTUND, FE_UNDERFLOW); disable = FE_UNDERFLOW; break;
    case FPE_FLTRES: printf ("FPE_FLTRES(%d, %d)", FPE_FLTRES, FE_INEXACT); disable = FE_INEXACT; break;
    case FPE_FLTINV: printf ("FPE_FLTINV(%d, %d)", FPE_FLTINV, FE_INVALID); disable = FE_INVALID; break;
    case FPE_FLTSUB: printf ("FPE_FLTSUB(%d, %d)", FPE_FLTSUB, 0); disable = 0; break;
    default: printf ("Unknown code"); break;
   }
   printf("   addr %p\n", si->si_addr);
   exit(1);
}

void shownum(char * msg, union FLT val)
{
   int res;
   unsigned int bit;
   //printf ("%4.15f\n", val.flt);
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
   //res = feenableexcept(FE_ALL_EXCEPT);
   //if (res < 0) perror ("feenableexcept");
}


int main(int argc, char * argv[])
{
  int res;
  int fe_exceptions = 0;


  for (res = 1; res < argc; res++) {
   if      (strcmp("invalid", argv[res]) == 0) fe_exceptions |= FE_INVALID;
   else if (strcmp("denorm",  argv[res]) == 0) fe_exceptions |= __FE_DENORM;
   else if (strcmp("divbyzero", argv[res]) == 0) fe_exceptions |= FE_DIVBYZERO;
   else if (strcmp("overflow", argv[res]) == 0) fe_exceptions |= FE_OVERFLOW;
   else if (strcmp("underflow", argv[res]) == 0) fe_exceptions |= FE_UNDERFLOW;
   else if (strcmp("inexact", argv[res]) == 0) fe_exceptions |= FE_INEXACT;
   else { fprintf(stderr, "Unknown exception specified.\n");
	  fprintf(stderr, "Us: %s [invalid] [denorm] [divbyzero]\n", argv[0]);
	  fprintf(stderr, 
	     "                     [overflow] [underflow] [inexact]\n");
	  exit(1);
   }
  }

  struct sigaction action;
  action.sa_sigaction = sig_fpe;
  bzero(&action.sa_mask, sizeof(action.sa_mask));
  action.sa_flags = SA_RESTART | SA_SIGINFO;
  action.sa_restorer = NULL;

  res = sigaction(SIGFPE, &action, NULL);
  if (res < 0) perror ("sigaction");
  res = feenableexcept(fe_exceptions);
  if (res < 0) perror ("feenableexcept");
  else         printf ("Previous exceptions enabled: 0x%x\n", res);

	union FLT val1, val2, val;

	// +infinite
	val1.vals.sign = 0;
	val1.vals.exp = 255;
	val1.vals.mantissa = 0;
	shownum("+inf", val1);

	printf ("press <enter>\n");
	getchar();

	// -infinite
	val2.vals.sign = 1;
	val2.vals.exp = 255;
	val2.vals.mantissa = 0;
	shownum("-inf", val2);

	printf ("press <enter>\n");
	getchar();

	// nan
	val.flt = val2.flt + val1.flt;
	shownum(" nan: ", val);

	printf ("press <enter>\n");
	getchar();

	// nan
	val.flt = val1.flt + val2.flt;
	shownum(" nan: ", val);

	printf ("press <enter>\n");
	getchar();

	// snan
        val1.vals.sign = 1;
	val1.vals.exp = 255;
	val1.vals.mantissa = 1;
	shownum("snan", val1);

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

	printf ("press <enter>\n");
	getchar();


	val1.flt = 2.0f*atanf(1.0f);
	shownum("2*atanf(1)", val1);
	val2.flt = tanf(val1.flt);
	shownum("tanf(val1)", val2);

	printf ("press <enter>\n");
	getchar();

	val1.flt=0.5f*3.1415926535897932384626433832795028841971f;
	shownum("PI/2      ", val1);
	val2.flt = tanf(val1.flt);
	shownum("tanf(PI/2)", val2);

	printf ("press <enter>\n");
	getchar();

	// +infinite
	val1.vals.sign = 0;
	val1.vals.exp = 255;
	val1.vals.mantissa = 0;
	shownum("      +inf ", val1);
	val2.flt = atanf(val1.flt);
	shownum("atanf(+inf)", val2);

	printf ("press <enter>\n");
	getchar();

	// -infinite
	val1.vals.sign = 1;
	val1.vals.exp = 255;
	val1.vals.mantissa = 0;
	shownum("      -inf ", val1);
	val2.flt = atanf(val1.flt);
	shownum("atanf(-inf)", val2);

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

   return 0;
}
