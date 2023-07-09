#ifndef SECDIALOG_H
#define SECDIALOG_H

#include <QDialog>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTcpSocket>
#include<QTcpServer>
#include<QString>
namespace Ui {
class SecDialog;
}
class SecDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecDialog(QString username,QWidget *parent = nullptr);
    ~SecDialog();
    void setUsername(QString username);
public slots :
    void timeOperationForClock();
    void datetimeOperationForClock();
    void onReadyRead();
private slots:
    void on_sendButton_clicked();
private:
    QString username;
    Ui::SecDialog *ui;
    QTimer *timer;
    QTcpSocket socket;
    QByteArray socketData;
    //MainWindow username_data;
};
#endif // SECDIALOG_H

