CC	:= g++
OBJECTS	:= main.o CEncapMysql.o MysqlCom.o
CFLAGS	:= -g 
TARGET	:= main.bin
MysqlLib :=-lmysqlcppconn -lmysqlcppconn-static



$(TARGET):$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(MysqlLib)

main.o:main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o 

CEncapMysql.o:CEncapMysql.cpp
	$(CC) $(CFLAGS) -c CEncapMysql.cpp -o CEncapMysql.o 

MysqlCom.o:MysqlCom.cpp
	$(CC) $(CFLAGS) -c MysqlCom.cpp -o MysqlCom.o

.PHONY:clean


clean:
	-rm -rf *.o ./$(TARGET)
