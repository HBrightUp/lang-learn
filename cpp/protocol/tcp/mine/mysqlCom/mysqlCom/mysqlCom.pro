TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    CEncapMysql.cpp \
    MysqlCom.cpp

HEADERS += \
    CEncapMysql.h \
    MysqlCom.h

LIBS +=-lmysqlcppconn -lmysqlcppconn-static


