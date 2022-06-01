#ifndef QAPTEXTURE_H
#define QAPTEXTURE_H

#include "qassetpreview.h"
#include <QLabel>

class QAPTexture : public QAssetPreview
{
    Q_OBJECT

private:
    QLabel* myImage;
    QLabel* myImageSize;

public:
    QAPTexture(QWidget *parent = nullptr);

public slots:
    virtual void SetFilePath(const QString& _filePath) override;
};

#endif // QAPTEXTURE_H
