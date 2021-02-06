#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "opencv2/opencv.hpp"

#include <QMainWindow>
#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace cv;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateImageLabel(Mat mat);
    void showPixmap(QPixmap image);
    QPixmap mat2Pixmap(Mat mat);

private slots:
    void on_actionOpen_triggered();

    void on_OpenImageButton_clicked();

    void on_SaveImageButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_ContrastSlider_valueChanged(int value);

    void on_SaveAsImageButton_clicked();

    void on_ZoomSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    QString fileName;
    QString saveAsFileName;
    QPixmap pix;
    Mat openCVImage;
    Mat new_image;
};
#endif // MAINWINDOW_H
