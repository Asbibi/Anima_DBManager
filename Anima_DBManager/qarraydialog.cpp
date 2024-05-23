#include "qarraydialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>


QArrayDialog::QArrayDialog(const TemplateAttribute* _template, const QList<Attribute*>& _attributeList, bool _useMin, int _min, bool _useMax, int _max, QWidget* _parent) :
    QDialog(_parent),
    myTemplateAttribute (_template)
{
    // ------ UI ------

    setWindowTitle("Edit Array Attribute");
    setModal(true);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    vLayout->addWidget(new QLabel("Array:"));
    myQListAttributes = new QAugmentedList(false, _template->GetDefaultAttribute()->GetValue_String(), this);
    myQListAttributes->SetItemEditable(false);
    myQListAttributes->SetMinMax(_useMin, _min, _useMax, _max);
    vLayout->addWidget(myQListAttributes);

    QHBoxLayout* countLayout = new QHBoxLayout();
    myRowCountSpinner = new QSpinBox();
    myRowCountSpinner->setMinimum(_useMin ? _min : 0);
    myRowCountSpinner->setMaximum(_useMax ? _max : INT_MAX);
    countLayout->addWidget(myRowCountSpinner);
    QPushButton* countPushBtn = new QPushButton("Set Item Count");
    countPushBtn->setMaximumWidth(90);
    QObject::connect(countPushBtn, &QPushButton::clicked, this, &QArrayDialog::SetItemCount);
    countLayout->addWidget(countPushBtn);
    vLayout->addLayout(countLayout);

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


    // ------ Values & propeties ------

    for (auto* attr : _attributeList)
    {
        myAttributes.push_back(attr->CreateDuplica());
        myQListAttributes->AddItemAt(attr->GetValue_String());
    }


    // ------ Connect ------

    QObject::connect(myQListAttributes, &QAugmentedList::SelectionChanged, this, &QArrayDialog::UpdateQAttribute);
    QObject::connect(myQListAttributes, &QAugmentedList::ItemAdded, this, &QArrayDialog::OnValueAdded);
    QObject::connect(myQListAttributes, &QAugmentedList::ItemDuplicated, this, &QArrayDialog::OnValueDuplicated);
    QObject::connect(myQListAttributes, &QAugmentedList::ItemMoved, this, &QArrayDialog::OnValueMoved);
    QObject::connect(myQListAttributes, &QAugmentedList::ItemRemoved, this, &QArrayDialog::OnValueRemoved);


    // ------ Init ------

    UpdateQAttribute(0);
    UpdateCountSpinnerValue();
}
QArrayDialog::~QArrayDialog()
{
    CleanAttributes();
}
void QArrayDialog::CleanAttributes()
{
    for (auto* attr : myAttributes)
    {
        attr->PreManualDelete();
        delete attr;
    }
    myAttributes.clear();
}
void QArrayDialog::UpdateCountSpinnerValue()
{
    myRowCountSpinner->setValue(myAttributes.count());
}
const QList<Attribute*>& QArrayDialog::GetAttributes() const
{
    return myAttributes;
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
    UpdateCountSpinnerValue();
}
void QArrayDialog::OnValueDuplicated(const int _index, const int _originalIndex)
{
    const int attrCount = myAttributes.count();
    Q_ASSERT(_index >= 0 &&  _index <= attrCount);
    Q_ASSERT(_originalIndex >= 0 &&  _originalIndex < attrCount);
    myAttributes.insert(_index, myAttributes[_originalIndex]->CreateDuplica());
    UpdateCountSpinnerValue();
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
    auto* removedAttr = myAttributes.takeAt(_index);
    removedAttr->PreManualDelete();
    delete removedAttr;
    UpdateCountSpinnerValue();
}
void QArrayDialog::SetItemCount()
{
    const int wantedCount = myRowCountSpinner->value();

    myQListAttributes->SelectItemAt(myAttributes.count() - 1);
    while (wantedCount < myAttributes.count())
    {
        myQListAttributes->OnRemove();
    }
    while (wantedCount > myAttributes.count())
    {
        myQListAttributes->OnAddAfter();
    }
}
void QArrayDialog::OnSelectedValueEdited()
{
    int current = myQListAttributes->GetCurrent();
    Q_ASSERT(current >= 0 &&  current < myAttributes.count());
    myQListAttributes->SetItemValue(current, myAttributes[current]->GetValue_String(), false);
}
