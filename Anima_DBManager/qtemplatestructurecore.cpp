#include "qtemplatestructurecore.h"

#include "qtemplateattributecore.h"

QTemplateStructureCore::QTemplateStructureCore(TemplateStructure& _templateStruct, QWidget *parent)
    : QtemplateStructureBase{parent}
    , myTemplateStruct{_templateStruct}
{
    UpdateContent();
}


QString QTemplateStructureCore::ComputeTabNameForAttribute(const QString& _name, const AttributeTypeHelper::Type& _type)
{
    static const QString tabNameBase = "%1\n[%2]";
    return tabNameBase.arg(_name, AttributeTypeHelper::TypeToString(_type));
}

bool QTemplateStructureCore::HasConfigValid() const
{
    for (const auto* qattr : myTemplateAttributeCoreList)
    {
        if (!qattr->HasConfigValid())
        {
            return false;
        }
    }
    return true;
}
void QTemplateStructureCore::ShowDefaultWidget(bool _show)
{
    for (auto* qattr : myTemplateAttributeCoreList)
    {
        qattr->ShowDefaultWidget(_show);
    }
}
void QTemplateStructureCore::UpdateTabName(const int _index)
{
    Q_ASSERT(_index < myTabWidget->count() && _index < myTemplateStruct.GetAttributesCount());
    const auto* templAttr = myTemplateStruct.GetAttributeTemplate(_index);
    const QString& name = templAttr->GetName();
    const AttributeTypeHelper::Type type = templAttr->GetType();
    myTabWidget->setTabText(_index, ComputeTabNameForAttribute(name, type));
}
void QTemplateStructureCore::RenameAttribute(const int _index, QString& _name)
{
    myTemplateStruct.RenameAttributeTemplate(_index, _name);
    UpdateTabName(_index);
}



void QTemplateStructureCore::UpdateContent()
{
    myTemplateAttributeCoreList.clear();
    myTabWidget->clear();
    int i = 0;
    for (auto* attr : myTemplateStruct.GetAttributesW())
    {
        QTemplateAttributeCore* qattr = new QTemplateAttributeCore(*attr, true);
        myTabWidget->addTab(qattr, ComputeTabNameForAttribute(attr->GetName(), attr->GetType()));
        QObject::connect(qattr, &QTemplateAttributeCore::ParamEdited, this, &QTemplateStructureCore::OnAttributeEdited);
        QObject::connect(qattr, &QTemplateAttributeCore::NameChanged, this, [this, i](QString& _name){ RenameAttribute(i, _name); });
        QObject::connect(qattr, &QTemplateAttributeCore::TypeChanged, this, [this, i](){ UpdateTabName(i); });
        myTemplateAttributeCoreList.append(qattr);
        i++;
    }
}
void QTemplateStructureCore::OnTabMoved(int _indexFrom, int _indexTo)
{
    myTemplateStruct.MoveAttribute(_indexFrom, _indexTo);
    emit StructureChanged();
}
void QTemplateStructureCore::AddAttribute(int _position, bool _duplicatePrevious)
{
    if (_duplicatePrevious)
    {
        // --- TODO
        myTemplateStruct.AddAttributeTemplate(_position);
    }
    else
    {
        myTemplateStruct.AddAttributeTemplate(_position);
    }
    UpdateContent();
    myTabWidget->setCurrentIndex(_position);
    emit StructureChanged();
}
void QTemplateStructureCore::OnAddBefore()
{
    int position = myTabWidget->currentIndex();
    if (position < 0)
        position = 0;
    AddAttribute(position);
}
void QTemplateStructureCore::OnAddAfter()
{
    int position = myTabWidget->currentIndex() + 1;
    AddAttribute(position);
}
void QTemplateStructureCore::OnDuplicate()
{
    int current = myTabWidget->currentIndex();
    if (current < 0)
        return;

    AddAttribute(current + 1, true);
}
void QTemplateStructureCore::OnRemove()
{
    int current = myTabWidget->currentIndex();
    if (current < 0)
        return;

    myTemplateStruct.RemoveAttribute(current);
    UpdateContent();
    myTabWidget->setCurrentIndex(current);
    emit StructureChanged();
}
void QTemplateStructureCore::OnAttributeEdited(bool _withCriticalChanges)
{
    emit StructureChanged(_withCriticalChanges);
}
