#include "qaptexture.h"

#include <QFrame>
#include <QPixmap>
#include <QVBoxLayout>

#define IMAGE_SIZE 512

QAPTexture::QAPTexture(QWidget *parent) :
    QAssetPreview(parent)
{
    auto* vLayout = layout();

    myImage = new QLabel();
    myImage->setMinimumSize(IMAGE_SIZE, IMAGE_SIZE);
    myImage->setMaximumSize(IMAGE_SIZE, IMAGE_SIZE);
    myImage->setFrameStyle(QFrame::Panel | QFrame::Plain);
    myImage->setLineWidth(1);
    vLayout->addWidget(myImage);

    vLayout->addItem(new QSpacerItem(1,3));

    myImageSize = new QLabel();
    myImageSize->setMinimumWidth(150);
    vLayout->addWidget(myImageSize);
}

void QAPTexture::SetFilePath(const QString& _filePath)
{
    QAssetPreview::SetFilePath(_filePath);

    auto imagePixMap = QPixmap(_filePath);
    myImage->setPixmap(imagePixMap.scaled(IMAGE_SIZE, IMAGE_SIZE, Qt::AspectRatioMode::KeepAspectRatio));
    myImageSize->setText(QString("Size: %1x%2 px").arg(QString::number(imagePixMap.width()), QString::number(imagePixMap.height())));
}
