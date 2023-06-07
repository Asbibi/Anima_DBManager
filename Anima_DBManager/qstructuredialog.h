#ifndef QSTRUCTUREDIALOG_H
#define QSTRUCTUREDIALOG_H

#include <QDialog>

#include "attribute.h"
#include "qattribute.h"
#include <QListWidget>

class QStructureDialog : public QDialog
{
    Q_OBJECT

private:
    QList<Attribute*> myAttributes;
    QListWidget* myList;
    QAttribute* myCurrentAttributeEditor = nullptr;

public:
    QStructureDialog(const QList<Attribute*>& _attributeList, QWidget* _parent = nullptr);
    ~QStructureDialog();

    QString GetValue() const;

public slots:
    void OnSelectItem(QListWidgetItem* _current, QListWidgetItem* _previous);
};

#endif // QSTRUCTUREDIALOG_H
