#include "imageread.h"
#include "ui_imageread.h"

ImageRead::ImageRead(QString data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageRead)
{
    ui->setupUi(this);
    text = data;

    ui->textBrowser->setText(text);
}

ImageRead::~ImageRead()
{
    delete ui;
}
