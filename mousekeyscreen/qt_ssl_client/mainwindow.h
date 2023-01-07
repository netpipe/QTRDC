#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QSslError>
#include <QPixmap>
#include <QDebug>
#include "define.h"
#include <QMouseEvent>
#include <QKeyEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class CSslSocket;
class MainWindow : public QMainWindow
{
    Q_OBJECT

    void run();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool flg;
    QByteArray results;
    void createSocket();
    void mousePressEvent(QMouseEvent *event);

    int pic_w,pic_h;
private slots:
    void on_button_connect_clicked();
    void connectedSlot();
    void disconnectedSlot();
    void errorSlot(QAbstractSocket::SocketError);
    void stateChangedSlot(QAbstractSocket::SocketState s);
    void encryptedSlot();
    void sslErrorsSlot(const QList<QSslError> &errors);

    void on_button_send_clicked();

    void onReceiveMessage();

    void on_edt_receive_data_textChanged();

    void on_pushButton_clicked();
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    //virtual void keyPressEvent(QKeyEvent* event);
private:

    Ui::MainWindow *ui;
    CSslSocket* socket;
};
#endif // MAINWINDOW_H
