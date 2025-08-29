TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    epoll/EpollServer.cpp \
    public/PublicFun.cpp \
    protobuf/chat.pb.cc \
    protobuf/login.pb.cc \
    mysql/MysqlCom.cpp \
    log/LogFile.cpp \
    msg/MsgKeeplive.cpp \
    msg/MsgPush.cpp \
    msg/MsgLogin.cpp \
    mysql/EncapMysql.cpp \
    msg/MsgManage.cpp \
    usr/UserManage.cpp \
    main/main.cpp \
    public/DateTime.cpp \
    protobuf/ContactAdd.pb.cc \
    msg/MsgContactReq.cpp \
    msg/MsgContactRsp.cpp \
    public/ServerConfig.cpp \
    msg/MsgChatPrivate.cpp \
    msg/MsgChatGroup.cpp \
    msg/imsgbase.cpp



DISTFILES += \
    Mars.config \
    Mars.files \
    Mars.includes \
    Mars.creator.user \
    MarsServer.pro.user \
    Mars.creator \
    Makefile \
    #/usr/local/protobuf/include\
    ../msg/



HEADERS += \
    public/PublicFun.h \
    msg/IMsgBase.h \
    protobuf/chat.pb.h \
    protobuf/login.pb.h \
    mysql/MysqlCom.h \
    log/LogFile.h \
    msg/MsgManage.h \
    mysql/EncapMysql.h \
    msg/MsgKeeplive.h \
    msg/MsgPush.h \
    msg/MsgLogin.h \
    usr/UserManage.h \
    epoll/EpollServer.h \
    public/DateTime.h \
    protobuf/ContactAdd.pb.h \
    msg/MsgContactReq.h \
    msg/MsgContactRsp.h \
    public/ServerConfig.h \
    msg/MsgChatPrivate.h \
    msg/MsgChatGroup.h




INCLUDEPATH +=/usr/local/protobuf/include  ../msg/


LIBS    +=-lpthread -lprotobuf -lmysqlcppconn -lmysqlcppconn-static


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/protobuf/lib/release/ -lprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/protobuf/lib/debug/ -lprotobuf
else:unix: LIBS += -L$$PWD/../../../../../usr/local/protobuf/lib/ -lprotobuf

INCLUDEPATH += $$PWD/../../../../../usr/local/protobuf/lib
DEPENDPATH += $$PWD/../../../../../usr/local/protobuf/lib
