#ifndef QSSTRING_H
#define QSSTRING_H

//#include <QWidget>
#include <QLabel>

class QSString : public QLabel
{
    Q_OBJECT

private:
    QString myTableName;
    QString myStringIdentifier;

public:
    QSString(QWidget* parent);

    void SetValue(const QString& _tableId, const QString& _stringId);
    void GetValue(QString& _outTableId, QString& _outStringId) const;
    QString GetFormattedValue() const;

signals:
    void OnValueEdited();

public slots:
    void EditValue();
};

#endif // QSSTRING_H
