#include "qpanelstring.h"
#include "db_manager.h"

QPanelString::QPanelString(QWidget *parent)
    : QPanelBase{"String Table", true, parent}
{}


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
