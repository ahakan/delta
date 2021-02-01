#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;

    QString fileName;
    QAction *fitToWindowAct;
};
#endif // MAINWINDOW_H
