#include "qtemplatestructurecore.h"

#include "qtemplateattributecore.h"

QTemplateStructureCore::QTemplateStructureCore(TemplateStructure& _templateStruct, QWidget *parent)
    : QtemplateStructureBase{parent}
    , myTemplateStruct{_templateStruct}
{
    UpdateContent();
}


QString QTemplateStructureCore::ComputeTabNameForAttribute(const AttributeTypeHelper::Type& _type)
{
    static const QString tabNameBase = "[%1]";
    return tabNameBase.arg(AttributeTypeHelper::TypeToString(_type));
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
void QTemplateStructureCore::OnAttributeTypeChanged(const int _index, const AttributeTypeHelper::Type _type)
{
    Q_ASSERT(_index < myTabWidget->count());
    myTabWidget->setTabText(_index, ComputeTabNameForAttribute(_type));
}

void QTemplateStructureCore::UpdateContent()
{
    myTemplateAttributeCoreList.clear();
    myTabWidget->clear();
    int i = 0;
    for (auto* attr : myTemplateStruct.GetAttributesW())
    {
        QTemplateAttributeCore* qattr = new QTemplateAttributeCore(*attr);
        myTabWidget->addTab(qattr, ComputeTabNameForAttribute(attr->GetType()));
        QObject::connect(qattr, &QTemplateAttributeCore::ParamEdited, this, &QTemplateStructureCore::OnAttributeEdited);
        QObject::connect(qattr, &QTemplateAttributeCore::TypeChanged, this, [this, i](const AttributeTypeHelper::Type _type){ OnAttributeTypeChanged(i, _type); });
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
