#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Account{
    QString username;
    QString name;
    QString password;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readFromFile(QString fileName);
    //void writeToFile(QString fileName);
    //QList<Account> readAccountsFromFile(const QString& fileName);
    Account account;
    void writeToFile(QList<QByteArray>, QString fileName);
public slots:
    void onNewConnected();
    void onReadyRead();
    void onDisconnected();
    void login_Correction(QByteArray data,QTcpSocket *client);
private:
    QString fileName="C:/Users/My/Desktop/QT_/ChatApp_v1/ChatApp_Server/ChatApp_v1_Server/Accounts.txt";
    Ui::MainWindow *ui;
    QTcpServer server;
    QTcpSocket *clients;
    QList<QTcpSocket*> sockets;
    QList<Account> accountList;
    int connectedClients=0;
};
#endif // MAINWINDOW_H
