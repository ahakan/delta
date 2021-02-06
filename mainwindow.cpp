#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Delta");
//    this->showMaximized();
//    this->setFixedSize(this->width(), this->height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *event) {
    qDebug() << "Deneme";

    if (event->type() == QEvent::WindowStateChange) {
        ui->imageLabel->resize(pix.width(),(pix.height()));
        ui->scrollArea->resize(pix.width(),(pix.height()));
        this->resize(pix.width(),(pix.height()));
    }
}


void MainWindow::on_actionOpen_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, "Open Image", ".", "Image Files(*.png *.jpg *.jpeg)");

    qDebug() << "File: " << fileName;
    this->setWindowTitle(fileName + " - Delta");
    pix=fileName;

    ui->imageLabel->setPixmap(pix);
    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setScaledContents(true);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->imageLabel->resize(pix.width(),(pix.height()));
    ui->scrollArea->resize(pix.width(),(pix.height()));
    this->resize(pix.width(),(pix.height()));

    ui->horizontalSlider->setSliderPosition(50);

}

void MainWindow::on_OpenImageButton_clicked()
{
    on_actionOpen_triggered();
}

void MainWindow::on_SaveImageButton_clicked()
{
    if(!pix.isNull())
    {
        pix.save(fileName);

    }
    else
    {
        ui->InfoLabel->setText("Lütfen önce resim seçiniz.");
        ui->InfoLabel->setStyleSheet("font: 100 10pt 'Roboto';");
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if(!pix.isNull())
    {
        openCVImage = cv::imread(fileName.toStdString());

        Mat new_image = Mat::zeros( openCVImage.size(), openCVImage.type() );
        int beta = value-50;       /*< Simple brightness control */

        ui->BrightnessLabel->setText("Brightness ("+QString::number(beta)+")");
        openCVImage.convertTo(new_image, -1, 1, beta);

        updateCanvasLabel(new_image);
    }
    else
    {
        ui->InfoLabel->setText("Lütfen önce resim seçiniz.");
        ui->InfoLabel->setStyleSheet("font: 100 10pt 'Roboto';");
    }
}

void MainWindow::updateCanvasLabel(Mat mat){

    QPixmap pixmap;
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        pixmap = QPixmap::fromImage(img);
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        pixmap = QPixmap::fromImage(img.rgbSwapped());
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage or QPixmap.";
        return;
    }

    ui->imageLabel->setPixmap(pixmap);
    ui->imageLabel->setScaledContents(true);
}
