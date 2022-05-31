#ifndef QASSETLABEL_H
#define QASSETLABEL_H

#include <QLabel>

class QAssetLabel : public QLabel
{
    Q_OBJECT

protected:
    const QString myDialogTitle;
    const QString myDialogExtensions;

    QString myFilePath;

public:
    explicit QAssetLabel(const QString& dialogTitle, const QString& dialogExtensions, QWidget* parent = nullptr);

    virtual void SetValue(const QString& _filePath);
    virtual const QString& GetValue() const;

signals:
    void OnValueEdited();

public slots:
    void OpenFileDialog();
};

#endif // QASSETLABEL_H
