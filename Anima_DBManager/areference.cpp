#include "areference.h"

#include "structure.h"
#include "structuredb.h"

#include <QDebug>

AReference::AReference(const AttributeParam& _sharedParam) :
    Attribute(_sharedParam)
{
    if (_sharedParam.structTable == nullptr)
        qFatal("\n\nNull StructureDB Attribute given when instancing <REFERENCE> Attribute:\n\n\t===== Not allowed =====\n\n");
}
AReference::AReference(const AttributeParam& _sharedParam, QPointer<const Structure> _structureRef) :
    Attribute(_sharedParam),
    myStructureRef(_structureRef)
{
    if (_sharedParam.structTable == nullptr)
        qFatal("\n\nNull StructureDB Attribute given when instancing <REFERENCE> Attribute:\n\n\t===== Not allowed =====\n\n");
}


QString AReference::GetDisplayedText() const
{
    if(!mySharedParam.structTable || !myStructureRef)
        return "<font color=\"darkred\">Ø</font>";

    QString structString = "";
    if (myStructureRef->IsOneOfMyAttributes(this))
        structString = "this";
    else
        // return mySharedParam.structTable->GetStructureRowName(myStructureRef) ?
        myStructureRef->GetAttributesDisplayedText(structString);
    return "&{ " + structString + " }";

}
QString AReference::GetValueAsText() const
{
    return '&'
        + QString((mySharedParam.structTable && myStructureRef) ?
            QString::number(mySharedParam.structTable->GetStructureIndex(myStructureRef)) :
            "Ø");
}
QString AReference::GetAttributeAsCSV() const
{
    return (mySharedParam.structTable && myStructureRef) ?
        mySharedParam.structTable->GetStructureRowName(myStructureRef) :
        "";
}
void AReference::SetValueFromText(const QString& text)
{
    QString contentText = text;
    if (contentText[0] != '&')
    {
        qFatal("\n\nMissing start '&' character while setting <REFERENCE> Attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }
    bool ok;
    int structIndex = (contentText.remove('&')).toInt(&ok);
    if (!ok || !mySharedParam.structTable || structIndex < 0)
    {
        SetReference(nullptr);
        return;
    }

    SetReference(mySharedParam.structTable->GetStructureAt(structIndex));
}
void AReference::CopyValueFromOther(const Attribute* _other)
{
    const AReference* other_AR = dynamic_cast<const AReference*>(_other);
    if (!other_AR || mySharedParam.structTable != other_AR->mySharedParam.structTable)
        return;

    myStructureRef = other_AR->myStructureRef;
}
void AReference::ReadValue_CSV(const QString& _text)
{
    SetValueFromText('&' + QString(_text).remove(mySharedParam.structTable->GetTemplateAbbrev()));
}



void AReference::SetReference(const Structure* _reference)
{
    bool changed = myStructureRef != _reference;
    myStructureRef = _reference;

    if (changed)
    {
        EmitValueChanged();
    }
}
const Structure* AReference::GetReference() const
{
    return myStructureRef;
}
const StructureDB* AReference::GetStructureDB() const
{
    return mySharedParam.structTable;
}
int AReference::GetReferenceIndex() const
{
    if (!myStructureRef)
        return -1;

    const auto& structs = GetStructureDB()->GetStructures();
    const int count = (int)(structs.size());
    for (int i = 0; i < count; i++)
    {
        if (myStructureRef == structs[i])
            return i;
    }

    qCritical() << "My Ref not found in structs: should be null";
    return -1;
}
