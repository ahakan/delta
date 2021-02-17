#ifndef IMAGEREAD_H
#define IMAGEREAD_H

#include <QDialog>
#include <QString>

namespace Ui {
class ImageRead;
}

class ImageRead : public QDialog
{
    Q_OBJECT

public:
    explicit ImageRead(QString data = "", QWidget *parent = nullptr);
    ~ImageRead();

private:
    Ui::ImageRead *ui;
    QString text;
};

#endif // IMAGEREAD_H
