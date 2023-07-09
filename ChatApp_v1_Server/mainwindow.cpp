#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPair>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),server(this)
{
    ui->setupUi(this);
    readFromFile(fileName);
    server.listen(QHostAddress("127.0.0.1"),8080);
    connect(&server,SIGNAL(newConnection()),this,SLOT(onNewConnected()));
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onNewConnected(){
    QTcpSocket *clientSocket=server.nextPendingConnection();
    if(clientSocket){
        qDebug() << "New connection. Total connected clients: " << connectedClients + 1;
        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(clientSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
        sockets.push_back(clientSocket);
        connectedClients++;
    }
}
void MainWindow::onReadyRead(){
    QTcpSocket *client = (QTcpSocket*)sender();
    QByteArray data = client->readAll(); // Gelen veriyi oku.
    qDebug() << "Received data: " << data;

    QList<QByteArray> parts = data.split(':');// Gelen veriyi ayrıştır.
    if(parts[0]=="server"){
        QString username = parts[1];
        QString message = parts[2];
        QString time_text=parts[3];
        qDebug() << username << " says: " << message<<"at"<<time_text;
        ui->message_text->append(username+":"+message+"\n"+time_text);
    }
    else if(parts.size() == 3 && parts[0]=="register"){
        qDebug()<<parts[1]<<":"<<parts[2]<<":"<<parts[3];
        writeToFile(parts,fileName);
        readFromFile(fileName);
    }
    else if(parts.size() == 3 && parts[0]=="login"){
        login_Correction(data,client);
    }
    qDebug()<<"parts1:"<<parts[0];
}
void MainWindow::readFromFile(QString fileName){
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Error: Could not open file";
        return;
    }
    QTextStream in(&file);
    //int i=0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList accountInfo = line.split(" ");
        if (accountInfo.size() != 3){
            qDebug() << "Error: Invalid file format";
            return;
        }
        Account account;
        account.username= accountInfo[0];
        account.name=accountInfo[1];
        account.password=accountInfo[2];
        accountList.append(account);
        //qDebug()<<accountList.at(i).username;
        //qDebug()<<accountList.at(i).password;
        //qDebug()<<accountList.at(i).name;
        //i++;
    }
    file.close();
}
void MainWindow::login_Correction(QByteArray data,QTcpSocket *client){
    QStringList list=QString(data).split(":");
    QString username = list[1];
    QString password = list[2];
    bool accountFound=false;
    qDebug()<<"username_server_:"<<username<<" password_server_:"<<password;
    for(int i=0;i<accountList.size();i++){
        qDebug()<<"username_in_for: "<<accountList.at(i).username;
        if(accountList.at(i).username == username && accountList.at(i).password == password)
        {
            QByteArray accountInfo;
            QString correction="Correct";
            accountInfo.append(correction+":"+accountList.at(i).username);
            //accountInfo.append(accountList.at(i).username + ":"+accountList.at(i).name+":"+accountList.at(i).password);
            client->write(accountInfo);
            accountFound=true;
            break;
        }
    }
    if(!accountFound){
        QByteArray accountInfo;
        QString wrong="Wrong";
        accountInfo.append(wrong+":"+username);
        client->write(accountInfo);
    }
}
void MainWindow::writeToFile(QList<QByteArray>parts,QString fileName){

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
            return ;
     QTextStream out(&file);
     out << parts.at(1)<<" ";
     out << parts.at(3)<<" ";
     out << parts.at(2)<<"\n";

     file.close();

}
void MainWindow::onDisconnected(){
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        connectedClients--;
        qDebug() << "Client disconnected. Total connected clients: " << connectedClients;
        clientSocket->deleteLater();
    }
}

