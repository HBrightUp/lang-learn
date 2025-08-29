CC	:= g++
TARGET	:= server.bin
OBJECTS	:= main.o EpollServer.o
CFLAGS	:= -g
LIB	:= -L/usr/local/protobuf/lib
INCLUDE	:= -I/usr/local/protobuf/include
OTHERS	:= -lprotobuf -lpthread




$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(INCLUDE) $(LIB) $(OTHERS) 

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

EpollServer.o: EpollServer.cpp 
	$(CC) $(CFLAGS) -c EpollServer.cpp $(INCLUDE) -o EpollServer.o


.PHONY:clean

clean:
	-rm -rf *.o ./$(TARGET)
