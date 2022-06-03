#include "areference.h"

#include "structure.h"
#include "structuredb.h"

AReference::AReference(const AttributeParam* _sharedParam) :
    Attribute(_sharedParam)
{
    if (_sharedParam == nullptr)
        qFatal("\n\nNull param given when instancing <REFERENCE> Attribute:\n\n\t===== Not allowed =====\n\n");
    else if (_sharedParam->structTable == nullptr)
        qFatal("\n\nNull StructureDB Attribute given when instancing <REFERENCE> Attribute:\n\n\t===== Not allowed =====\n\n");
}
AReference::AReference(const AttributeParam* _sharedParam, QPointer<const Structure> _structureRef) :
    Attribute(_sharedParam),
    myStructureRef(_structureRef)
{
    if (_sharedParam == nullptr)
        qFatal("\n\nNull param given when instancing <REFERENCE> Attribute:\n\n\t===== Not allowed =====\n\n");
    else if (_sharedParam->structTable == nullptr)
        qFatal("\n\nNull StructureDB Attribute given when instancing <REFERENCE> Attribute:\n\n\t===== Not allowed =====\n\n");
}



Attribute* AReference::CreateDuplica() const
{
    return new AReference(sharedParam, myStructureRef);
}
QString AReference::GetDisplayedText(bool complete) const
{
    if(!myStructureRef)
        return "<font color=\"darkred\">Ø</font>";

    else if (complete)
    {
        QString structString = "";
        myStructureRef->GetAttributesDisplayedText(structString);
        return "&{" + structString + "}";
    }

    return sharedParam->structTable->GetStructureRowName(myStructureRef);
}
void AReference::WriteValue_CSV(std::ofstream& file) const
{
    if(myStructureRef)
        file << sharedParam->structTable->GetStructureRowName(myStructureRef).toStdString();
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

    if (structIndex > -1)
        SetReference(GetStructureDB()->GetStructureAt(structIndex));
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
    return sharedParam->structTable;
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
    return -1;
}
