#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "opencv2/opencv.hpp"

#include <QMainWindow>
#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QTimer>

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

private slots:
    void updateImageLabel(Mat mat);

    void showPixmap(QPixmap image);

    QPixmap mat2Pixmap(Mat mat);

    void on_actionOpen_triggered();

    void on_OpenImageButton_clicked();

    void on_SaveImageButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_ContrastSlider_valueChanged(int value);

    void on_SaveAsImageButton_clicked();

    void on_ZoomSlider_valueChanged(int value);

    void on_QuitButton_clicked();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionQuit_triggered();

    void showStatusBarMessage(QString message);

private:
    Ui::MainWindow *ui;

    QString fileName;
    QString saveAsFileName;
    QPixmap pix;
    Mat openCVImage;
    Mat new_image;
};
#endif // MAINWINDOW_H
