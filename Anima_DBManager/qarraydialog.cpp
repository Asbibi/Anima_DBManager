#include "qarraydialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>


QArrayDialog::QArrayDialog(const TemplateAttribute* _template, const QList<Attribute*>& _attributeList, QWidget* _parent) :
    QDialog(_parent),
    myTemplateAttribute (_template)
{
    // ------ Values & propeties ------

    for (auto* attr : _attributeList)
    {
        myAttributes.push_back(attr->CreateDuplica());
    }


    // ------ UI ------

    setWindowTitle("Edit Array Attribute Values");
    setModal(true);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    vLayout->addWidget(new QLabel("Array:"));
    myQListAttributes = new QAugmentedList(false, _template->GetDefaultAttribute()->GetDisplayedText(true), this);
    myQListAttributes->SetItemEditable(false);
    vLayout->addWidget(myQListAttributes);
    // + count label ?

    vLayout->addWidget(new QLabel("Edit Selected:"));
    myQAttribute = new QAttribute();
    vLayout->addWidget(myQAttribute);

    QPushButton* applyBtn = new QPushButton("Apply");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QHBoxLayout* hLayoutBtn = new QHBoxLayout();
    hLayoutBtn->addWidget(applyBtn);
    QObject::connect(applyBtn, &QPushButton::clicked, this, &QDialog::accept);
    hLayoutBtn->addWidget(cancelBtn);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    vLayout->addLayout(hLayoutBtn);


    // Connect
    QObject::connect(myQListAttributes, &QAugmentedList::SelectionChanged, this, &QArrayDialog::UpdateQAttribute);
    QObject::connect(myQListAttributes, &QAugmentedList::ItemAdded, this, &QArrayDialog::OnValueAdded);
    QObject::connect(myQListAttributes, &QAugmentedList::ItemDuplicated, this, &QArrayDialog::OnValueDuplicated);
    QObject::connect(myQListAttributes, &QAugmentedList::ItemMoved, this, &QArrayDialog::OnValueMoved);
    QObject::connect(myQListAttributes, &QAugmentedList::ItemRemoved, this, &QArrayDialog::OnValueRemoved);


    UpdateQAttribute(0);
}
QArrayDialog::~QArrayDialog()
{
    CleanAttributes();
}
void QArrayDialog::CleanAttributes()
{
    for (auto* attr : myAttributes)
    {
        delete attr;
    }
    myAttributes.clear();
}



void QArrayDialog::UpdateQAttribute(const int _index)
{
    if (_index < 0 || _index >= myAttributes.count())
    {
        myQAttribute->hide();
        return;
    }

    myQAttribute->show();
    myQAttribute->UpdateAttribute(myAttributes[_index]);
    QObject::connect(myQAttribute, &QAttribute::OnWidgetValueChanged, this, &QArrayDialog::OnSelectedValueEdited);
}
void QArrayDialog::OnValueAdded(const int _index)
{
    Q_ASSERT(_index >= 0 &&  _index <= myAttributes.count());
    myAttributes.insert(_index, myTemplateAttribute->GenerateAttribute());
}
void QArrayDialog::OnValueDuplicated(const int _index, const int _originalIndex)
{
    const int attrCount = myAttributes.count();
    Q_ASSERT(_index >= 0 &&  _index <= attrCount);
    Q_ASSERT(_originalIndex >= 0 &&  _originalIndex < attrCount);
    myAttributes.insert(_index, myAttributes[_originalIndex]->CreateDuplica());
}
void QArrayDialog::OnValueMoved(const int _indexFrom, const int _indexTo)
{
    if (_indexFrom == _indexTo)
        return;

    const int attrCount = myAttributes.count();
    Q_ASSERT(_indexFrom >= 0 &&  _indexFrom < attrCount);
    Q_ASSERT(_indexTo >= 0 &&  _indexTo < attrCount);

    myAttributes.insert(_indexTo, myAttributes.takeAt(_indexFrom));
    UpdateQAttribute(_indexTo);
}
void QArrayDialog::OnValueRemoved(const int _index)
{
    Q_ASSERT(_index >= 0 &&  _index < myAttributes.count());
    delete myAttributes.takeAt(_index);
}
void QArrayDialog::OnSelectedValueEdited()
{
    int current = myQListAttributes->GetCurrent();
    Q_ASSERT(current >= 0 &&  current < myAttributes.count());
    myQListAttributes->SetItemValue(current, myAttributes[current]->GetDisplayedText(true), false);
}
