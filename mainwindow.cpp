#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setScaledContents(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectFile() {
    qDebug() << "selectFile";
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("open_image"), "/Users/paipeng/Documents", tr("image_file_format"));
    qDebug() << "selected file: " << fileName;

    QImage image = QImage(fileName);
    QPixmap pixmap = QPixmap::fromImage(image);

    int w = ui->imageLabel->width();
    int h = ui->imageLabel->height();

    ui->imageLabel->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));

    h = 200;
    //w = image.width() * h / image.height();

    QString filepath("test.webp");
    //cpWebP.write(image, filepath);
    cpWebP.save(image.scaledToHeight(h, Qt::SmoothTransformation), filepath, 4096/2);


    QImage readImage;
    cpWebP.read(filepath, &readImage);
    qDebug() << "read image size: " << readImage.width() << "-" << readImage.height();
}

