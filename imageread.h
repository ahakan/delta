#ifndef IMAGEREAD_H
#define IMAGEREAD_H

#include <QDialog>
#include <QString>
#include <QFile>
#include <QTextStream>

namespace Ui {
class ImageRead;
}

using namespace std;

class ImageRead : public QDialog
{
    Q_OBJECT

public:
    explicit ImageRead(QString data = "", QString imgName = "", QString imgDirectory = "", QWidget *parent = nullptr);
    ~ImageRead();

private slots:
    void on_saveButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ImageRead *ui;
    QString text;
    QString name;
    QString directory;
};

#endif // IMAGEREAD_H
