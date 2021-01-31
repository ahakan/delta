#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Delta");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSave_triggered()
{

}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", ".", "Image Files(*.png *.jpg *.jpeg)");

    qDebug() << "File: " << fileName;
    this->setWindowTitle(fileName + " - Delta");
}
