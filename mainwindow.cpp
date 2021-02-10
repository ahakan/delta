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
    ui->imageLabel->resize( pix.width()/3, pix.height()/3 );
}

void MainWindow::showPixmap(QPixmap image)
{
    ui->imageLabel->setPixmap(image);

    if(image.width() >= 1920 || image.height() >= 1080)
    {
        this->showMaximized();
        ui->imageLabel->resize( this->width()-166, this->height()-56);
        ui->scrollArea->resize( this->width()-166, this->height()-56);
        ui->scrollAreaWidgetContents->resize( this->width()-166, this->height()-56);
    }
    else
    {
        this->showMaximized();
    }

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

        std::cerr << "Image directory: " + base.toStdString() << std::endl ;

        QDir directory(base);

        allImages = directory.entryList(QStringList() << "*.png" << "*.jpg" << "*.jpeg", QDir::Files);

        allImagesSize = allImages.length();

        std::cerr << "Total images: " + std::to_string(allImagesSize) << std::endl ;

        if(allImagesSize!=1)
        {
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
                // std::cerr << filename.toStdString() << std::endl ;
            }
            ui->previousButton->setEnabled(true);
            ui->nextButton->setEnabled(true);
            std::cerr << "Current images: " + std::to_string(imgQue) << std::endl ;
            // std::cerr << allImages[imgQue-1].toStdString() << std::endl ;
        }
        else
        {
            ui->previousButton->setDisabled(true);
            ui->nextButton->setDisabled(true);
        }
    }
}

void MainWindow::resetAllWidget()
{
    ui->brightnessSlider->setSliderPosition(50);
    ui->BrightnessLabel->setText("Brightness");
    ui->ContrastSlider->setSliderPosition(100);
    ui->ContrastLabel->setText("Contrast");
    ui->ZoomSlider->setSliderPosition(50);
    ui->ZoomLabel->setText("Zoom");
    ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    // clear mat image
    new_image.release();
}

void MainWindow::on_actionOpen_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, "Open Image", ".", "Image Files(*.png *.jpg *.jpeg)");

    // std::cerr << fileName.toStdString() << std::endl ;

    getImageDirFiles(fileName);

    this->setWindowTitle(fileName + " - Delta");

    pix=fileName;

    if(!pix.isNull())
    {
        showPixmap(pix);

        resetAllWidget();
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
    QPixmap pixmap;
    if(!new_image.empty())
    {
        pixmap = mat2Pixmap(new_image);
    }
    else
    {
        pixmap = fileName;
    }

    if(!pix.isNull())
    {
        saveAsFileName = QFileDialog::getSaveFileName(this,
               tr("Save Address Book"), "",
               tr("JPEG Image(*.jpg);;PNG Image(*.png);;"));

        if(!saveAsFileName.isEmpty() && !saveAsFileName.isNull())
        {
            pixmap.save(saveAsFileName);
            showStatusBarMessage("Kaydedildi.");
            // new-search after save as
            getImageDirFiles(fileName);
        }
        else
        {
            showStatusBarMessage("Vazgeçildi.");
        }

    }
    else
    {
        showStatusBarMessage("Lütfen önce resim seçiniz.");
    }
}


void MainWindow::on_brightnessSlider_valueChanged(int value)
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
        scaleFactor *= value/100.0;
        ui->imageLabel->resize(scaleFactor * ui->imageLabel->pixmap(Qt::ReturnByValue).size());
        ui->ZoomLabel->setText("Zoom ("+QString::number(scaleFactor*2)+") ");
        ui->scrollAreaWidgetContents->setMinimumSize(scaleFactor * ui->imageLabel->pixmap(Qt::ReturnByValue).size());
        ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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
        fileName=nextImage;
        resetAllWidget();

        std::cerr << nextImage.toStdString() << std::endl ;
    }
    else
    {
        imgQue = 0;

        QString nextImage = base + "/" + allImages[imgQue];
        this->setWindowTitle(nextImage + " - Delta");

        showPixmap(nextImage);
        fileName=nextImage;
        resetAllWidget();

        std::cerr << nextImage.toStdString() << std::endl ;
    }
}

void MainWindow::on_previousButton_clicked()
{
    if(imgQue > 0)
    {
        imgQue--;
        std::cerr << imgQue << std::endl ;
        QString nextImage = base + "/" + allImages[imgQue];
        this->setWindowTitle(nextImage + " - Delta");

        showPixmap(nextImage);
        fileName=nextImage;
        resetAllWidget();

        std::cerr << nextImage.toStdString() << std::endl ;
    }
    else
    {
        imgQue = allImagesSize-1;

        QString nextImage = base + "/" + allImages[imgQue];
        this->setWindowTitle(nextImage + " - Delta");

        showPixmap(nextImage);
        fileName=nextImage;
        resetAllWidget();

        std::cerr << nextImage.toStdString() << std::endl ;
    }
}
