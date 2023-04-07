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

    QGroupBox* editGroupBox = new QGroupBox("Edit Selected Structure");
    QFormLayout* editLayout = new QFormLayout();
    editGroupBox->setLayout(editLayout);
    myLayout->addWidget(editGroupBox);

    myTemplateEditor = new QTemplateStructure();
    editLayout->addRow("Attributes:", myTemplateEditor);

    myElementHandler = new QElementHandler();
    editLayout->addRow("Current:", myElementHandler);
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
    QObject::connect(myElementHandler, &QElementHandler::SpinBoxSelected, this, &QPanelStruct::OnElementSelected);
    QObject::connect(myElementHandler, &QElementHandler::AddRequested, this, &QPanelStruct::OnElementAdded);
    QObject::connect(myElementHandler, &QElementHandler::DuplicateRequested, this, &QPanelStruct::OnElementDuplicated);
    QObject::connect(myElementHandler, &QElementHandler::MoveRequested, this, &QPanelStruct::OnElementMoved);
    QObject::connect(myElementHandler, &QElementHandler::RemoveRequested, this, &QPanelStruct::OnElementRemoved);
}


StructureDB* QPanelStruct::GetMyStructureDB()
{
    return DB_Manager::GetDB_Manager().GetStructureTable(GetSelectedItem());
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
    StructureDB* currentStructDB = DB_Manager::GetDB_Manager().GetStructureTable(_index);
    myTemplateEditor->SetStructureDB(currentStructDB);
    myElementHandler->OnSelectElement(-1, "");
}
void QPanelStruct::OnItemEdited(const int _index, const QString& _value)
{
    DB_Manager::GetDB_Manager().RenameStructureDB(_index, _value);
    OnItemSelected(_index);
}
void QPanelStruct::OnItemAdded(const int _index, const QString& _value)
{
    DB_Manager::GetDB_Manager().AddStructureDB({_value, QColorConstants::LightGray}, _index);
    OnItemSelected(_index);
}
void QPanelStruct::OnItemDuplicated(const int _index, const int _originalIndex)
{
    DB_Manager::GetDB_Manager().DuplicateStructureDB(_index, _originalIndex);
    OnItemSelected(_index);
}
void QPanelStruct::OnItemMoved(const int _indexFrom, const int _indexTo)
{
    DB_Manager::GetDB_Manager().MoveStructureDB(_indexFrom, _indexTo);
    OnItemSelected(_indexTo);
}
void QPanelStruct::OnItemRemoved(const int _index)
{
    DB_Manager::GetDB_Manager().RemoveStructureDB(_index);
    OnItemSelected(_index);
}



void QPanelStruct::OnElementSelected(const int _index)
{
    const auto* structure = GetMyStructureDB()->GetStructureAt(_index);
    if (structure)
        myElementHandler->OnSelectElement(_index, structure->GetDisplayText());
    else
        myElementHandler->OnSelectElement(-1, "");
}

void QPanelStruct::OnElementAdded(const int _index)
{
    DB_Manager::GetDB_Manager().AddStructureRow(GetSelectedItem(), _index);
    OnElementSelected(_index);
}
void QPanelStruct::OnElementDuplicated(const int _index, const int _originalIndex)
{
    DB_Manager::GetDB_Manager().DuplicateStructureRow(GetSelectedItem(), _index, _originalIndex);
    OnElementSelected(_index);
}
void QPanelStruct::OnElementMoved(const int _indexFrom, int _indexTo)
{
    DB_Manager::GetDB_Manager().MoveStructureRow(GetSelectedItem(), _indexFrom, _indexTo);
    OnElementSelected(_indexTo);
}
void QPanelStruct::OnElementRemoved(const int _index)
{
    DB_Manager::GetDB_Manager().RemoveStructureRow(GetSelectedItem(), _index);
    OnElementSelected(_index);
}
