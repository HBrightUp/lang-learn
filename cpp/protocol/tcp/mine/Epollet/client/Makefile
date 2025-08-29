CC	:= g++
OBJECTS	:= main.o EpollClient.o
CFLAGS	:= -g
TARGET	:= client.bin

$(TARGET):$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

main.o:main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

EpollClient.o:EpollClient.cpp
	$(CC) $(CFLAGS) -c EpollClient.cpp -o EpollClient.o 

.PHONY:clean


clean:
	-rm -rf *.o ./$(TARGET)
