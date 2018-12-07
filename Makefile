pftp: main.o connect.o analyze.o
	cc -o pftp main.o connect.o analyze.o

main.o: main.c
	cc -c main.c
connect.o: connect.c connect.h
	cc -c connect.c
analyze.o: analyze.c analyze.h
	cc -c analyze.c
clean:
	rm pftp main.o connect.o analyze.o
