#include "uimanage.h"
#include<QDebug>

UiManage::UiManage() {
    login_.reset(new Login);

    connect(login_.get(), &Login::login_send_message, this, &UiManage::login_message_rev);
}

void UiManage::start(){

    login_.get()->show();
}

void UiManage::login_message_rev(int msg_id, QString data){
    qInfo() << msg_id << data;
    login_.reset();

    player_.reset(new Player);
    player_.get()->show();


}
