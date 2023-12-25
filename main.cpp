#include <QtGui/QtGui>
#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QSlider>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QFileDialog>

QImage blurImage(QImage _source, int _blurRadius) {
    if (_source.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(_source));
    //
    auto* blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(_blurRadius);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(_source.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(), QRectF(0, 0, _source.width(), _source.height()));
    //
    return result;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget blurWidget;
    blurWidget.resize(1024, 768);

    QVBoxLayout vbox(&blurWidget);
    QLabel imageLabel;
    vbox.addWidget(&imageLabel);
    QSlider blurRadiusSlider;
    blurRadiusSlider.setOrientation(Qt::Horizontal);
    blurRadiusSlider.setMinimum(0);
    blurRadiusSlider.setMaximum(10);
    vbox.addWidget(&blurRadiusSlider);
    QPushButton openFileButton("Open File");
    vbox.addWidget(&openFileButton);

    QString filePath;
    int blurRadius;

    QObject::connect(&openFileButton, &QPushButton::clicked, [&filePath, &blurRadius, &blurRadiusSlider, &imageLabel](){
        filePath = QFileDialog::getOpenFileName(nullptr,
                                                "Open image",
                                                "F:\\__C++\\Skillbox\\38_Advanced_Qt\\_HW\\38_5_3_Blur\\38_5_3_Blur\\images",
                                                "Image files (*.jpg, *.jpeg, *.JPG, *.JPEG)");
        blurRadius = blurRadiusSlider.sliderPosition();
        imageLabel.setPixmap(QPixmap::fromImage(blurImage(QImage(filePath), blurRadius).scaled(imageLabel.width(),
                                                                                               imageLabel.height(),
                                                                                               Qt::KeepAspectRatio)));
    });

    QObject::connect(&blurRadiusSlider, &QSlider::sliderMoved, [&imageLabel, &filePath, &blurRadius, &blurRadiusSlider](){
        blurRadius = blurRadiusSlider.sliderPosition();
        imageLabel.setPixmap(QPixmap::fromImage(blurImage(QImage(filePath), blurRadius).scaled(imageLabel.width(),
                                                                                               imageLabel.height(),
                                                                                               Qt::KeepAspectRatio)));
    });

    blurWidget.show();
    return a.exec();
}
