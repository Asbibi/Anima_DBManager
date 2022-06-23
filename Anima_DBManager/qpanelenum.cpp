#include "qpanelenum.h"

#include "db_manager.h"
#include "enumerator.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

QPanelEnum::QPanelEnum(QWidget *parent)
    : QPanelBase {"Enum", false, parent}
{
    QLayout* myLayout = layout();

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

    QObject::connect(myEnumColorList, &QListWidget::itemChanged, this, &QPanelEnum::SetColorFromText);
    QObject::connect(myEnumValuesList, &QAugmentedList::ItemAdded, this, &QPanelEnum::OnAddedEnumValue);
    QObject::connect(myEnumValuesList, &QAugmentedList::ItemDuplicated, this, &QPanelEnum::OnDuplicatedEnumValue);
    QObject::connect(myEnumValuesList, &QAugmentedList::ItemMoved, this, &QPanelEnum::OnMovedEnumValue);
    QObject::connect(myEnumValuesList, &QAugmentedList::ItemRemoved, this, &QPanelEnum::OnRemovedEnumValue);
    QObject::connect(myEnumValuesList, &QAugmentedList::SelectionChanged, this, &QPanelEnum::OnSelectEnumValue);

    QObject::connect(myColorCheckbox, &QCheckBox::stateChanged, this, &QPanelEnum::OnToggleUseColor);

    QObject::connect(myApplyBtn, &QPushButton::clicked, this, &QPanelEnum::ApplyEdits);
    QObject::connect(myResetBtn, &QPushButton::clicked, this, &QPanelEnum::RevertEdits);

    //OnItemSelected(-1);
    myItemList->SelectItemAt(-1);
}

void QPanelEnum::Init()
{
    QPanelBase::Init();
    OnItemSelected(-1);
}

void QPanelEnum::UpdateItemList()
{
    myItemList->Clear();
    const DB_Manager& DB = DB_Manager::GetDB_Manager();
    const int enumCount = DB.GetEnumCount();
    for (int i = 0; i < enumCount; i++)
    {
        myItemList->AddItemAt(DB.GetEnum(i)->GetName());
    }
}


void QPanelEnum::AddColorItem(const QString& _hexColor, int _index)
{
    const int colorCount = myEnumColorList->count();
    if (_index < 0 || _index > colorCount)
        _index = colorCount;

    myEnumColorList->insertItem(_index, _hexColor);
    auto* item = myEnumColorList->item(_index);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
}

void QPanelEnum::OnItemSelected(const int _index)
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



void QPanelEnum::ApplyEdits()
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
void QPanelEnum::RevertEdits()
{
    OnItemSelected(myItemList->GetCurrent());
}



void QPanelEnum::OnItemEdited(const int _index, const QString& _value)
{
    DB_Manager::GetDB_Manager().UpdateEnumName(_index, _value);
}
void QPanelEnum::OnItemAdded(const int _index, const QString& _value)
{
    DB_Manager::GetDB_Manager().AddEnum(Enumerator(_value), _index);
}
void QPanelEnum::OnItemDuplicated(const int _index, const int _originalIndex)
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
void QPanelEnum::OnItemMoved(const int _indexFrom, const int _indexTo)
{
    DB_Manager::GetDB_Manager().MoveEnum(_indexFrom, _indexTo);
}
void QPanelEnum::OnItemRemoved(const int _index)
{
    DB_Manager::GetDB_Manager().RemoveEnum(_index);
}



void QPanelEnum::SetColorFromText(QListWidgetItem* _item)
{
    QColor color = QColor(_item->text());
    QBrush itemColor = QBrush(color.isValid() ? color : QColorConstants::Black);
    _item->setForeground(itemColor);
}
#define CHECK_USE_COLOR()     if (!myColorCheckbox->checkState()) return
void QPanelEnum::OnAddedEnumValue(int _index)
{
    CHECK_USE_COLOR();
    AddColorItem("#------", _index);
}
void QPanelEnum::OnDuplicatedEnumValue(int _index, int _originalIndex)
{
    CHECK_USE_COLOR();
    const auto* originalColor = myEnumColorList->item(_originalIndex);
    if (originalColor)
        AddColorItem(originalColor->text(), _index);
    else
        AddColorItem("#------", _index);    // Fallback, shouldn't be triggered
}
void QPanelEnum::OnMovedEnumValue(int _indexFrom, int _indexTo)
{
    CHECK_USE_COLOR();
    auto* movedItem = myEnumColorList->takeItem(_indexFrom);
    if (!movedItem)
        return;
    myEnumColorList->insertItem(_indexTo, movedItem);
}
void QPanelEnum::OnRemovedEnumValue(int _index)
{
    CHECK_USE_COLOR();

    auto* removedItem = myEnumColorList->takeItem(_index);
    if (removedItem)
        delete removedItem;
}
void QPanelEnum::OnToggleUseColor(const int _state)
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
void QPanelEnum::OnSelectEnumValue(const int _index)
{
    CHECK_USE_COLOR();

    myEnumColorList->setCurrentRow(_index);
}
#undef CHECK_USE_COLOR
