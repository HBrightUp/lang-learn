#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void login_send_message(int, QString);

private slots:
    void on_btn_login_clicked();



private:
    Ui::Login *ui;
};

#endif // LOGIN_H
