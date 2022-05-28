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
    ui->targetSizeLineEdit->setText("4096");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectFile() {
    qDebug() << "selectFile";
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("open_image"), "/Users/paipeng/Documents", tr("image_file_format"));
    ui->fileLineEdit->setText(fileName);
    qDebug() << "selected file: " << fileName;

    this->image = QImage(fileName);
    QPixmap pixmap = QPixmap::fromImage(this->image);

    int w = ui->imageLabel->width();
    int h = ui->imageLabel->height();

    ui->imageLabel->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));

    encode();
}

void MainWindow::encode() {
    qDebug() << "encode: " << ui->targetSizeLineEdit->text();

    int target_size = ui->targetSizeLineEdit->text().toInt();

    int h = ui->webpLabel->height();
    //w = image.width() * h / image.height();

    QString filepath("test.webp");
    //cpWebP.write(image, filepath);

    if (ui->grayCheckBox->isChecked()) {
        QImage img(this->image);
        for (int i = 0; i < img.width(); ++i) {
            for (int j = 0; j < img.height(); j++) {
                int gray =  qGray(img.pixel(i, j));
                img.setPixel(i, j, QColor(gray, gray, gray).rgb());
            }
        }
        cpWebP.save(img.scaledToHeight(h, Qt::SmoothTransformation), filepath, target_size);
    } else {
        cpWebP.save(this->image.scaledToHeight(h, Qt::SmoothTransformation), filepath, target_size);
    }

    QImage readImage;
    cpWebP.read(filepath, &readImage);
    qDebug() << "read image size: " << readImage.width() << "-" << readImage.height();
    ui->webpLabel->setPixmap(QPixmap::fromImage(readImage));
}

