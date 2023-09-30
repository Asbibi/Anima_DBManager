#include "qpanelstruct.h"

#include "db_manager.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

QPanelStruct::QPanelStruct(QWidget* parent)
    : QPanelBase{"Structure DB", true, parent}
{
    QLayout* myLayout = layout();

    QGroupBox* editGroupBox = new QGroupBox("Edit Selected Structure");
    QFormLayout* editLayout = new QFormLayout();
    editGroupBox->setLayout(editLayout);
    myLayout->addWidget(editGroupBox);

    myStructIdentity = new QStructIdentity();
    editLayout->addRow("Identity:", myStructIdentity);

    myTemplateEditor = new QTemplateStructure();
    QObject::connect(myTemplateEditor, &QTemplateStructure::AttributeChangeApplied, this, &QPanelStruct::OnItemApplied);
    editLayout->addRow("Attributes:", myTemplateEditor);

    myElementHandler = new QElementHandler();
    editLayout->addRow("Item:", myElementHandler);
    QHBoxLayout* numLayout = new QHBoxLayout();
    myRowCountSpinner = new QSpinBox();
    numLayout->addWidget(myRowCountSpinner);
    QPushButton* myPushBtn = new QPushButton("Set Item Count");
    myPushBtn->setMaximumWidth(90);
    numLayout->addWidget(myPushBtn);
    editLayout->addRow("Count:", numLayout);
    QObject::connect(myElementHandler, &QElementHandler::SpinBoxSelected, this, &QPanelStruct::OnElementSelected);
    QObject::connect(myElementHandler, &QElementHandler::AddRequested, this, &QPanelStruct::OnElementAdded);
    QObject::connect(myElementHandler, &QElementHandler::DuplicateRequested, this, &QPanelStruct::OnElementDuplicated);
    QObject::connect(myElementHandler, &QElementHandler::MoveRequested, this, &QPanelStruct::OnElementMoved);
    QObject::connect(myElementHandler, &QElementHandler::RemoveRequested, this, &QPanelStruct::OnElementRemoved);
    QObject::connect(myPushBtn, &QPushButton::clicked, this, &QPanelStruct::SetItemCount);
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
    myStructIdentity->SetValueFromTemplate(currentStructDB->GetTemplate());
    myTemplateEditor->SetStructureDB(currentStructDB);
    myElementHandler->OnSelectElement(-1, "");
    if (currentStructDB != nullptr)
    {
        RefreshItemCount(_index);
    }
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
void QPanelStruct::OnItemApplied()
{
    int currentIndex = myItemList->GetCurrent();
    if (currentIndex < 0)
    {
        return;
    }
    OnItemSelected(currentIndex);
}



void QPanelStruct::OnElementSelected(const int _index)
{
    const auto* selectedStructureDB = GetMyStructureDB();
    if (selectedStructureDB)
    {
        const auto* structure = selectedStructureDB->GetStructureAt(_index);
        if (structure)
        {
            myElementHandler->OnSelectElement(_index, structure->GetDisplayText());
            return;
        }
    }

    myElementHandler->OnSelectElement(-1, "");
}

void QPanelStruct::OnElementAdded(const int _index)
{
    const int tableIndex = GetSelectedItem();
    DB_Manager::GetDB_Manager().AddStructureRow(tableIndex, _index);
    OnElementSelected(_index);
    RefreshItemCount(tableIndex);
}
void QPanelStruct::OnElementDuplicated(const int _index, const int _originalIndex)
{
    const int tableIndex = GetSelectedItem();
    DB_Manager::GetDB_Manager().DuplicateStructureRow(tableIndex, _index, _originalIndex);
    OnElementSelected(_index);
    RefreshItemCount(tableIndex);
}
void QPanelStruct::OnElementMoved(const int _indexFrom, int _indexTo)
{
    DB_Manager::GetDB_Manager().MoveStructureRow(GetSelectedItem(), _indexFrom, _indexTo);
    OnElementSelected(_indexTo);
}
void QPanelStruct::OnElementRemoved(const int _index)
{
    const int tableIndex = GetSelectedItem();
    DB_Manager::GetDB_Manager().RemoveStructureRow(tableIndex, _index);
    OnElementSelected(_index);
    RefreshItemCount(tableIndex);
}


void QPanelStruct::RefreshItemCount(const int _tableIndex)
{
    const int count = DB_Manager::GetDB_Manager().GetStructureTable(_tableIndex)->GetStructureCount();
    myRowCountSpinner->setValue(count);
}
void QPanelStruct::SetItemCount()
{
    const int count = myRowCountSpinner->value();
    DB_Manager::GetDB_Manager().SetStructureRowCount(GetSelectedItem(), count);
}
