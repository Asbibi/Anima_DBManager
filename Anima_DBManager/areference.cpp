#include "areference.h"

#include "savemanager.h"
#include "structure.h"
#include "structuredb.h"

#include <QDebug>
#include <QJsonDocument>

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
        return "<font color=\"blue\">this</font>";

    QJsonObject refStructValue = myStructureRef->WriteValue_JSON_AsRow();
    return QJsonDocument(refStructValue).toJson(QJsonDocument::Compact);
}
QString AReference::GetValue_CSV() const
{
    return (MY_SHARED_PARAM.structTable && myStructureRef) ?
        MY_SHARED_PARAM.structTable->GetStructureRowName(myStructureRef) :
        "";
}
QJsonValue AReference::GetValue_JSON() const
{
    return QJsonValue(GetValue_CSV());
}
void AReference::SetValueFromText(const QString& _text)
{
    if (_text.isEmpty())
    {
        SetReference(nullptr);
        return;
    }

    StructureDB* structTable = MY_SHARED_PARAM.structTable;
    Q_ASSERT(structTable != nullptr);

    QString contentText = _text;
    bool ok;
    int structIndex = (contentText.remove(structTable->GetTemplateAbbrev())).toInt(&ok);
    Q_ASSERT(ok);

    // When opening a Savefile, we fill up the the referenced Struct table if the referenced structure doesn't exist yet
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
bool AReference::SetValue_JSON(const QJsonValue& _value)
{
    if (!_value.isString())
    {
        return false;
    }

    SetValueFromText(_value.toString());
    return true;
}
void AReference::SetValue_CSV(const QString& _text)
{
    SetValueFromText(_text);
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
