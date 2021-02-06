#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "opencv2/opencv.hpp"

#include <QMainWindow>
#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QUndoView>
#include <QAction>

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
    void changeEvent(QEvent *event);
    void updateCanvasLabel(Mat mat);

private slots:
    void on_actionOpen_triggered();

    void on_OpenImageButton_clicked();

    void on_SaveImageButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    QString fileName;
    QPixmap pix;
    Mat openCVImage;
};
#endif // MAINWINDOW_H
