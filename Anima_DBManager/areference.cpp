#include "areference.h"

#include "savemanager.h"
#include "structure.h"
#include "structuredb.h"

#include <QDebug>

AReference::AReference(TemplateAttribute& _template) :
    Attribute(_template),
    myStructureRef(nullptr)
{}


QString AReference::GetDisplayedText() const
{
    if(!MY_SHARED_PARAM.structTable || !myStructureRef)
        return "<font color=\"darkred\">Ø</font>";

    QString structString = "";
    if (myStructureRef->IsOneOfMyAttributes(this))
        structString = "this";
    else
        // return MY_SHARED_PARAM.structTable->GetStructureRowName(myStructureRef) ?
        myStructureRef->GetAttributesDisplayedText(structString);
    return "&{ " + structString + " }";

}
QString AReference::GetValueAsText() const
{
    return '&'
        + QString((MY_SHARED_PARAM.structTable && myStructureRef) ?
            QString::number(MY_SHARED_PARAM.structTable->GetStructureIndex(myStructureRef)) :
            "Ø");
}
QString AReference::GetAttributeAsCSV() const
{
    return (MY_SHARED_PARAM.structTable && myStructureRef) ?
        MY_SHARED_PARAM.structTable->GetStructureRowName(myStructureRef) :
        "";
}
QJsonValue AReference::GetAttributeAsJSON() const
{
    return QJsonValue(GetAttributeAsCSV());
}
void AReference::SetValueFromText(const QString& text)
{
    Q_ASSERT(text[0] == '&');
    QString contentText = text;

    bool ok;
    int structIndex = (contentText.remove('&')).toInt(&ok);
    StructureDB* structTable = MY_SHARED_PARAM.structTable;
    if (!ok || !structTable || structIndex < 0)
    {
        SetReference(nullptr);
        return;
    }

    if (structIndex >= structTable->GetStructureCount() && SaveManager::IsOpeningFile())
    {
        structTable->SetStructureCount(structIndex+1);
    }
    SetReference(structTable->GetStructureAt(structIndex));
}
void AReference::CopyValueFromOther(const Attribute* _other)
{
    const AReference* other_AR = dynamic_cast<const AReference*>(_other);
    if (!other_AR || MY_SHARED_PARAM.structTable != other_AR->MY_SHARED_PARAM.structTable)
        return;

    myStructureRef = other_AR->myStructureRef;
}
bool AReference::ReadValue_JSON(const QJsonValue& _value)
{
    if (!_value.isString())
    {
        return false;
    }

    SetValueFromText(ConvertRowNameToTextValue(_value.toString()));
    return true;
}
void AReference::ReadValue_CSV(const QString& _text)
{
    SetValueFromText(ConvertRowNameToTextValue(_text));
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
    return MY_SHARED_PARAM.structTable;
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

QString AReference::ConvertRowNameToTextValue(const QString& _rowName) const
{
    return '&' + QString(_rowName).remove(MY_SHARED_PARAM.structTable->GetTemplateAbbrev());
}
