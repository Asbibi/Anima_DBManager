#include "templateattribute.h"

#include "abool.h"
#include "areference.h"
#include "templatestructure.h"

#include <QDebug>


TemplateAttribute::TemplateAttribute() :
    myAttrName(""),
    mySharedParam()
{
    InitDefaultAttribute(AttributeTypeHelper::Type::Bool);
}
#ifdef TEST_VALUES
TemplateAttribute::TemplateAttribute(const QString& _name, const AttributeTypeHelper::Type _type, const AttributeParam& _sharedParamToCopy) :
    myAttrName(_name),
    mySharedParam(_sharedParamToCopy)
{
    qDebug() << "Template Constructed via Test Constructor";
    InitDefaultAttribute(_type);
}
#endif
TemplateAttribute::TemplateAttribute(const TemplateAttribute& _another) :
    myAttrName(_another.myAttrName),
    mySharedParam(_another.mySharedParam)
{
    InitDefaultAttribute(_another.GetType());

    Q_ASSERT(myDefaultAttribute != nullptr && _another.myDefaultAttribute != nullptr);
    Q_ASSERT(myDefaultAttribute->GetType() == _another.myDefaultAttribute->GetType());

    myDefaultAttribute->CopyValueFromOther(_another.myDefaultAttribute);
}
void TemplateAttribute::InitDefaultAttribute(AttributeTypeHelper::Type _type)
{
    if (myDefaultAttribute != nullptr)
    {
        UnregisterAttribute(myDefaultAttribute);
        delete myDefaultAttribute;
        myDefaultAttribute = nullptr;
    }

    myDefaultAttribute = AttributeTypeHelper::NewAttributeFromType(_type, *this);
    Q_ASSERT(myDefaultAttribute != nullptr);
}
void TemplateAttribute::ResetUselessParam(AttributeTypeHelper::Type _type)
{
    AttributeTypeHelper::ResetUselessAttributesForType(_type, mySharedParam);
}
TemplateAttribute::~TemplateAttribute()
{
    Q_ASSERT(myAttributes.contains(myDefaultAttribute));
    for (auto* relatedAttr : myAttributes)
    {
        delete relatedAttr;
    }
}

void TemplateAttribute::RegisterAttribute(Attribute* _attr)
{
    Q_ASSERT(_attr != nullptr && _attr->GetTemplate() == this && !myAttributes.contains(_attr));
    myAttributes.insert(_attr);
}
void TemplateAttribute::UnregisterAttribute(Attribute* _attr)
{
    Q_ASSERT(myAttributes.contains(_attr));
    myAttributes.remove(_attr);
}



const QString& TemplateAttribute::GetName() const
{
    return myAttrName;
}
AttributeTypeHelper::Type TemplateAttribute::GetType() const
{
    Q_ASSERT(myDefaultAttribute);
    return myDefaultAttribute->GetType();
}
const Attribute* TemplateAttribute::GetDefaultAttribute() const
{
    return myDefaultAttribute;
}
const AttributeParam& TemplateAttribute::GetSharedParam() const
{
    return mySharedParam;
}
AttributeParam& TemplateAttribute::GetSharedParam()
{
    return mySharedParam;
}
Attribute* TemplateAttribute::GetDefaultAttributeW()
{
    return myDefaultAttribute;
}
bool TemplateAttribute::HasValidSharedParam() const
{
    return AttributeTypeHelper::AreParamValid(GetType(), mySharedParam);
}

