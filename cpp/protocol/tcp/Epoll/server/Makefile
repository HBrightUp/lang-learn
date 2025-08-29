CC	:= g++
TARGET	:= server.bin
OBJECTS	:= main.o EpollServer.o
CFLAGS	:= -g

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) -lpthread

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

EpollServer.o: EpollServer.cpp
	$(CC) $(CFLAGS) -c EpollServer.cpp -o EpollServer.o


.PHONY:clean

clean:
	-rm -rf *.o ./$(TARGET)
