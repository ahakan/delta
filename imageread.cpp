#include "imageread.h"
#include "ui_imageread.h"

ImageRead::ImageRead(QString data, QString imgName, QString imgDirectory, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageRead)
{
    ui->setupUi(this);
    text = data;
    name = imgName;
    directory = imgDirectory;

    this->setWindowTitle(name + " - Image Text");
    ui->plainTextEdit->appendPlainText(text);
}

ImageRead::~ImageRead()
{
    delete ui;
}


void ImageRead::on_saveButton_clicked()
{

    QString filename= directory + "/" + "TEXT_" + name + "_.txt";

    QFile file( filename );

    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << ui->plainTextEdit->toPlainText();
    }

    this->close();
}

void ImageRead::on_cancelButton_clicked()
{
    this->close();
}
