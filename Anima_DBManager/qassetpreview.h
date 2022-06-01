#ifndef QASSETPREVIEW_H
#define QASSETPREVIEW_H

#include <QWidget>
#include <QLabel>

class QAssetPreview : public QWidget
{
    Q_OBJECT

private:
    QLabel* myAssetPath;

public:
    QAssetPreview(QWidget *parent = nullptr);
    virtual ~QAssetPreview() {}

public slots:
    virtual void SetFilePath(const QString& _filePath);
};

#endif // QASSETPREVIEW_H
