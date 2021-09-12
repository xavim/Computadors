all: gencodes genchars

gencodes: gencodes.c
	gcc -o gencodes -O -g gencodes.c

genchars: genchars.c
	gcc -o genchars -O -g genchars.c

clean:
	rm -f gencodes genchars

# https://www.fileformat.info/info/unicode/char/00C1/index.htm

gen-greekchars:gencodes genchars
	./gencodes 0xceb0 0xcebf | ./genchars >chars2.out
	./gencodes 0xce90 0xce9f | ./genchars >>chars2.out
	cat chars2.out

gen-accents: gencodes genchars
	./gencodes 0xc3a0 0xc3bf | ./genchars >chars.out
	./gencodes 0xc380 0xc39f | ./genchars >>chars.out
	cat chars.out