bool TemplateAttribute::IsSameArrayType(const AttributeParam& _firstParam, const AttributeParam& _secondParam)
{
    Q_ASSERT(_firstParam.templateAtt && _secondParam.templateAtt);
    AttributeTypeHelper::Type firstType = _firstParam.templateAtt->GetType();
    AttributeTypeHelper::Type secondType = _secondParam.templateAtt->GetType();

    if (firstType != secondType)
        return false;
    else if (firstType == AttributeTypeHelper::Type::Array)
        return IsSameArrayType(_firstParam.templateAtt->GetSharedParam(), _secondParam.templateAtt->GetSharedParam());
    else if (firstType != AttributeTypeHelper::Type::Structure)
        return IsSameStructType(_firstParam.templateAtt->GetSharedParam(), _secondParam.templateAtt->GetSharedParam());
    else
        return true;

    return IsSameArrayType(_firstParam.templateAtt->GetSharedParam(), _secondParam.templateAtt->GetSharedParam());
}
bool TemplateAttribute::IsSameStructType(const AttributeParam& _firstParam, const AttributeParam& _secondParam)
{
    Q_ASSERT(_firstParam.templateStruct && _secondParam.templateStruct);
    const int firstNumAttr = _firstParam.templateStruct->GetAttributesCount();
    const int secondNumAttr = _secondParam.templateStruct->GetAttributesCount();
    if (firstNumAttr != secondNumAttr)
        return false;

    bool allTheSame = true;
    for (int i = 0; i < firstNumAttr; i++)
    {
        const TemplateAttribute* firstAttr =_firstParam.templateStruct->GetAttributeTemplate(i);
        const TemplateAttribute* secondAttr =_secondParam.templateStruct->GetAttributeTemplate(i);
        AttributeTypeHelper::Type firstType = firstAttr->GetType();
        AttributeTypeHelper::Type secondType = secondAttr->GetType();


        if (firstType != secondType)
        {
            allTheSame = false;
        }
        else if (firstType == AttributeTypeHelper::Type::Array)
        {
            allTheSame =  IsSameArrayType(firstAttr->GetSharedParam(), secondAttr->GetSharedParam());
        }
        else if (firstType == AttributeTypeHelper::Type::Structure)
        {
            allTheSame =  IsSameStructType(firstAttr->GetSharedParam(), secondAttr->GetSharedParam());
        }

        if (!allTheSame)
            break;
    }
    return allTheSame;
}
bool TemplateAttribute::SetNewValues(const TemplateAttribute& _templateToCopy)
{
    const AttributeTypeHelper::Type newType = _templateToCopy.GetType();
    const AttributeParam& newParamToCopy = _templateToCopy.mySharedParam;
    bool softChange = GetType() == newType;
    if (softChange && newType == AttributeTypeHelper::Type::Array)
    {
        softChange = IsSameArrayType(mySharedParam, newParamToCopy);
    }
    else if (softChange && newType == AttributeTypeHelper::Type::Structure)
    {
        softChange = IsSameStructType(mySharedParam, newParamToCopy);
    }

    mySharedParam = newParamToCopy;
    if (!softChange)
    {
        InitDefaultAttribute(newType);
        ResetUselessParam(newType);
    }

    SetDefaultValue(_templateToCopy.myDefaultAttribute->GetValueAsText());

    return softChange;
}
void TemplateAttribute::SetDefaultValue(const QString& _valueAsText)
{
    myDefaultAttribute->SetValueFromText(_valueAsText);
}


Attribute* TemplateAttribute::GenerateAttribute() const
{
    Q_ASSERT(myDefaultAttribute != nullptr);
    return myDefaultAttribute->CreateDuplica();
}


void TemplateAttribute::SaveTemplate_CSV(std::ofstream& file) const
{
    file << GetTemplateAsCSV().toStdString();
}
QString TemplateAttribute::GetTemplateAsCSV() const
{
    return myAttrName + '|'
            + AttributeTypeHelper::TypeToString(GetType()) + '|'
            + mySharedParam.GetParamsAsCSV() + '|'
            + myDefaultAttribute->GetAttributeAsCSV();
}
TemplateAttribute* TemplateAttribute::NewAttribute_CSV(const QString& _csvLine, QHash<AReference*, QString>& _outRefMap)
{
    if (_csvLine.isEmpty())
    {
        return nullptr;
    }

    AttributeTypeHelper::Type type = AttributeTypeHelper::StringToType(_csvLine.section('|', 1, 1));

    AttributeParam param = AttributeParam(_csvLine.section('|', 2, -2), _outRefMap);
    TemplateAttribute* newTemplate = new TemplateAttribute(_csvLine.section('|', 0, 0), type, param);

    // Reference attribute initalisation is deleguated to the map owner
    if (type == AttributeTypeHelper::Type::Reference)
    {
        AReference* aref = dynamic_cast<AReference*>(newTemplate->GetDefaultAttributeW());
        Q_ASSERT(aref != nullptr);
        _outRefMap.insert(aref, _csvLine.section('|', -1, -1));
    }
    else
    {
        newTemplate->GetDefaultAttributeW()->ReadValue_CSV(_csvLine.section('|', -1, -1));
    }

    return newTemplate;
}
