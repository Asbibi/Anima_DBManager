#ifndef QAPMESH_H
#define QAPMESH_H

#include "qassetpreview.h"

class QAPMesh : public QAssetPreview
{
    Q_OBJECT

public:
    explicit QAPMesh(QWidget *parent = nullptr);

public slots:
    virtual void SetFilePath(const QString& _filePath) override;
};

#endif // QAPMESH_H
