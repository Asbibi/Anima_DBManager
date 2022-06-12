#include "qenumpanel.h"

#include "db_manager.h"
#include "enumerator.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

QEnumPanel::QEnumPanel(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* myLayout = new QVBoxLayout();
    setLayout(myLayout);

    QGroupBox* listGroupBox = new QGroupBox("All Enums");
    myLayout->addWidget(listGroupBox);
    myItemList = new QAugmentedList(false, "New Enum", listGroupBox);
    QVBoxLayout* listGroupBoxLayout = new QVBoxLayout();
    listGroupBox->setLayout(listGroupBoxLayout);
    listGroupBoxLayout->addWidget(myItemList);


    QGroupBox* editGroupBox = new QGroupBox("Edit Selected Enum");
    QGridLayout* editLayout = new QGridLayout();
    editGroupBox->setLayout(editLayout);
    myLayout->addWidget(editGroupBox);


    myEnumValuesList = new QAugmentedList(true, "Case");
    myEnumColorList = new QListWidget();
    myColorCheckbox = new QCheckBox();
    editLayout->addWidget(new QLabel("Values:"), 0, 0);
    editLayout->addWidget(new QLabel("Colors:"), 0, 1);
    editLayout->addWidget(myEnumValuesList, 1, 0);
    editLayout->addWidget(myEnumColorList, 1, 1);

    QFormLayout* useColorLayout = new QFormLayout();
    useColorLayout->addRow("Use Colors:", myColorCheckbox);
    editLayout->addLayout(useColorLayout, 2, 0);

    myApplyBtn = new QPushButton("Apply");
    myResetBtn = new QPushButton("Discard");
    editLayout->addWidget(myApplyBtn, 3, 0);
    editLayout->addWidget(myResetBtn, 3, 1);

    QObject::connect(myItemList, &QAugmentedList::SelectionChanged, this, &QEnumPanel::UpdateEditSection);

    QObject::connect(myItemList, &QAugmentedList::ItemEdited, this, &QEnumPanel::OnItemEdited);
    QObject::connect(myItemList, &QAugmentedList::ItemAdded, this, &QEnumPanel::OnItemAdded);
    QObject::connect(myItemList, &QAugmentedList::ItemDuplicated, this, &QEnumPanel::OnItemDuplicated);
    QObject::connect(myItemList, &QAugmentedList::ItemMoved, this, &QEnumPanel::OnItemMoved);
    QObject::connect(myItemList, &QAugmentedList::ItemRemoved, this, &QEnumPanel::OnItemRemoved);

    QObject::connect(myEnumColorList, &QListWidget::itemChanged, this, &QEnumPanel::SetColorFromText);
    QObject::connect(myEnumValuesList, &QAugmentedList::ItemAdded, this, &QEnumPanel::OnAddedEnumValue);
    QObject::connect(myEnumValuesList, &QAugmentedList::ItemDuplicated, this, &QEnumPanel::OnDuplicatedEnumValue);
    QObject::connect(myEnumValuesList, &QAugmentedList::ItemMoved, this, &QEnumPanel::OnMovedEnumValue);
    QObject::connect(myEnumValuesList, &QAugmentedList::ItemRemoved, this, &QEnumPanel::OnRemovedEnumValue);

    QObject::connect(myColorCheckbox, &QCheckBox::stateChanged, this, &QEnumPanel::OnToggleUseColor);

    QObject::connect(myApplyBtn, &QPushButton::clicked, this, &QEnumPanel::ApplyEdits);
    QObject::connect(myResetBtn, &QPushButton::clicked, this, &QEnumPanel::RevertEdits);

    UpdateItemList();
    UpdateEditSection(-1);
}

void QEnumPanel::UpdateItemList()
{
    const DB_Manager& DB = DB_Manager::GetDB_Manager();
    const int enumCount = DB.GetEnumCount();
    for (int i = 0; i < enumCount; i++)
    {
        myItemList->AddItemAt(DB.GetEnum(i)->GetName());
    }
}


void QEnumPanel::AddColorItem(const QString& _hexColor, int _index)
{
    const int colorCount = myEnumColorList->count();
    if (_index < 0 || _index > colorCount)
        _index = colorCount;

    myEnumColorList->insertItem(_index, _hexColor);
    auto* item = myEnumColorList->item(_index);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    //SetColorFromText(item);
}

