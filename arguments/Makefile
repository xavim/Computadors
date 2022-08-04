all: args arguments

arguments: arguments.o
	g++ -O -o arguments arguments.o

arguments.o: arguments.C
	g++ -O -c arguments.C

args: args.c
	gcc -O -o args args.c

clean:
	rm -f arguments arguments.o args
