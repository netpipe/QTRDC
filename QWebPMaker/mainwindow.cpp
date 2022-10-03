#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "img2webp.h"
#include "dwebp.h"
#include <QDebug>
#include <QFileDialog>
#include <QDirIterator>
#include <QFile>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFileInfo>

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstring>
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//static void Help(void) {
//  printf("Usage:\n\n");
//  printf("  img2webp [file-level options] [image files...] "
//         "[per-frame options...]\n");
//  printf("\n");

//  printf("File-level options (only used at the start of compression):\n");
//  printf(" -min_size ............ minimize size\n");
//  printf(" -loop <int> .......... loop count (default: 0, = infinite loop)\n");
//  printf(" -kmax <int> .......... maximum number of frame between key-frames\n"
//         "                        (0=only keyframes)\n");
//  printf(" -kmin <int> .......... minimum number of frame between key-frames\n"
//         "                        (0=disable key-frames altogether)\n");
//  printf(" -mixed ............... use mixed lossy/lossless automatic mode\n");
//  printf(" -v ................... verbose mode\n");
//  printf(" -h ................... this help\n");
//  printf(" -version ............. print version number and exit\n");
//  printf("\n");

//  printf("Per-frame options (only used for subsequent images input):\n");
//  printf(" -d <int> ............. frame duration in ms (default: 100)\n");
//  printf(" -lossless  ........... use lossless mode (default)\n");
//  printf(" -lossy ... ........... use lossy mode\n");
//  printf(" -q <float> ........... quality\n");
//  printf(" -m <int> ............. method to use\n");

//  printf("\n");
//  printf("example: img2webp -loop 2 in0.png -lossy in1.jpg\n"
//         "                  -d 80 in2.tiff -o out.webp\n");
//  printf("\nNote: if a single file name is passed as the argument, the "
//         "arguments will be\n");
//  printf("tokenized from this file. The file name must not start with "
//         "the character '-'.\n");
//}
int img2webp_wrapper(char* csv)
{
std::vector<const char*> parts;
const char* part = strtok(csv, ",");
while (part) {
    parts.push_back(part);
    part = strtok(nullptr, ",");
}
return img2webp(parts.size(), parts.data());
}

int decodeWEBP_wrapper(char* csv)
{
std::vector<const char*> parts;
const char* part = strtok(csv, ",");
while (part) {
    parts.push_back(part);
    part = strtok(nullptr, ",");
}
return decodeWEBP(parts.size(), parts.data());
}

void MainWindow::on_pushButton_clicked()
{

  //QString  fileName= QFileDialog::getSaveFileName(this, "Save image", QCoreApplication::applicationDirPath(), "JPEG (*.jpg);;PNG (*.png)" );
    QString fileName = QFileDialog ::getOpenFileName(0,"Select File","","JPEG (*.jpg);;PNG (*.png)" );

    QFileInfo fileInfo(fileName);
    //QString webpoutfile = QDir(fileInfo.absolutePath()).filePath(fileInfo.baseName()).toLatin1()+".webp";
    //qDebug() << webpoutfile.toLatin1();


    QString fileslist;

    fileslist.append("blank,");
    if (ui->singleLossy->isChecked()){qDebug() << "lossy- smaller files";
        fileslist.append("-lossy,");
    }else {

              fileslist.append("-lossless,");
    }
    fileslist.append("-q,");
    fileslist.append(ui->singlequality->text().toLatin1()+",");

    fileslist.append(fileName.toLatin1()+",");

    fileslist.append("-o,");

    QString  fileName2= QFileDialog::getSaveFileName(this, "Save image", QCoreApplication::applicationDirPath()+"/.webp", "webp (*.webp)" );

    fileslist.append(fileName2.toLatin1()+",");
    //   fileslist.append("blank");

    QByteArray array = fileslist.toLocal8Bit();
    char* buffer = array.data();

    img2webp_wrapper(buffer);

     QImage *img_object = new QImage();
     img_object->load(fileName);
     QPixmap image = QPixmap::fromImage(*img_object);
  //   QPixmap scaled_img = image.scaled(this->width(), this->height(), Qt::KeepAspectRatio);
     QPixmap scaled_img = image.scaled(ui->graphicsView->width(), ui->graphicsView->height(), Qt::KeepAspectRatio);
     QGraphicsScene *scene= new QGraphicsScene();
    // scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
     scene->addPixmap(scaled_img);
     scene->setSceneRect(scaled_img.rect());
     ui->graphicsView->setScene(scene);
     ui->graphicsView->show();

}

void MainWindow::on_batchbutton_clicked()
{
     ui->listWidget->clear();
    QString directory = QFileDialog::getExistingDirectory(0, ("Select Output Folder"), QDir::currentPath());

    QDirIterator it(directory.toLatin1(), QStringList() << "*.jpg", QDir::Files, QDirIterator::Subdirectories);
    QStringList files;
int e=0;
    while (it.hasNext()){
      //  QFileInfo fileInfo(f.fileName());

        files << it.next().toLatin1();
     //   qDebug() <<  it.next().toLatin1() ;
        qDebug() << e;
        e++;
    }
//qDebug() << it.fileName();
   // QStringList fileslist;
    QString fileslist;

    fileslist.append("blank,");
        if (ui->lossycheck->isChecked()){qDebug() << "lossy- smaller files";
    fileslist.append("-lossy,");
    }else {
        fileslist.append("-lossless,");
    }
    fileslist.append("-q,");
    fileslist.append(ui->batchQuality->text().toLatin1()+",");
    fileslist.append("-loop,");
    fileslist.append(ui->loopcount->text().toLatin1() + ",");
    fileslist.append("-d,");
    fileslist.append(ui->loopdelay->text().toLatin1() + ",");


        qDebug() << files.size();
    for (int i=0; i < files.size() ; i++)
    {
        fileslist.append(files.at(i).toLatin1() +",");
        ui->listWidget->addItem(files.at(i).toLatin1());
        qDebug() << files.at(i).toLatin1();
    }
        fileslist.append("-o,");
    QString  fileName2= QFileDialog::getSaveFileName(this, "Save image", QCoreApplication::applicationDirPath()+"/.webp", "webp (*.webp)" );

    fileslist.append(fileName2.toLatin1()+",");
 //   fileslist.append("blank");

qDebug() << fileslist.toLatin1();

QByteArray array = fileslist.toLocal8Bit();
char* buffer = array.data();


qDebug() << "test " << buffer;
img2webp_wrapper(buffer);
}
