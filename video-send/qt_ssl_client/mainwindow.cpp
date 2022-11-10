#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>
#include <QProcess>
#include <QDateTime>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include "csslsocket.h"
#include <QSaveFile>
#include "img2webp.h"
#include "dwebp.h"
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    flg = false ;


}

void MainWindow::createSocket()
{
    socket = new CSslSocket(this);
    socket->addCaCertificates(SERVER_CA_PEM);
    socket->setPeerVerifyMode(QSslSocket::VerifyNone);
    socket->ignoreSslErrors({QSslError::SelfSignedCertificate});

    connect(socket, SIGNAL(connected()),                                this,   SLOT(connectedSlot()));
    connect(socket, SIGNAL(disconnected()),                             this,   SLOT(disconnectedSlot()));
    connect(socket, SIGNAL(encrypted()),                                this,   SLOT(encryptedSlot()));
    connect(socket, SIGNAL(readyRead()),                                this,   SLOT(onReceiveMessage()), Qt::DirectConnection);
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this,   SLOT(stateChangedSlot(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),        this,   SLOT(errorSlot(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(sslErrors(const QList<QSslError> &)),        this,   SLOT(sslErrorsSlot(const QList<QSslError> &)));

    ui->statusbar->showMessage("Unconnected");
}

MainWindow::~MainWindow()
{
    delete ui;
}
int decodeWEBP_wrapper(QByteArray in)
{
    QString shell = "webp,in,-o,/tmp/out";
    QByteArray b = shell.toLocal8Bit();
    char* csv = b.data();
    std::vector<const char*> parts;
    const char* part = strtok(csv, ",");
    while (part) {
        parts.push_back(part);
        part = strtok(nullptr, ",");
    }
    return decodeWEBP(parts.size(), parts.data(),in);
}
void MainWindow::on_button_connect_clicked()
{
    createSocket();

    socket->connectToHostEncrypted(ui->ip->text(), ui->port->text().toUInt());

    if (socket->waitForEncrypted()) {
        socket->write("Authentication Suceeded");
    }
    else {
        qDebug("Unable to connect to server");
    }

}

void MainWindow::connectedSlot()
{
    ui->log->append("Connected successfully [socket descriptor: " + QString::number(socket->socketDescriptor()) + "]");
    ui->statusbar->showMessage("Connected");
}

void MainWindow::disconnectedSlot()
{
    ui->log->append("Disconnected");
    ui->statusbar->showMessage("Disconnected");
}

void MainWindow::errorSlot(QAbstractSocket::SocketError e)
{
    ui->log->append("Error [" + QString::number(e) + "]: " + this->socket->errorString());
    qDebug() << "Client_Socket_Error:" << e;
}

void MainWindow::stateChangedSlot(QAbstractSocket::SocketState s)
{
    ui->log->append("State: " + QString::number(s));
    qDebug() << "Client_Socket_State:" << s;
}

void MainWindow::encryptedSlot()
{
    ui->log->append("Connection is encrypted");
}

void MainWindow::sslErrorsSlot(const QList<QSslError> &errors)
{

    for(const QSslError& e : errors){
        ui->log->append("SSL Error: " + e.errorString());
    }

    qDebug() << "Client_SSL_Error:" << errors;
}

void MainWindow::on_button_send_clicked()
{
    qDebug() << socket->isEncrypted() << "Encrypted";
    QByteArray byte_array = ui->textEdit->toPlainText().toLatin1();
    qint64 bytes = socket->write((const char*)byte_array);
    socket->flush();

    qDebug() << "Message sent : " << bytes << "bytes";
}

void MainWindow::onReceiveMessage()
{
    QByteArray array ;
    array = socket->readAll();
    if(array.contains("SS")){
        flg = true;
    }
    if(flg){
        if(array.contains("End")){
            results.push_back(array);
            results.remove(0,2);
            results.remove(results.indexOf("End"),3);
            //QByteArray* in = new QByteArray();
            //in = (QByteArray*)results.data();
            Decode(results);

           // QByteArray buffer = qUncompress(results);
            //if(buffer) results.clear(); return;
           QPixmap p;
           p.load("/tmp/out");
            ui->server_screen->setPixmap(p.scaled(559,366));
            results.clear();
            flg = false;
            return;
        }
        results.push_back(array);
    }
}
void MainWindow::Decode(QByteArray in){
    decodeWEBP_wrapper(in);
}
void MainWindow::on_edt_receive_data_textChanged()
{

}

void MainWindow::on_pushButton_clicked()
{

    //QFile sourceFile("test.jpeg");
    //sourceFile.open(QIODevice::ReadOnly );
   // QByteArray base64Encoded  = sourceFile.readAll().toBase64();
      QByteArray base64Encoded  = ui->edt_receive_data->toPlainText().toUtf8();
   // ui->edt_receive_data->setText(QByteArray::fromBase64(base64Encoded));
   // const char* b64Enc = QString::fromLatin1(base64Encoded).toStdString().c_str();

 //   qDebug() << QByteArray::fromBase64(base64Encoded);


    QSaveFile file("screen.jpeg");
    file.open(QIODevice::WriteOnly);
    file.write(  QByteArray::fromBase64(base64Encoded) );
    // Calling commit() is mandatory, otherwise nothing will be written.
    file.commit();
}
