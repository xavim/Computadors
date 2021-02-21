all: new-except


new-except: new-except.cpp
	g++ -static -O -g -o new-except new-except.cpp

run: new-except
	(./new-except)

run-except: new-except
	(ulimit -d 8000; ./new-except )

clean:
	rm -f new-except
