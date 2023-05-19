#ifndef QARRAYDIALOG_H
#define QARRAYDIALOG_H

#include <QDialog>

#include "attribute.h"
#include "qattribute.h"
#include "qaugmentedlist.h"
#include "templateattribute.h"

class QArrayDialog : public QDialog
{
    Q_OBJECT

private:
    const TemplateAttribute* myTemplateAttribute;   // not the owner, the owner is the AAray's param object
    QList<Attribute*> myAttributes;                 // owner, they are (altered) copies of the array element

    QAugmentedList* myQListAttributes;
    QAttribute* myQAttribute;

    void CleanAttributes();

public:
    QArrayDialog(const TemplateAttribute* _template, const QList<Attribute*>& _attributeList, QWidget* _parent = nullptr);
    ~QArrayDialog();

public slots:
    void UpdateQAttribute(const int _index);

    void OnValueAdded(const int _index);
    void OnValueDuplicated(const int _index, const int _originalIndex);
    void OnValueMoved(const int _indexFrom, const int _indexTo);
    void OnValueRemoved(const int _index);

    void OnSelectedValueEdited();
};

#endif // QARRAYDIALOG_H
