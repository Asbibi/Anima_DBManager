#include "qpanelbase.h"

#include <QGroupBox>
#include <QVBoxLayout>

QPanelBase::QPanelBase(const QString& itemType, bool _uniqueNames, QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* myLayout = new QVBoxLayout();
    setLayout(myLayout);
    QGroupBox* listGroupBox = new QGroupBox("All " + itemType + "s");
    myLayout->addWidget(listGroupBox);

    myItemList = new QAugmentedList(_uniqueNames, "New " + itemType, listGroupBox);
    QVBoxLayout* listGroupBoxLayout = new QVBoxLayout();
    listGroupBox->setLayout(listGroupBoxLayout);
    listGroupBoxLayout->addWidget(myItemList);

    QObject::connect(myItemList, &QAugmentedList::ItemEdited, this, &QPanelBase::OnItemEdited);
    QObject::connect(myItemList, &QAugmentedList::ItemAdded, this, &QPanelBase::OnItemAdded);
    QObject::connect(myItemList, &QAugmentedList::ItemDuplicated, this, &QPanelBase::OnItemDuplicated);
    QObject::connect(myItemList, &QAugmentedList::ItemMoved, this, &QPanelBase::OnItemMoved);
    QObject::connect(myItemList, &QAugmentedList::ItemRemoved, this, &QPanelBase::OnItemRemoved);
}

void QPanelBase::Init()
{
    UpdateItemList();
}
