CC	:= g++
CFLAGS	:= -g

OBJECTPATH 	:= ./bin/

TARGET	:= $(OBJECTPATH)server.bin
OBJECTS	:= main.o EpollServer.o MsgManage.o PublicFun.o chat.pb.o login.pb.o MsgKeeplive.o \
		MsgPush.o SendMsgReq.o MsgLogin.o EncapMysql.o MysqlCom.o UserManage.o LogFile.o

OBJECTS_O	:= $(OBJECTPATH)main.o $(OBJECTPATH)EpollServer.o $(OBJECTPATH)MsgManage.o $(OBJECTPATH)PublicFun.o $(OBJECTPATH)chat.pb.o $(OBJECTPATH)login.pb.o $(OBJECTPATH)MsgKeeplive.o \
		$(OBJECTPATH)MsgPush.o $(OBJECTPATH)SendMsgReq.o $(OBJECTPATH)MsgLogin.o $(OBJECTPATH)EncapMysql.o $(OBJECTPATH)MysqlCom.o $(OBJECTPATH)UserManage.o $(OBJECTPATH)LogFile.o
	
LIB_PROTOBUF	:= -L/usr/local/protobuf/lib
FLAG_MYSQL	:=-lmysqlcppconn -lmysqlcppconn-static
OTHERS	:= -lprotobuf -lpthread

INCLUDE	:= -I/usr/local/protobuf/include -I./protobuf -I./epoll -I./log -I./msg -I./mysql -I./public -I./usr





$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS_O) -o $(TARGET) $(INCLUDE) $(LIB_PROTOBUF) $(FLAG_MYSQL) $(OTHERS) 

main.o: ./main/main.cpp
	$(CC) $(CFLAGS) -c ./main/main.cpp -o $(OBJECTPATH)/main.o $(INCLUDE)

EpollServer.o: ./epoll/EpollServer.cpp 
	$(CC) $(CFLAGS) -c ./epoll/EpollServer.cpp -o $(OBJECTPATH)/EpollServer.o $(INCLUDE) 

MsgManage.o:./msg/MsgManage.cpp
	$(CC) $(CFLAGS) -c ./msg/MsgManage.cpp -o $(OBJECTPATH)/MsgManage.o  $(INCLUDE)  

PublicFun.o:./public/PublicFun.cpp
	$(CC) $(CFLAGS) -c ./public/PublicFun.cpp -o $(OBJECTPATH)/PublicFun.o  $(INCLUDE)

MsgKeeplive.o:./msg/MsgKeeplive.cpp
	$(CC) $(CFLAGS) -c ./msg/MsgKeeplive.cpp -o  $(OBJECTPATH)/MsgKeeplive.o

chat.pb.o: ./protobuf/chat.pb.cc
	$(CC) $(CFLAGS) -c ./protobuf/chat.pb.cc -o  $(OBJECTPATH)/chat.pb.o $(INCLUDE)

login.pb.o: ./protobuf/login.pb.cc
	$(CC) $(CFLAGS) -c ./protobuf/login.pb.cc -o $(OBJECTPATH)/login.pb.o $(INCLUDE)

MsgPush.o: ./msg/MsgPush.cpp
	$(CC) $(CFLAGS) -c ./msg/MsgPush.cpp -o $(OBJECTPATH)/MsgPush.o $(INCLUDE)

SendMsgReq.o: ./msg/SendMsgReq.cpp
	$(CC) $(CFLAGS) -c ./msg/SendMsgReq.cpp -o $(OBJECTPATH)/SendMsgReq.o  $(INCLUDE)

MsgLogin.o: ./msg/MsgLogin.cpp
	$(CC) $(CFLAGS) -c ./msg/MsgLogin.cpp -o $(OBJECTPATH)/MsgLogin.o $(INCLUDE)

EncapMysql.o: ./mysql/EncapMysql.cpp
	$(CC) $(CFLAGS) -c ./mysql/EncapMysql.cpp -o $(OBJECTPATH)/EncapMysql.o $(INCLUDE)

MysqlCom.o: ./mysql/MysqlCom.cpp
	$(CC) $(CFLAGS) -c ./mysql/MysqlCom.cpp -o $(OBJECTPATH)/MysqlCom.o $(INCLUDE)

UserManage.o: ./usr/UserManage.cpp
	$(CC) $(CFLAGS) -c ./usr/UserManage.cpp -o $(OBJECTPATH)/UserManage.o

LogFile.o: ./log/LogFile.cpp
	$(CC) $(CFLAGS) -c ./log/LogFile.cpp -o $(OBJECTPATH)/LogFile.o

.PHONY:clean

clean:
	-rm -rf $(OBJECTPATH)*
