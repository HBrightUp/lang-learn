TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    epoll/EpollServer.cpp \
    public/PublicFun.cpp \
    protobuf/chat.pb.cc \
    mysql/MysqlCom.cpp \
    log/LogFile.cpp \
    mysql/EncapMysql.cpp \
    msg/MsgManage.cpp \
    usr/UserManage.cpp \
    main/main.cpp \
    public/DateTime.cpp \
    public/ServerConfig.cpp \
    epoll/MsgDispose.cpp



DISTFILES += \
    Mars.config \
    Mars.files \
    Mars.includes \
    Mars.creator.user \
    MarsServer.pro.user \
    Mars.creator \
    Makefile \
    ../msg/



HEADERS += \
    public/PublicFun.h \
    protobuf/chat.pb.h \
    mysql/MysqlCom.h \
    log/LogFile.h \
    msg/MsgManage.h \
    mysql/EncapMysql.h \
    usr/UserManage.h \
    epoll/EpollServer.h \
    public/DateTime.h \
    public/ServerConfig.h \
    epoll/MsgDispose.h \
    epoll/locker.h \
    epoll/threadpool.h \
    msg/MsgInfo.h \
    public/errorcode.h




INCLUDEPATH +=/usr/local/protobuf/include  ../msg/


LIBS    +=-lpthread -lprotobuf -lmysqlcppconn -lmysqlcppconn-static


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/protobuf/lib/release/ -lprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/protobuf/lib/debug/ -lprotobuf
else:unix: LIBS += -L$$PWD/../../../../../usr/local/protobuf/lib/ -lprotobuf

INCLUDEPATH += $$PWD/../../../../../usr/local/protobuf/lib
DEPENDPATH += $$PWD/../../../../../usr/local/protobuf/lib
