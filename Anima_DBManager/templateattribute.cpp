#include "templateattribute.h"

#include "abool.h"
#include "areference.h"
#include "templatestructure.h"

#include <QDebug>


TemplateAttribute::TemplateAttribute() :
    myAttrName("New"),
    mySharedParam(),
    myIsActive(true)
{
    InitDefaultAttribute(AttributeTypeHelper::Type::Bool);
}
TemplateAttribute::TemplateAttribute(const QString& _name, const AttributeTypeHelper::Type _type, const AttributeParam& _sharedParamToCopy, bool _active) :
    myAttrName(_name),
    mySharedParam(_sharedParamToCopy),
    myIsActive(_active)
{
    InitDefaultAttribute(_type);
}
TemplateAttribute::TemplateAttribute(const TemplateAttribute& _another) :
    myAttrName(_another.myAttrName),
    mySharedParam(_another.mySharedParam),
    myIsActive(_another.myIsActive)
{
    InitDefaultAttribute(_another.GetType());

    Q_ASSERT(myDefaultAttribute != nullptr && _another.myDefaultAttribute != nullptr);
    Q_ASSERT(myDefaultAttribute->GetType() == _another.myDefaultAttribute->GetType());

    myDefaultAttribute->CopyValueFromOther(_another.myDefaultAttribute);
}
void TemplateAttribute::DeleteDefaultAttribute()
{
    // Should not be called if not meant to be replace immediately after

    if (myDefaultAttribute == nullptr)
    {
        return;
    }

    UnregisterAttribute(myDefaultAttribute);
    delete myDefaultAttribute;
    myDefaultAttribute = nullptr;
}
void TemplateAttribute::InitDefaultAttribute(AttributeTypeHelper::Type _type)
{
    DeleteDefaultAttribute();

    myDefaultAttribute = AttributeTypeHelper::NewAttributeFromType(_type, *this);
    Q_ASSERT(myDefaultAttribute != nullptr);
}
void TemplateAttribute::ResetUselessParam(AttributeTypeHelper::Type _type)
{
    AttributeTypeHelper::ResetUselessParamsForType(_type, mySharedParam);
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
bool TemplateAttribute::IsActive() const
{
    return myIsActive;
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
    else if (firstType == AttributeTypeHelper::Type::Structure)
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
    SetActive(_templateToCopy.IsActive());

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


    if (!softChange)
    {
        // Deleting the default attribute in advance to avoid nullptr issues on AArray and AStructure
        DeleteDefaultAttribute();
    }
    mySharedParam = newParamToCopy;
    if (!softChange)
    {
        InitDefaultAttribute(newType);
        ResetUselessParam(newType);
    }

    SetDefaultValue(_templateToCopy.myDefaultAttribute->GetValue_JSON());

    return softChange;
}
void TemplateAttribute::SetDefaultValue(const QJsonValue& _value)
{
    myDefaultAttribute->SetValue_JSON(_value);
}
void TemplateAttribute::SetName(const QString& _name)
{
    myAttrName = _name;
}
void TemplateAttribute::SetActive(bool _active)
{
    myIsActive = _active;
}


Attribute* TemplateAttribute::GenerateAttribute() const
{
    Q_ASSERT(myDefaultAttribute != nullptr);
    return myDefaultAttribute->CreateDuplica();
}



QJsonObject TemplateAttribute::GetAsJson() const
{
    QJsonObject obj = QJsonObject();
    obj.insert("name", myAttrName);
    obj.insert("type", (int)GetType());
    obj.insert("param", mySharedParam.GetAsJson());
    obj.insert("active", myIsActive);
    obj.insert("default", myDefaultAttribute->GetValue_JSON());
    return obj;
}
TemplateAttribute* TemplateAttribute::NewAttributeFromJSON(const QJsonObject& _templateAttributeAsJson)
{
    const QString name = _templateAttributeAsJson.value("name").toString();
    const AttributeTypeHelper::Type type = AttributeTypeHelper::Type(_templateAttributeAsJson.value("type").toInt());
    const AttributeParam param = AttributeParam(_templateAttributeAsJson.value("param").toObject());
    const bool active = _templateAttributeAsJson.value("active").toBool();

    TemplateAttribute* templAttr = new TemplateAttribute(name, type, param, active);

    templAttr->GetDefaultAttributeW()->SetValue_JSON(_templateAttributeAsJson.value("default"));

    return templAttr;
}

