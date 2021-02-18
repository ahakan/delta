#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include "imageread.h"

#include <QMainWindow>
#include <QDialog>
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QSizePolicy>
#include <QLayout>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPainter>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace cv;
using namespace tesseract;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateImageLabel(Mat mat);

    void showPixmap(QPixmap image);

    QPixmap mat2Pixmap(Mat mat);

    void getImageDirFiles(QString fileName);

    void resetAllWidget();

    void statusBarCheck(bool trueFalse, QString question="");

    void imageInfo(QPixmap image);

    void on_actionOpen_triggered();

    void on_OpenImageButton_clicked();

    void on_SaveImageButton_clicked();

    void on_brightnessSlider_valueChanged(int value);

    void on_ContrastSlider_valueChanged(int value);

    void on_SaveAsImageButton_clicked();

    void on_ZoomSlider_valueChanged(int value);

    void on_QuitButton_clicked();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionQuit_triggered();

    void showStatusBarMessage(QString message);

    void on_nextButton_clicked();

    void on_previousButton_clicked();

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);


    void on_cancelButton_clicked();

    void on_cropButton_clicked();

    void on_readButton_clicked();

    void on_actionRead_Image_triggered();

private:
    Ui::MainWindow *ui;

    QString fileName;
    QString saveAsFileName;

    QPixmap pix;

    Mat openCVImage;
    Mat new_image;

    TessBaseAPI *ocr;
    QString outText;

    QString imageName;
    QString base;
    QStringList allImages;
    int allImagesSize;
    int imgQue;

    QRubberBand *rubberBand;
    QPoint origin;

    QPoint a;
    QPoint b;

};
#endif // MAINWINDOW_H
