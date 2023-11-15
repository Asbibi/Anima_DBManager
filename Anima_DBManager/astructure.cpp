#include "astructure.h"

#include<QDebug>

AStructure::AStructure(TemplateAttribute& _template) :
    AStructure(_template, nullptr)
{}
AStructure::AStructure(TemplateAttribute& _template, const Structure* _value) :
    Attribute(_template),
    myValue(nullptr)
{
    Q_ASSERT(MY_SHARED_PARAM.templateStruct != nullptr);
    if (_value == nullptr)
    {
        myValue = new Structure(*MY_SHARED_PARAM.templateStruct);
    }
    else
    {
        Q_ASSERT(MY_SHARED_PARAM.templateStruct == &(_value->GetTemplate()));
        myValue = new Structure(*_value, true);
    }
}


QString AStructure::GetDisplayedText() const
{
    // return GetValueAsText() ?
    return "{STRUCT}";
}
QString AStructure::GetValue_CSV() const
{
    return myValue->GetStructureAsCSV();
}
QJsonValue AStructure::GetValue_JSON() const
{
    return QJsonValue(myValue->WriteValue_JSON_AsRow());
}
void AStructure::CopyValueFromOther(const Attribute* _other)
{
    const AStructure* other_AS = dynamic_cast<const AStructure*>(_other);
    if (!other_AS)
        return;

    myValue = other_AS->myValue;
}
bool AStructure::SetValue_JSON(const QJsonValue& _value)
{
    if (!_value.isObject())
    {
        return false;
    }

    Q_ASSERT(myValue != nullptr);
    myValue->ReadValue_JSON(_value.toObject());
    return true;
}
void AStructure::SetValue_CSV(const QString& /*_text*/)
{
    qFatal("Not Implemented.");
}



QList<QString> AStructure::GetDisplayedTexts() const
{
    int _count = myValue->GetAttributeCount();
    QList<QString> strings = QList<QString>();
    strings.reserve(_count);
    for (int i = 0; i< _count; i++)
        strings.push_back(myValue->GetAttribute(i)->GetDisplayedText());

    return strings;
}
const QList<Attribute*>& AStructure::GetAttributes() const
{
    Q_ASSERT(myValue != nullptr);
    return myValue->GetAttributes();
}
void AStructure::ResetValueToDefaults()
{
    int _count = myValue->GetAttributeCount();
    for (int i = 0; i< _count; i++)
    {
        auto* attr = myValue->GetAttribute(i);
        attr->CopyValueFromOther(attr->GetTemplate()->GetDefaultAttribute());
    }
}
