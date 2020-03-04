CC = g++
CFLAGS = -g -Wall -O

qd-novelty: qd.o setup.o
	$(CC) $(CFLAGS) -o qd-novelty qd.o setup.o 

qd.o: qd.cpp
	$(CC) $(CFLAGS) -c qd.cpp -o qd.o

setup.o: dataSetup.cpp
	$(CC) $(CFLAGS) -c dataSetup.cpp -o setup.o

clean:
	rm -f $(TARGET) *.o