#include "qtemplatestructure.h"

#include "db_manager.h"


QTemplateStructure::QTemplateStructure(QWidget *parent)
    : QtemplateStructureBase{parent},
      myTemplateStructureCopy{nullptr}
{
    UpdateContent();
}



QString QTemplateStructure::ComputeTabNameForAttribute(const QString& _name, const AttributeTypeHelper::Type& _type)
{
    static const QString tabNameBase = "%1\n[%2]";
    return tabNameBase.arg(_name, AttributeTypeHelper::TypeToString(_type));
}

void QTemplateStructure::SetStructureDB(const StructureDB* _structureDB)
{    
    myTemplateStructureCopy = _structureDB != nullptr ? new TemplateStructure(_structureDB->GetTemplate()) : nullptr;
    UpdateContent();
}
void QTemplateStructure::FocusAttribute(const int _index)
{
    myTabWidget->setCurrentIndex(_index);
}

void QTemplateStructure::UpdateContent()
{
    myAttributeNames.clear();
    myTabWidget->clear();
    bool hasStructPtr = (myTemplateStructureCopy != nullptr);
    myTabWidget->setEnabled(hasStructPtr);
    for (auto* btn : myPushBtns)
    {
        btn->setEnabled(hasStructPtr);
    }

    if (!hasStructPtr)
    {
        return;
    }

    for (const auto* attr : myTemplateStructureCopy->GetAttributes())
    {
        QTemplateAttribute* qattr = new QTemplateAttribute();
        myAttributeNames.push_back(attr->GetName());
        qattr->UpdateTemplateAttribute(attr);
        myTabWidget->addTab(qattr, ComputeTabNameForAttribute(attr->GetName(), attr->GetType()));
        QObject::connect(qattr, &QTemplateAttribute::NameChanged, this, &QTemplateStructure::OnNameChanged);
        QObject::connect(qattr, &QTemplateAttribute::Applied, this, &QTemplateStructure::OnApply);
        QObject::connect(qattr, &QTemplateAttribute::Reverted, this, &QTemplateStructure::OnRevert);
        QObject::connect(qattr, &QTemplateAttribute::AppliedDefaultToAll, this, &QTemplateStructure::OnApplyDefaultToAll);
    }
}



void QTemplateStructure::OnTabMoved(int _indexFrom, int _indexTo)
{
    auto item = myAttributeNames.takeAt(_indexFrom);
    myAttributeNames.insert(_indexTo, item);
    DB_Manager::GetDB_Manager().MoveStructureAttribute(myTemplateStructureCopy->GetStructName(), _indexFrom, _indexTo);
}
void QTemplateStructure::OnNameChanged_Index(int _attributeIndex, QString& _newName)
{
    if (_attributeIndex < 0)
        return;

    DB_Manager::GetDB_Manager().RenameStructureAttribute(myTemplateStructureCopy->GetStructName(), _attributeIndex, _newName);
    myAttributeNames[_attributeIndex] = _newName;

    emit RequestUpdateTemplateCopy(_attributeIndex);
}
void QTemplateStructure::OnNameChanged(const QString& _previousName, QString& _newName)
{
    int index = myAttributeNames.indexOf(_previousName);
    OnNameChanged_Index(index, _newName);
}
void QTemplateStructure::OnApply(const QString& _attrName,  const TemplateAttribute& _editedTemplateCopy, bool _hasCriticalChanges)
{
    int index = myAttributeNames.indexOf(_attrName);
    if (index < 0)
        return;

    DB_Manager::GetDB_Manager().ChangeAttributeTemplate(myTemplateStructureCopy->GetStructName(), index, _editedTemplateCopy, _hasCriticalChanges);

    emit AttributeChangeApplied();

    myTabWidget->setCurrentIndex(index);
}
void QTemplateStructure::OnRevert(const QString& _attrName)
{
    int index = myAttributeNames.indexOf(_attrName);
    if (index < 0)
        return;

    QTemplateAttribute* qattr = dynamic_cast<QTemplateAttribute*>(myTabWidget->widget(index));
    if (!qattr)
        return;

    qattr->UpdateTemplateAttribute(myTemplateStructureCopy->GetAttributeTemplate(index));
}
void QTemplateStructure::OnApplyDefaultToAll(const QString& _attrName)
{
    int index = myAttributeNames.indexOf(_attrName);
    if (index < 0)
        return;

    DB_Manager::GetDB_Manager().ResetAttributesToDefaultValue(myTemplateStructureCopy->GetStructName(), index);
}



void QTemplateStructure::AddAttribute(int _position, bool _duplicatePrevious)
{
    DB_Manager::GetDB_Manager().AddAttributeTemplate(myTemplateStructureCopy->GetStructName(), _position, _duplicatePrevious);
    emit RequestUpdateTemplateCopy(_position);
}
void QTemplateStructure::OnAddBefore()
{
    int position = myTabWidget->currentIndex();
    if (position < 0)
        position = 0;
    AddAttribute(position);
}
void QTemplateStructure::OnAddAfter()
{
    int position = myTabWidget->currentIndex() + 1;
    AddAttribute(position);
}
void QTemplateStructure::OnDuplicate()
{
    int current = myTabWidget->currentIndex();
    if (current < 0)
        return;

    AddAttribute(current + 1, true);
    QString currentName = myAttributeNames[current + 1] + "_1";
    OnNameChanged_Index(current + 1, currentName);
}
void QTemplateStructure::OnRemove()
{
    int current = myTabWidget->currentIndex();
    if (current < 0)
        return;

    DB_Manager::GetDB_Manager().RemoveAttributeTemplate(myTemplateStructureCopy->GetStructName(), current);
    emit RequestUpdateTemplateCopy(current);
}
