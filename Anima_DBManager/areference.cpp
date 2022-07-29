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



Attribute* AReference::CreateDuplica() const
{
    return new AReference(mySharedParam, myStructureRef);
}
QString AReference::GetDisplayedText(bool complete) const
{
    if(!mySharedParam.structTable || !myStructureRef)
        return complete ? "Ø" : "<font color=\"darkred\">Ø</font>";

    else if (complete)
    {
        QString structString = "";
        if (myStructureRef->IsOneOfMyAttributes(this))
            structString = "this";
        else
            myStructureRef->GetAttributesDisplayedText(structString);
        return "&{ " + structString + " }";
    }

    return mySharedParam.structTable->GetStructureRowName(myStructureRef);
}
void AReference::WriteValue_CSV(std::ofstream& file) const
{
    if(mySharedParam.structTable && myStructureRef)
        file << mySharedParam.structTable->GetStructureRowName(myStructureRef).toStdString();
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
    if (!ok)
    {
        qFatal("\n\nInvalid Index given while setting <REFERENCE> Attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }

    if (mySharedParam.structTable && structIndex > -1)
        SetReference(mySharedParam.structTable->GetStructureAt(structIndex));
}
void AReference::CopyValueFromOther(const Attribute* _other)
{
    const AReference* other_AR = dynamic_cast<const AReference*>(_other);
    if (!other_AR || mySharedParam.structTable != other_AR->mySharedParam.structTable)
        return;

    myStructureRef = other_AR->myStructureRef;
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
