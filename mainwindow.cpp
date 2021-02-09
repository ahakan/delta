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

    new_image = Mat::zeros( openCVImage.size(), openCVImage.type() );

    this->statusBar()->addPermanentWidget(ui->ZoomLabel);
    this->statusBar()->addPermanentWidget(ui->ZoomSlider);

    std::cerr << "Delta Image Viewer" << std::endl ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QPixmap MainWindow::mat2Pixmap(Mat mat)
{
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

    return pixmap;
}


void MainWindow::updateImageLabel(Mat mat)
{
    QPixmap pixmap = mat2Pixmap(mat);
    ui->imageLabel->setPixmap(pixmap);
    ui->imageLabel->setScaledContents(true);
}

void MainWindow::showPixmap(QPixmap image)
{
    ui->imageLabel->setPixmap(image);
    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->resize(pix.width(),(pix.height()));
}

void MainWindow::showStatusBarMessage(QString message)
{
    this->statusBar()->showMessage(message);
}


void MainWindow::getImageDirFiles(QString fileName){

    if(!fileName.isEmpty()){
        QFileInfo fi(fileName);
        base = fi.absolutePath();

        //std::cerr << fi.fileName().toStdString() << std::endl ;

        std::cerr << base.toStdString() << std::endl ;

        QDir directory(base);

        allImages = directory.entryList(QStringList() << "*.png" << "*.jpg" << "*.jpeg", QDir::Files);

        allImagesSize = allImages.length();

        std::cerr << allImagesSize << std::endl ;

        imgQue = 0;
        foreach(QString filename, allImages) {
            if(fi.fileName()!=filename)
            {
                imgQue++;
            }
            else
            {
                break;
            }
            std::cerr << filename.toStdString() << std::endl ;
        }
        std::cerr << imgQue << std::endl ;
        std::cerr << allImages[imgQue-1].toStdString() << std::endl ;
    }
}

void MainWindow::on_actionOpen_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, "Open Image", ".", "Image Files(*.png *.jpg *.jpeg)");

    std::cerr << fileName.toStdString() << std::endl ;

    getImageDirFiles(fileName);

    this->setWindowTitle(fileName + " - Delta");
    pix=fileName;

    showPixmap(pix);

    if(!pix.isNull())
    {
        ui->scrollArea->setBackgroundRole(QPalette::Dark);

        if(pix.width() >= 1920 || pix.height() >= 1080)
        {
            if(pix.width() >= 1920 && pix.height() >= 1080)
            {
                ui->scrollArea->resize(1920,1080);
                this->resize(1920,1080);
            }
            else if(pix.width() >= 1920)
            {
                ui->scrollArea->resize(1920,pix.height());
                this->resize(1920,pix.height());
            }
            else
            {
                ui->scrollArea->resize(pix.width(),1080);
                this->resize(pix.width(),1080);
            }
        }
        else
        {
            ui->scrollArea->resize(pix.width(),pix.height());
            this->resize(pix.width(),pix.height());
        }

        ui->horizontalSlider->setSliderPosition(50);
        ui->ContrastSlider->setSliderPosition(100);
    }
}

void MainWindow::on_OpenImageButton_clicked()
{
    on_actionOpen_triggered();
}

void MainWindow::on_SaveImageButton_clicked()
{
    QPixmap pixmap = mat2Pixmap(new_image);

    if(!pix.isNull())
    {
        if(!pixmap.isNull())
        {
            pixmap.save(fileName);
            showStatusBarMessage("Kaydedildi.");
        }
        else
        {
            showStatusBarMessage("Resimde bir değişiklik yapılmadı.");
        }

    }
    else
    {
        showStatusBarMessage("Lütfen önce resim seçiniz.");
    }
}


void MainWindow::on_SaveAsImageButton_clicked()
{
    QPixmap pixmap = mat2Pixmap(new_image);

    if(!pix.isNull())
    {

        saveAsFileName = QFileDialog::getSaveFileName(this,
               tr("Save Address Book"), "",
               tr("JPEG Image(*.jpg);;PNG Image(*.png);;"));

        pixmap.save(saveAsFileName);
        showStatusBarMessage("Kaydedildi.");

    }
    else
    {
        showStatusBarMessage("Lütfen önce resim seçiniz.");
    }
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if(!pix.isNull())
    {
        openCVImage = cv::imread(fileName.toStdString());

        int beta = value-50;       /*< Simple brightness control */

        ui->BrightnessLabel->setText("Brightness ("+QString::number(beta)+")");
        openCVImage.convertTo(new_image, -1, 1, beta);

        updateImageLabel(new_image);
    }
    else
    {
        showStatusBarMessage("Lütfen önce resim seçiniz.");
    }
}


void MainWindow::on_ContrastSlider_valueChanged(int value)
{
    if(!pix.isNull())
    {
        openCVImage = cv::imread(fileName.toStdString());

        float alpha = value/100.0;       /*< Simple brightness control */

        ui->ContrastLabel->setText("Contrast ("+QString::number(alpha)+")");
        openCVImage.convertTo(new_image, -1, alpha, 0);

        updateImageLabel(new_image);
    }
    else
    {
        showStatusBarMessage("Lütfen önce resim seçiniz.");
    }
}


void MainWindow::on_ZoomSlider_valueChanged(int value)
{
    if(!pix.isNull())
    {
        double scaleFactor = 1;
        scaleFactor *= value/50.0;
        ui->imageLabel->resize(scaleFactor * ui->imageLabel->pixmap(Qt::ReturnByValue).size());
        ui->ZoomLabel->setText("Zoom ("+QString::number(scaleFactor)+") ");
    }
    else
    {
        showStatusBarMessage("Lütfen önce resim seçiniz.");
    }

}


void MainWindow::on_QuitButton_clicked()
{
    QCoreApplication::quit();
}


void MainWindow::on_actionSave_triggered()
{
    on_SaveImageButton_clicked();
}


void MainWindow::on_actionSave_As_triggered()
{
    on_SaveAsImageButton_clicked();
}


void MainWindow::on_actionQuit_triggered()
{
    on_QuitButton_clicked();
}

void MainWindow::on_nextButton_clicked()
{
    if(imgQue+1 < allImagesSize)
    {
        imgQue++;
        QString nextImage = base + "/" + allImages[imgQue];
        this->setWindowTitle(nextImage + " - Delta");

        showPixmap(nextImage);

        std::cerr << nextImage.toStdString() << std::endl ;
    }
    else
    {
        imgQue = 0;

        QString nextImage = base + "/" + allImages[imgQue];
        this->setWindowTitle(nextImage + " - Delta");

        showPixmap(nextImage);

        std::cerr << nextImage.toStdString() << std::endl ;
    }
}
