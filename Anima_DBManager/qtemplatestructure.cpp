#include "qtemplatestructure.h"

#include "db_manager.h"


QTemplateStructure::QTemplateStructure(QWidget *parent)
    : QtemplateStructureBase{parent},
      myTemplateStructureCopy{nullptr}
{
    UpdateContent();
}


void QTemplateStructure::UpdateAttributeTabText(int index)
{
    if (myTemplateStructureCopy == nullptr)
        return;

    const auto* attrTmpl = myTemplateStructureCopy->GetAttributeTemplate(index);
    Q_ASSERT(attrTmpl);

    const QString tabNameBase = "%1\n[%2]";
    myTabWidget->setTabText(index, tabNameBase.arg(attrTmpl->GetName(), AttributeTypeHelper::TypeToString(attrTmpl->GetType())));
    Q_ASSERT(attrTmpl->GetName() == myAttributeNames[index]);
}

void QTemplateStructure::SetStructureDB(const StructureDB* _structureDB)
{
    myTemplateStructureCopy = new TemplateStructure(_structureDB->GetTemplate());
    UpdateContent();
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

    const QString tabNameBase = "%1\n[%2]";
    for (const auto* attr : myTemplateStructureCopy->GetAttributes())
    {
        QTemplateAttribute* qattr = new QTemplateAttribute();
        myAttributeNames.push_back(attr->GetName());
        qattr->UpdateTemplateAttribute(attr);
        myTabWidget->addTab(qattr, tabNameBase.arg(attr->GetName(), AttributeTypeHelper::TypeToString(attr->GetType())));
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
void QTemplateStructure::OnNameChanged(const QString& _previousName, QString& _newName)
{
    int index = myAttributeNames.indexOf(_previousName);
    if (index < 0)
        return;

    DB_Manager::GetDB_Manager().RenameStructureAttribute(myTemplateStructureCopy->GetStructName(), index, _newName);
    myAttributeNames[index] = _newName;
    UpdateAttributeTabText(index);
}
void QTemplateStructure::OnApply(const QString& _attrName,  const TemplateAttribute& _editedTemplateCopy, bool _hasCriticalChanges)
{
    int index = myAttributeNames.indexOf(_attrName);
    if (index < 0)
        return;

    DB_Manager::GetDB_Manager().ChangeAttributeTemplate(myTemplateStructureCopy->GetStructName(), index, _editedTemplateCopy, _hasCriticalChanges);
    /*OnRevert(_attrName);

    if (_hasCriticalChanges)
        UpdateAttributeTabText(index);*/
    emit AttributeChangeApplied();
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
    UpdateContent();
    myTabWidget->setCurrentIndex(_position);
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
}
void QTemplateStructure::OnRemove()
{
    int current = myTabWidget->currentIndex();
    if (current < 0)
        return;

    DB_Manager::GetDB_Manager().RemoveAttributeTemplate(myTemplateStructureCopy->GetStructName(), current);
    UpdateContent();
    myTabWidget->setCurrentIndex(current);
}
