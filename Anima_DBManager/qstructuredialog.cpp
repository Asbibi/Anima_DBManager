#include "qstructuredialog.h"

#include "astructure.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>


QStructureDialog::QStructureDialog(const QList<Attribute*>& _attributeList, QWidget* _parent) :
    QDialog(_parent)
{
    // ------ UI ------

    setWindowTitle("Edit Structure Attribute");
    setModal(true);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    vLayout->addWidget(new QLabel("Structure:"));
    myList = new QListWidget();
    vLayout->addWidget(myList);

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
        myList->addItem(attr->GetValueAsText());
    }


    QObject::connect(myList, &QListWidget::currentItemChanged, this, &QStructureDialog::OnSelectItem);
}
QStructureDialog::~QStructureDialog()
{
    for (auto* attr : myAttributes)
    {
        delete attr;
    }
}

QString QStructureDialog::GetValue() const
{
    return AStructure::GetValueAsTextFromAttributes(myAttributes);
}

void QStructureDialog::OnSelectItem(QListWidgetItem* _current, QListWidgetItem* _previous)
{
    if (myCurrentAttributeEditor != nullptr)
    {
        myList->setItemWidget(_previous, nullptr);
        QObject::disconnect(myCurrentAttributeEditor);
        delete myCurrentAttributeEditor;
        myCurrentAttributeEditor = nullptr;
        _previous->setText(myAttributes[myList->row(_previous)]->GetValueAsText());
    }

    myCurrentAttributeEditor = new QAttribute();
    myCurrentAttributeEditor->UpdateAttribute(myAttributes[myList->row(_current)]);
    myList->setItemWidget(_current, myCurrentAttributeEditor);
    _current->setText("");
}
