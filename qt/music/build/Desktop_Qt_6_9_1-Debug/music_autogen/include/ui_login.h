/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QLineEdit *le_name;
    QLabel *label_2;
    QLineEdit *le_password;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_login;
    QPushButton *btn_register;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(410, 336);
        Login->setMouseTracking(true);
        widget = new QWidget(Login);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(40, 89, 232, 92));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        le_name = new QLineEdit(widget);
        le_name->setObjectName("le_name");
        le_name->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_2->addWidget(le_name, 0, 1, 1, 2);

        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        le_password = new QLineEdit(widget);
        le_password->setObjectName("le_password");
        le_password->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_2->addWidget(le_password, 1, 1, 1, 2);

        horizontalSpacer = new QSpacerItem(58, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 2, 0, 1, 1);

        btn_login = new QPushButton(widget);
        btn_login->setObjectName("btn_login");

        gridLayout_2->addWidget(btn_login, 2, 1, 1, 1);

        btn_register = new QPushButton(widget);
        btn_register->setObjectName("btn_register");

        gridLayout_2->addWidget(btn_register, 2, 2, 1, 1);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Login", "Name", nullptr));
        le_name->setText(QCoreApplication::translate("Login", "hml", nullptr));
        label_2->setText(QCoreApplication::translate("Login", "Pasword", nullptr));
        le_password->setText(QCoreApplication::translate("Login", "123", nullptr));
        btn_login->setText(QCoreApplication::translate("Login", "login", nullptr));
        btn_register->setText(QCoreApplication::translate("Login", "register", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
