#include "qpanelstring.h"

#include "db_manager.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

QPanelString::QPanelString(QWidget *parent)
    : QPanelWithCount{"String Table", true, parent}
{
    QLayout* myLayout = layout();

    mySubGroupBox = new QGroupBox("Edit Selected String Table");
    QFormLayout* editLayout = new QFormLayout();
    mySubGroupBox->setLayout(editLayout);
    mySubGroupBox->hide();
    myLayout->addWidget(mySubGroupBox);

    mySearchList = new QListWithSearch(true, "New Identifier");
    mySearchList->SetDisableRemoveLast(true);
    editLayout->addRow("Identifiers:", mySearchList);
    /*
    QGridLayout* clearLayout = new QGridLayout();
    clearLayout->addWidget(new QLineEdit(), 0,0);
    clearLayout->addWidget(new QPushButton("Clear Those"), 0,1);
    clearLayout->addWidget(new QPushButton("Clear All"), 1,1);
    editLayout->addRow("Clear:", clearLayout);
    */

    QObject::connect(mySearchList, &QListWithSearch::SelectionChanged, this, &QPanelString::OnSubItemSelected);

#define CONNECT_SUB_ITEM_SIGNAL(method) QObject::connect(mySearchList, &QListWithSearch::method, this, &QPanelString::OnSub##method)
    CONNECT_SUB_ITEM_SIGNAL(ItemEdited);
    CONNECT_SUB_ITEM_SIGNAL(ItemAdded);
    CONNECT_SUB_ITEM_SIGNAL(ItemDuplicated);
    CONNECT_SUB_ITEM_SIGNAL(ItemMoved);
    CONNECT_SUB_ITEM_SIGNAL(ItemRemoved);
#undef CONNECT_SUB_ITEM_SIGNAL


    InitItemCountWidget(editLayout);
}

int QPanelString::RefreshItemCount_Internal(const int _tableIndex)
{
    return DB_Manager::GetDB_Manager().GetStringTable(_tableIndex)->GetStringItemCount();
}
void QPanelString::SetItemCount_Internal(const int _tableIndex, const int _newCount)
{
    DB_Manager::GetDB_Manager().SetStringTableItemCount(_tableIndex, _newCount);
}


void QPanelString::UpdateItemList()
{
    myItemList->Clear();
    auto& DB = DB_Manager::GetDB_Manager();
    const int tableCount = DB.GetStringTableCount();
    for (int i = 0; i < tableCount; i++)
    {
        myItemList->AddItemAt(DB.GetStringTable(i)->GetTableName());
    }
}


void QPanelString::OnItemSelected(const int _index)
{
    mySearchList->ResetValues();

    SStringTable* currentTable = DB_Manager::GetDB_Manager().GetStringTable(_index);
    if (!currentTable)
    {
        mySubGroupBox->hide();
        return;
    }

    mySubGroupBox->show();
    auto strings = currentTable->GetStringItems();
    for (const auto& str : strings)
    {
        mySearchList->AddItemAtEnd_NoEmit(str.GetIdentifier());
    }

    RefreshItemCount(_index);
}
void QPanelString::OnItemEdited(const int _index, const QString& _value)
{
    DB_Manager::GetDB_Manager().RenameStringTable(_index, _value);
}
void QPanelString::OnItemAdded(const int _index, const QString& _value)
{
    DB_Manager::GetDB_Manager().AddStringTable(_value, _index);
}
void QPanelString::OnItemDuplicated(const int _index, const int _originalIndex)
{
    DB_Manager::GetDB_Manager().DuplicateStringTable(_index, _originalIndex);
}
void QPanelString::OnItemMoved(const int _indexFrom, const int _indexTo)
{
    DB_Manager::GetDB_Manager().MoveStringTable(_indexFrom, _indexTo);
}
void QPanelString::OnItemRemoved(const int _index)
{
    DB_Manager::GetDB_Manager().RemoveStringTable(_index);
}


#define GET_CURRENT_STRING_TABLE() \
const int currentTableIndex = myItemList->GetCurrent(); \
SStringTable* currentTable = DB_Manager::GetDB_Manager().GetStringTable(currentTableIndex); \
if (!currentTable) \
    return


void QPanelString::OnSubItemSelected(const int _index)
{
    GET_CURRENT_STRING_TABLE();
    DB_Manager::GetDB_Manager().AskFocusOnStringItem(currentTableIndex, _index);
}
void QPanelString::OnSubItemEdited(const int _index, const QString& _value)
{
    GET_CURRENT_STRING_TABLE();
    currentTable->SetItemIdentifier(_index, _value);
    DB_Manager::GetDB_Manager().AskUpdateOnStringTable(currentTableIndex);
}
void QPanelString::OnSubItemAdded(const int _index, const QString& _value)
{
    GET_CURRENT_STRING_TABLE();
    currentTable->AddStringItem(_index, &_value);
    DB_Manager::GetDB_Manager().AskUpdateOnStringTable(currentTableIndex);
    RefreshItemCount(currentTableIndex);
}
void QPanelString::OnSubItemDuplicated(const int _index, const int _originalIndex)
{
    GET_CURRENT_STRING_TABLE();
    auto* original = currentTable->GetStringItem(_originalIndex);
    if (!original)
        return;

    QString _texts[SStringHelper::SStringLanguages::Count];
    for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
    {
        _texts[i] = original->GetString((SStringHelper::SStringLanguages)i);
    }

    currentTable->AddStringItemWithTexts(_index, _texts, &original->GetIdentifier());
    DB_Manager::GetDB_Manager().AskUpdateOnStringTable(currentTableIndex);
    RefreshItemCount(currentTableIndex);
}
void QPanelString::OnSubItemMoved(const int _indexFrom, const int _indexTo)
{
    GET_CURRENT_STRING_TABLE();
    currentTable->MoveStringItems(_indexFrom, _indexTo);
    DB_Manager::GetDB_Manager().AskUpdateOnStringTable(currentTableIndex);
}
void QPanelString::OnSubItemRemoved(const int _index)
{
    GET_CURRENT_STRING_TABLE();
    currentTable->RemoveStringItem(_index);
    DB_Manager::GetDB_Manager().AskUpdateOnStringTable(currentTableIndex);
    int selectionIndex = _index;
    if(currentTable->GetStringItemCount() == selectionIndex)
        selectionIndex--;
    DB_Manager::GetDB_Manager().AskFocusOnStringItem(currentTableIndex, selectionIndex);
    RefreshItemCount(currentTableIndex);
}
