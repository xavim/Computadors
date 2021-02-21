all: valsfp32 valsfp64 nan nan-exceptions exercises


valsfp32: valsfp32.c
	gcc -o valsfp32 -O -g valsfp32.c

valsfp64: valsfp64.c
	gcc -o valsfp64 -O -g valsfp64.c

nan: nan.c
	gcc -o nan -O -g nan.c -lm

nan-exceptions: nan-exceptions.c
	#gcc -fsignaling-nans -ftrapping-math -o nan-exceptions -O -g nan-exceptions.c -lm
	gcc -o nan-exceptions -O -g nan-exceptions.c -lm

exercises: exercises.c
	gcc -o exercises -O -g exercises.c

clean:
	rm -f valsfp64 valsfp32 nan nan-exceptions exercises 
