CC = c++
CFLAGS = -std=c++11 -stdlib=libc++ -g -Wall -O

all : objDef.o simulation.o novelty.o main simsim

objDef.o : sim_objects.cpp
	$(CC) $(CFLAGS) -c sim_objects.cpp -o objDef.o

simulation.o : simulation.cpp
	$(CC) $(CFLAGS) -c objDef.o simulation.cpp -o simulation.o

novelty.o : novelty.cpp
	$(CC) $(CFLAGS) -c objDef.o novelty.cpp -o novelty.o

main : main.cpp
	$(CC) $(CFLAGS) objDef.o main.cpp -o main

simsim : simsim.cpp
	$(CC) $(CFLAGS) objDef.o simulation.o novelty.o simsim.cpp -o simsim

clean:
	rm -f $(TARGET) *.o