void QEnumPanel::UpdateEditSection(const int _index)
{
    myEnumValuesList->Clear();
    myEnumColorList->clear();
    if (_index < 0)
    {
        myColorCheckbox->setChecked(false);
        myEnumColorList->setEnabled(false);
        return;
    }

    const Enumerator* currentEnum = DB_Manager::GetDB_Manager().GetEnum(_index);
    if (!currentEnum)
    {
        qFatal("Selected inexisting enum at index %d", _index);
        return;
    }

    const bool useColor = currentEnum->GetUseColor();
    myColorCheckbox->setChecked(useColor);
    myEnumColorList->setEnabled(useColor);
    const int valueCount = currentEnum->GetValueCount();
    for (int i = 0; i < valueCount; i++)
    {
        myEnumValuesList->AddItemAt(currentEnum->GetValue(i));
        if (useColor)
        {
            AddColorItem(currentEnum->GetColorHex(i));
        }
    }
}



void QEnumPanel::ApplyEdits()
{
    QString name;
    const int currentIndex = myItemList->GetCurrent(&name);
    if (currentIndex < 0)
        return;

    const int valueCount = myEnumValuesList->Count();
    const bool useColor = myColorCheckbox->checkState() != Qt::Unchecked;

    Enumerator newEnum {name};
    for (int i = 0; i < valueCount; i++)
    {
        const QString value = myEnumValuesList->GetItemValue(i);
        if (useColor)
        {
            const QColor enumColor = QColor(myEnumColorList->item(i)->text());
            newEnum.AddValue(value, &enumColor);
        }
        else
            newEnum.AddValue(value);
    }

    DB_Manager::GetDB_Manager().UpdateEnum(currentIndex, newEnum);
}
void QEnumPanel::RevertEdits()
{
    UpdateEditSection(myItemList->GetCurrent());
}



void QEnumPanel::OnItemEdited(const int _index, const QString& _value)
{
    DB_Manager::GetDB_Manager().UpdateEnumName(_index, _value);
}
void QEnumPanel::OnItemAdded(const int _index, const QString& _value)
{
    DB_Manager::GetDB_Manager().AddEnum(Enumerator(_value), _index);
}
void QEnumPanel::OnItemDuplicated(const int _index, const int _originalIndex)
{
    auto& DB = DB_Manager::GetDB_Manager();
    const auto* originalEnum = DB.GetEnum(_originalIndex);
    if (!originalEnum)
    {
        qFatal("Duplicating inexisting enumerator");
        return;
    }
    DB.AddEnum(Enumerator(*originalEnum), _index);
}
void QEnumPanel::OnItemMoved(const int _indexFrom, const int _indexTo)
{
    DB_Manager::GetDB_Manager().MoveEnum(_indexFrom, _indexTo);
}
void QEnumPanel::OnItemRemoved(const int _index)
{
    DB_Manager::GetDB_Manager().RemoveEnum(_index);
}



void QEnumPanel::SetColorFromText(QListWidgetItem* _item)
{
    QColor color = QColor(_item->text());
    QBrush itemColor = QBrush(color.isValid() ? color : QColorConstants::Black);
    _item->setForeground(itemColor);
}
#define CHECK_USE_COLOR()     if (!myColorCheckbox->checkState()) return
void QEnumPanel::OnAddedEnumValue(int _index)
{
    CHECK_USE_COLOR();
    AddColorItem("#------", _index);
}
void QEnumPanel::OnDuplicatedEnumValue(int _index, int _originalIndex)
{
    CHECK_USE_COLOR();
    const auto* originalColor = myEnumColorList->item(_originalIndex);
    if (originalColor)
        AddColorItem(originalColor->text(), _index);
    else
        AddColorItem("#------", _index);    // Fallback, shouldn't be triggered
}
void QEnumPanel::OnMovedEnumValue(int _indexFrom, int _indexTo)
{
    CHECK_USE_COLOR();
    auto* movedItem = myEnumColorList->takeItem(_indexFrom);
    if (!movedItem)
        return;
    myEnumColorList->insertItem(_indexTo, movedItem);
}
void QEnumPanel::OnRemovedEnumValue(int _index)
{
    CHECK_USE_COLOR();

    auto* removedItem = myEnumColorList->takeItem(_index);
    if (removedItem)
        delete removedItem;
}
#undef CHECK_USE_COLOR
void QEnumPanel::OnToggleUseColor(const int _state)
{
    bool useColor = _state != Qt::Unchecked;
    myEnumColorList->setEnabled(useColor);
    myEnumColorList->clear();
    if (!useColor)
        return;

    const int valueCount = myEnumValuesList->Count();
    for (int i = 0; i < valueCount; i++)
    {
        AddColorItem("#------");
    }
}
