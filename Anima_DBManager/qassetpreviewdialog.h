#ifndef QASSETPREVIEWDIALOG_H
#define QASSETPREVIEWDIALOG_H

#include <QDialog>

#include "attribute.h"
#include "qaptexture.h"
#include "qapsound.h"
#include "qapmesh.h"
#include <QString>
#include <QPushButton>
#include <QLabel>

class QAssetPreviewDialog : public QDialog
{
    Q_OBJECT

private:
    //const Attribute::Type myType;
    QString& myFilePath;
    QPushButton* myOpenFileButton;

    QAPTexture* myTexturePreview;
    QAPSound* mySoundPreview;
    QAPMesh* myMeshPreview;

    const QString& myDialogTitle;
    const QString& myDialogExtensions;

public:
    explicit QAssetPreviewDialog(const Attribute::Type& assetType,
                                const QString& dialogTitle,
                                const QString& dialogExtensions,
                                QString& filePath,
                                QWidget* parent = nullptr);

signals:
    void FilePathChanged(const QString& _filePath);

public slots:
    void ChangeFilePath();
};

#endif // QASSETPREVIEWDIALOG_H
