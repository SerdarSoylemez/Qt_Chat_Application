#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QList>
#include <QByteArray>
#include "secdialog.h"
#include "ui_secdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("C:/Users/My/Desktop/QT_/ChatApp_v1/ChatApp_v1/pic.png");
    int w = ui -> label_pic -> width();
    int h = ui -> label_pic -> height();
    ui -> label_pic -> setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    socket.connectToHost(QHostAddress("127.0.0.1"),8080);
    connect(&socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onReadyRead(){
    QByteArray newData = socket.readAll();
    qDebug()<<"Data_cl: "<<newData;
    socketData.append(newData);
    QList<QByteArray> parts = socketData.split(':');// Gelen veriyi ayrıştır.
    username=parts[1];
    qDebug()<<"Client_ready_username: "<<username;
    if (socketData.contains("Correct")) {
           hide();
           SecDialog *secDialog = new SecDialog(username, this);
           secDialog->show();
    }
    else{
        qDebug()<<"Wrong login";
        QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Login Failed", "Login information is incorrect. Do you want to try again?", QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                ui->username_lineEdit->clear();
                ui->password_lineEdit->clear();
            }
    }
    //password=parts[2];
    //qDebug()<<"Client_ready_username: "<<username;
    //qDebug()<<"Client_ready_password: "<<password;
}
void MainWindow::on_pushButton_login_clicked()
{
    QString username2 = ui -> username_lineEdit ->text();
    QString password2 = ui -> password_lineEdit ->text();
    qDebug()<<"username_line_edit: "<<username2;//Correct
    qDebug()<<"password_line_edit: "<<password2;//Correct
    QByteArray information;
    QString login="login";
    information.append(login+":"+username2+ ":"+password2);
    socket.write(information);
}
void MainWindow::on_register_button_clicked()
{
    QString username=ui->username_R->text();
    QString name=ui->Name_R->text();
    QString password=ui->password_R->text();
    QByteArray data;
    QString reg="register";
    data.append(reg+":"+username+":"+password+":"+name);
    socket.write(data);

}
QString MainWindow::getUsername(){
    return username;
}




