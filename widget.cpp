#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connection();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::connection(){
    ui->pbDisconnect->setEnabled(tcpsocket_.state() || sslsocket_.state());
    ui->pbClear->setEnabled(tcpsocket_.state() || sslsocket_.state());
    ui->pbSend->setEnabled(tcpsocket_.state() || sslsocket_.state());
}

void Widget::doDisconnected(){
    ui->pteMessage->insertPlainText("Disconnected\n");
}

void Widget::doConnected(){
    ui->pteMessage->insertPlainText("Connected\n");
}

void Widget::doReadyRead(){
    if (tcpsocket_.state()){
        ui->pteMessage->insertPlainText(tcpsocket_.readAll());
    }
    else if (sslsocket_.state()){
        ui->pteMessage->insertPlainText(sslsocket_.readAll());
    }
}

void Widget::on_pbConnect_clicked()
{
    if (ui->cbTCP->isChecked()){
        tcpsocket_.connectToHost(ui->leHost->text(),80);
        QObject::connect(&tcpsocket_, &QAbstractSocket::connected, this, &Widget::doConnected);
        QObject::connect(&tcpsocket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
        QObject::connect(&tcpsocket_, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);
        ui->cbSSL->setEnabled(tcpsocket_.state()==0);
    }
    else if (ui->cbSSL->isChecked()){
        sslsocket_.connectToHostEncrypted(ui->leHost->text(),443);
        QObject::connect(&sslsocket_, &QAbstractSocket::connected, this, &Widget::doConnected);
        QObject::connect(&sslsocket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
        QObject::connect(&sslsocket_, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);
        ui->cbTCP->setEnabled(sslsocket_.state()==0);
    }
    connection();
}

void Widget::on_pbDisconnect_clicked()
{
    if (ui->cbTCP->isChecked())
    {
        tcpsocket_.close();
        ui->cbSSL->setEnabled(tcpsocket_.state()==0);
        connection();
    }
    else if (ui->cbSSL->isChecked())
    {
        sslsocket_.close();
        ui->cbTCP->setEnabled(sslsocket_.state()==0);
        connection();
    }
}


void Widget::on_pbSend_clicked()
{
    if (ui->cbTCP->isChecked()){
        tcpsocket_.write(ui->pteSend->toPlainText().toUtf8());
    }
    else if (ui->cbSSL->isChecked()){
        sslsocket_.write(ui->pteSend->toPlainText().toUtf8());
    }
}

void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
}

