#include "qpanelstruct.h"

#include "db_manager.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

QPanelStruct::QPanelStruct(QWidget* parent)
    : QPanelBase{"Structure DB", true, parent}
{
    QLayout* myLayout = layout();

    QGroupBox* editGroupBox = new QGroupBox("Edit Selected String Table");
    QFormLayout* editLayout = new QFormLayout();
    editGroupBox->setLayout(editLayout);
    myLayout->addWidget(editGroupBox);


    QGridLayout* clearLayout = new QGridLayout();
    clearLayout->addWidget(new QLineEdit(), 0,0);
    clearLayout->addWidget(new QPushButton("Clear Those"), 0,1);
    clearLayout->addWidget(new QPushButton("Clear All"), 1,1);
    editLayout->addRow("Clear:", clearLayout);
    QHBoxLayout* numLayout = new QHBoxLayout();
    numLayout->addWidget(new QSpinBox());
    QPushButton* myPushBtn = new QPushButton("Set Item Count");
    myPushBtn->setMaximumWidth(90);
    numLayout->addWidget(myPushBtn);
    editLayout->addRow("Count:", numLayout);
}


void QPanelStruct::UpdateItemList()
{
    myItemList->Clear();
    auto& DB = DB_Manager::GetDB_Manager();
    const int dbCount = DB.GetStructuresCount();
    for (int i = 0; i < dbCount; i++)
    {
        myItemList->AddItemAt(DB.GetStructureTable(i)->GetTemplateName());
    }
}


void QPanelStruct::OnItemSelected(const int _index)
{
    // Reset sub area

    /*StructureDB* currentStructDB = DB_Manager::GetDB_Manager().GetStructureTable(_index);
    if (!currentStructDB)
        return;
    */

}
void QPanelStruct::OnItemEdited(const int _index, const QString& _value)
{
    DB_Manager::GetDB_Manager().RenameStructureDB(_index, _value);
}
void QPanelStruct::OnItemAdded(const int _index, const QString& _value)
{
    DB_Manager::GetDB_Manager().AddStructureDB({_value, QColorConstants::LightGray}, _index);
}
void QPanelStruct::OnItemDuplicated(const int _index, const int _originalIndex)
{
    DB_Manager::GetDB_Manager().DuplicateStructureDB(_index, _originalIndex);
}
void QPanelStruct::OnItemMoved(const int _indexFrom, const int _indexTo)
{
    DB_Manager::GetDB_Manager().MoveStructureDB(_indexFrom, _indexTo);
}
void QPanelStruct::OnItemRemoved(const int _index)
{
    DB_Manager::GetDB_Manager().RemoveStructureDB(_index);
}
