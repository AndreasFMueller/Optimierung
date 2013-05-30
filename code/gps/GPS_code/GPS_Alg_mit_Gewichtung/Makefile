
CC = g++
LINK = g++
# '-pedantic' sorgt dafuer, dass g++ streng nach dem C++-Standard arbeitet
CFLAGS = -c -pedantic
LFLAGS = -pedantic -llapack -math
OBJS = gps.o diffFunk.o

gps: $(OBJS)
	$(LINK) $(LFLAGS) -o gps $(OBJS)

gps.o: gps.cpp diffFunk.h
	$(CC) $(CFLAGS) gps.cpp

diffFunk.o: diffFunk.cpp diffFunk.h
	$(CC) $(CFLAGS) diffFunk.cpp

clean:
	rm -f main $(OBJS)
