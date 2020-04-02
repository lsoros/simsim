CC = c++
CFLAGS = -std=c++11 -stdlib=libc++ -g -Wall -O

all : objDef.o main simulation

objDef.o : sim_objects.cpp
	$(CC) $(CFLAGS) -c sim_objects.cpp -o objDef.o

main : main.cpp
	$(CC) $(CFLAGS) objDef.o main.cpp -o main

simulation : simulation.cpp
	$(CC) $(CFLAGS) objDef.o simulation.cpp -o simulation

clean:
	rm -f $(TARGET) *.o