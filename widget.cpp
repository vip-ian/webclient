#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pbDisconnect->setEnabled(false);
    ui->pbClear->setEnabled(false);
    ui->pbSend->setEnabled(false);
    // QObject::connect(&tcpsocket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    // QObject::connect(&tcpsocket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    // QObject::connect(&tcpsocket_, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::connection(){
}

void Widget::doDisconnected(){
    ui->pteMessage->insertPlainText("Disconnected\n");
}

void Widget::doConnected(){
    ui->pteMessage->insertPlainText("Connected\n");
}

void Widget::doReadyRead(){
    if (ui->cbTCP->isChecked()){
        ui->pteMessage->insertPlainText(tcpsocket_.readAll());
    }
    else if (ui->cbSSL->isChecked()){
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
        ui->cbSSL->setEnabled(false);
    }
    else if (ui->cbSSL->isChecked()){
        sslsocket_.connectToHostEncrypted(ui->leHost->text(),443);
        QObject::connect(&sslsocket_, &QAbstractSocket::connected, this, &Widget::doConnected);
        QObject::connect(&sslsocket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
        QObject::connect(&sslsocket_, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);
        ui->cbTCP->setEnabled(false);
    }
}

void Widget::on_pbDisconnect_clicked()
{
    if (ui->cbTCP->isChecked())
    {
        tcpsocket_.close();
    }
    else if (ui->cbSSL->isChecked())
    {
        sslsocket_.close();
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
