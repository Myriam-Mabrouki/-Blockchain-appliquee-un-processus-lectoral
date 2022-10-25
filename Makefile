cc = gcc -Wall

all: main_p1 main_p2 main_p3 main_p4 main_p5

partie1.o: partie1.c partie1.h
	$(cc) -c partie1.c

partie2.o: partie2.c partie2.h partie1.h
	$(cc) -c partie2.c

partie3.o: partie3.c partie3.h partie2.h partie1.h
	$(cc) -c partie3.c
	
partie4.o: partie4.c partie4.h partie3.h partie2.h partie1.h
	$(cc) -c partie4.c -lssl -lcrypto
	
partie5.o: partie5.c partie5.h partie4.h partie3.h partie2.h partie1.h
	$(cc) -c partie5.c -lssl -lcrypto

main_p1.o: main_p1.c partie1.h
	$(cc) -c main_p1.c

main_p1: partie1.o main_p1.o
	$(cc) -o main_p1 main_p1.o partie1.o

main_p2.o: main_p2.c partie2.h partie1.h
	$(cc) -c main_p2.c

main_p2: partie2.o main_p2.o partie1.o
	$(cc) -o main_p2 main_p2.o partie2.o partie1.o

main_p3.o: main_p3.c partie3.h partie2.h partie1.h
	$(cc) -c main_p3.c

main_p3: partie3.o main_p3.o partie2.o partie1.o
	$(cc) -o main_p3 main_p3.o partie3.o partie2.o partie1.o
	
main_p4.o: main_p4.c partie4.h partie3.h partie2.h partie1.h
	$(cc) -c main_p4.c -lssl -lcrypto

main_p4: main_p4.o partie4.o partie3.o partie2.o partie1.o
	$(cc) -o main_p4 main_p4.o partie4.o partie3.o partie2.o partie1.o -lssl -lcrypto
	
main_p5.o: main_p5.c partie5.h partie4.h partie3.h partie2.h partie1.h
	$(cc) -c main_p5.c -lssl -lcrypto

main_p5: main_p5.o partie5.o partie4.o partie3.o partie2.o partie1.o
	$(cc) -o main_p5 main_p5.o partie5.o partie4.o partie3.o partie2.o partie1.o -lssl -lcrypto

clean:
	rm -f *.o main_test main_p1 main_p2 main_p3 main_p4 main_p5
