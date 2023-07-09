#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "secdialog.h"
#include <QMainWindow>
#include <QTextStream>
#include <QFile>
#include<QString>
#include<QTcpServer>
#include<QTcpSocket>
#include <QByteArray>
#include "secdialog.h"
#include "ui_secdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getUsername();
public slots:
    void onReadyRead();
private slots:
    void on_pushButton_login_clicked();
    void on_register_button_clicked();

private:
    QString username;
    QString password;
    Ui::MainWindow *ui;
    QTcpSocket socket;
    QByteArray socketData;
    Ui::SecDialog *secDialog_ui;
    //SecDialog *secDialog;
};
#endif  // MAINWINDOW_H
