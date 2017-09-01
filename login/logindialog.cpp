#include "logindialog.h"
#include "ui_logindialog.h"
#include "Qmessagebox.h"
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    ui->usrLineEdit->setPlaceholderText("input user name");
    ui->pwdLineEdit->setPlaceholderText("input password");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtn_clicked()
{
    if(ui->usrLineEdit->text() ==tr("felix")
            &&ui->pwdLineEdit->text() == tr("123456"))
    {
        accept();
    }else{
        QMessageBox::warning(this,tr("waring"),
                             tr("invalid user or pwd"),
                             QMessageBox::Yes);
        // 清空内容并定位光标
        ui->usrLineEdit->clear();
        ui->pwdLineEdit->clear();
        ui->usrLineEdit->setFocus();
    }
}
