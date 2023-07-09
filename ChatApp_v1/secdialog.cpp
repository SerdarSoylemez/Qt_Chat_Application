#include "secdialog.h"
#include "ui_secdialog.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include<QTcpSocket>
#include<QTcpServer>
#include<QString>
SecDialog::SecDialog(QString username,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecDialog)

{
    ui->setupUi(this);
    ui->users_text->setText(username);
    setUsername(username);

    socket.connectToHost(QHostAddress("127.0.0.1"),8080);
    if (socket.waitForConnected()) {
        qDebug() << "Connected to host";
    } else {
        qDebug() << "Failed to connect to host";
    }
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOperationForClock()));
    connect(timer,SIGNAL(timeout()),this,SLOT(datetimeOperationForClock()));
    timer -> start();
    connect(&socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    //qDebug()<<"Mainwindow_username_in_secDialog: "<<username_data.getUsername();
}

SecDialog::~SecDialog()
{
    delete ui;
}

void SecDialog::timeOperationForClock()
{
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh:mm:ss");
    ui -> clock_label_2 ->setText(time_text);
}

void SecDialog::datetimeOperationForClock()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeText = dateTime.toString("dd.MM.yyyy");
    ui -> dateTime_label_2 -> setText(dateTimeText);
}

void SecDialog::on_sendButton_clicked()
{
    //sending message with TCP

    QString message = ui -> messageBox -> toPlainText();
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh.mm.ss");
    qDebug()<<"1:";
    int messageLenght = message.length() + 8 ;
    int blank = abs(200 - messageLenght);
    QString mesBlank = " ";
    for (int i = 1; i <= blank - 1 ; i++)
    {
        if(message.length() > 200)
        {
            // Special Sit.
        }
        mesBlank = mesBlank + " ";
    }
    QString newMessage =username +": "+message + mesBlank+time_text ;
    //ui->messageBrowser->insertPlainText();
    ui -> messageBrowser -> insertPlainText(newMessage);
    ui -> messageBox -> clear() ;
    qDebug()<<"3:";
    QString newLine = "\n" ;
    ui -> messageBrowser -> insertPlainText(newLine);
    qDebug()<<"4:";
    QByteArray data2server;
    QString server_text="server";
    data2server.append(server_text+":"+username +":"+message+":"+time_text);
    qDebug()<<"5:";
    if(socket.isWritable()){
        socket.write(data2server);
        qDebug()<<" Yazdırabilir";
    }
    else{
        qDebug()<<"Yazdırılamaz";
    }
    qDebug()<<"6:";
    //ui->users_text->setText(socketData);
}
void SecDialog::onReadyRead(){
    QByteArray data = socket.readAll();
    QDataStream in(&data, QIODevice::ReadOnly);
    QString receivedData;
    in >> receivedData;
    //ui->users_text->setText(receivedData);
}
void SecDialog::setUsername(QString username1){
    username=username1;
}
