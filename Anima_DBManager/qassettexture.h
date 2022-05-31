#ifndef QASSETTEXTURE_H
#define QASSETTEXTURE_H

#include <QWidget>
#include <QLabel>
#include "qassetlabel.h"

class QAssetTexture : public QWidget
{
    Q_OBJECT

private:
    QLabel* myTexturePreview;
    QAssetLabel* myAssetLabel;

public:
    explicit QAssetTexture(QWidget *parent = nullptr);

    void SetValue(const QString& _texturePath);
    const QString& GetValue() const;
    const QAssetLabel* GetLabel() const;

signals:
    void OnValueEdited();

public slots:
    void OpenFileDialog();
    void RelayAssetLabelSignal();
};

#endif // QASSETTEXTURE_H
