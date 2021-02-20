#include <stdio.h>

struct test {
	    int enter;      // 4 bytes
	    char caracter;  // 1 byte
            float decimal;  // 4 bytes
};                       // total 9 -> 12
struct test2 {
	    int enter;      // 4 bytes
	    double decimal; // 8 bytes
	    char caracter;  // 1 byte
};                       // total 13 -> 16 (no) -> 24!!!

struct test3 {
	    double decimal; // 8 bytes
	    int enter;      // 4 bytes
	    char caracter;  // 1 byte
};                       // total 13 -> 16 (char fits in the padding to 8 :)


int main()
{
	printf ("sizeof (test) { int(4), char(1), float(4) }: %d\n",
		       	sizeof(struct test));
	printf ("sizeof (test2) { int(4), double(8), char(1) }: %d\n",
		       	sizeof(struct test2));
	printf ("sizeof (test3) { double(8), int(4), char(1) }: %d\n",
		       	sizeof(struct test3));
	return 0;
}
