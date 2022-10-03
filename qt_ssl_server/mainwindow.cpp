#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include <QDebug>
//QString src = "Hello";
//src.toUtf8().toBase64();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->client_table->insertColumn(0);
    ui->client_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->client_table->setHorizontalHeaderLabels(QStringList() << "Client information");

    startServer();
    initConnects();


    //QString base64_encode(QString string);

    //https://stackoverflow.com/questions/15926359/encoding-file-as-base64-string-in-qt-with-xmlrpc
    QFile sourceFile("test.jpeg");
    sourceFile.open(QIODevice::ReadOnly);
    QByteArray base64Encoded  = sourceFile.readAll().toBase64();

    const char* b64Enc = QString::fromLatin1(base64Encoded).toStdString().c_str();
 ui->edt_send_data->setText(b64Enc);
 //   qDebug() << QByteArray::fromBase64(base64Encoded);


    //xmlrpc_c::paramList sampleAddParms;
    //sampleAddParms.add(xmlrpc_c::value_string("113957571"));
    //sampleAddParms.add(xmlrpc_c::value_string(b64Enc));
    //sampleAddParms.add(xmlrpc_c::value_string("test.jpg"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startServer()
{
    m_server = new SSLServer();
    if(m_server->listen(QHostAddress(ui->ip->text()),ui->port->text().toUInt())){
        ui->log->append("Server listening on " + ui->ip->text() + ":" + ui->port->text());
    }
    else{
        ui->log->append("Server has failed to start.");
    }
}

void MainWindow::stopServer()
{
    m_server->close();
    if(!m_server->isListening()){
        ui->log->append("Server is stopped");
    }
    else{
        ui->log->append("Server has failed to stop.");
    }
}

void MainWindow::initConnects()
{
    connect(m_server,           SIGNAL(appendToLog(const QString&)),    this,       SLOT(appendToLogSlot(const QString&)));
    connect(m_server,           SIGNAL(receiveString(QString)),         this,       SLOT(onClientData(QString)));
    connect(m_server,           SIGNAL(updateClientList(const QVector<QSslSocket*>&)),
            this,               SLOT(updateClientListSlot(const QVector<QSslSocket*>&)));
    connect(this,               SIGNAL(sendMessage(QString)),           m_server,   SLOT(onSendMessage(QString)));
    connect(ui->btn_send,       SIGNAL(clicked(bool)),                  this,       SLOT(onSendClicked(bool)));
}

void MainWindow::on_button_start_server_clicked()
{
    startServer();
}

void MainWindow::on_button_stop_server_clicked()
{
    stopServer();
}

//! [ Slots ]

void MainWindow::appendToLogSlot(const QString & str)
{
    ui->log->append(str);
}

void MainWindow::updateClientListSlot(const QVector<QSslSocket*>& connectedSockets)
{
    ui->client_table->clearContents();
    ui->client_table->setRowCount(0);

    for(QSslSocket* const &  s : connectedSockets){
        ui->client_table->insertRow(ui->client_table->rowCount());

       ui->client_table->setItem(ui->client_table->rowCount()-1,0, new QTableWidgetItem(s->peerAddress().toString() + ":" + QString::number(s->peerPort()) + " [" + QString::number(s->socketDescriptor()) + "]"));
      //ui->client_table->setItem(ui->client_table->rowCount()-1,0, new QTableWidgetItem("ahoj"));

    }
}

void MainWindow::onClientData(QString data)
{
    ui->client_data->append(data);
}

void MainWindow::onSendClicked(bool clicked)
{
    Q_UNUSED(clicked);

    QString msg_str = ui->edt_send_data->toPlainText();
    emit sendMessage(msg_str);
}
//! [ Slots ]
