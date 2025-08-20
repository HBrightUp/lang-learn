#ifndef UIMANAGE_H
#define UIMANAGE_H
#include<QScopedPointer>
#include<QObject>
#include"login.h"
#include"player.h"

class UiManage:public QObject
{
    public:
        UiManage();

        void start();

    private slots:
        void login_message_rev(int, QString);

    private:

        QScopedPointer<Login> login_;
        QScopedPointer<Player> player_;

};

#endif // UIMANAGE_H
