#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "sslserver.h"
#include <QTimer>
//#include "QtWebSockets/QWebSocket"

#include "define.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initConnects();
    void startServer();
    void stopServer();
     QTimer m_timer;
    void createCertificates();

    void Encode(QByteArray* in, QByteArray *out);
    void Decode(QByteArray* in, QByteArray *out);
public slots:
    void onClientData(QString data);
    void onOutTime();
private slots:
    void on_button_start_server_clicked();
    void on_button_stop_server_clicked();
    void appendToLogSlot(const QString&);
    void updateClientListSlot(const QVector<QSslSocket *> &);
    void onSendClicked(bool clicked);
signals:
    void sendMessage(QString message);
private:
    Ui::MainWindow *ui;
    SSLServer* m_server;

};
#endif // MAINWINDOW_H
