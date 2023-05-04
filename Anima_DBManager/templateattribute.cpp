#include "templateattribute.h"

#include "abool.h"
#include "aenumerator.h"
#include "afloat.h"
#include "aint.h"
#include "ashortstring.h"

#include "aarray.h"
#include "astructure.h"
#include "areference.h"
#include "atablestring.h"

#include "aasset.h"
#include "aamesh.h"
#include "aaniagara.h"
#include "aasound.h"
#include "aatexture.h"

#include <QDebug>


TemplateAttribute::TemplateAttribute() :
    myAttrName(""),
    mySharedParam()
{
    myDefaultAttribute = new ABool(mySharedParam);
}
TemplateAttribute::TemplateAttribute(const AttributeParam& _sharedParamToCopy) :
    myAttrName(""),
    mySharedParam(_sharedParamToCopy)
{
    myDefaultAttribute = new ABool(mySharedParam);
}
TemplateAttribute::TemplateAttribute(const QString& _name, const AttributeTypeHelper::Type _type, const AttributeParam& _sharedParamToCopy) :
    myAttrName(_name),
    mySharedParam(_sharedParamToCopy)
{
    InitDefaultAttribute(_type);
}
TemplateAttribute::TemplateAttribute(const TemplateAttribute& _another) :
    TemplateAttribute(_another.myAttrName, _another.GetType(), _another.mySharedParam)
{
    Q_ASSERT(myDefaultAttribute != nullptr && _another.myDefaultAttribute != nullptr);
    Q_ASSERT(myDefaultAttribute->GetType() == _another.myDefaultAttribute->GetType());

    myDefaultAttribute->CopyValueFromOther(_another.myDefaultAttribute);
}
void TemplateAttribute::InitDefaultAttribute(AttributeTypeHelper::Type _type)
{
    if (myDefaultAttribute != nullptr)
    {
        delete myDefaultAttribute;
        myDefaultAttribute = nullptr;
    }

#define CASE_INIT_TEMPLATE_WITH_CLASS(TYPE, CLASS) case AttributeTypeHelper::Type::TYPE: { myDefaultAttribute = new CLASS(mySharedParam); return; }
#define CASE_INIT_TEMPLATE(TYPE) CASE_INIT_TEMPLATE_WITH_CLASS(TYPE, A##TYPE)

    switch (_type)
    {
        default:
        case AttributeTypeHelper::Type::Invalid:
        {
            qFatal("Initialized a Template Attribute with INVALID type !");
            return;
        }

        CASE_INIT_TEMPLATE(Bool);
        CASE_INIT_TEMPLATE_WITH_CLASS(Enum, AEnumerator);
        CASE_INIT_TEMPLATE(Float);
        CASE_INIT_TEMPLATE(Int);
        CASE_INIT_TEMPLATE(ShortString);
        CASE_INIT_TEMPLATE(Array);
        //CASE_INIT_TEMPLATE(Structure);
        CASE_INIT_TEMPLATE(Reference);
        CASE_INIT_TEMPLATE(TableString);
        CASE_INIT_TEMPLATE_WITH_CLASS(UAsset, AAsset);
        CASE_INIT_TEMPLATE_WITH_CLASS(Mesh, AAMesh);
        CASE_INIT_TEMPLATE_WITH_CLASS(Niagara, AANiagara);
        CASE_INIT_TEMPLATE_WITH_CLASS(Sound, AASound);
        CASE_INIT_TEMPLATE_WITH_CLASS(Texture, AATexture);
    }

#undef CASE_INIT_TEMPLATE
#undef CASE_INIT_TEMPLATE_WITH_CLASS
}
void TemplateAttribute::ResetUselessParam(AttributeTypeHelper::Type _type)
{
    if (_type != AttributeTypeHelper::Type::Enum)
    {
        mySharedParam.enumeratorIndex = -1;
    }
    if (_type != AttributeTypeHelper::Type::Reference)
    {
        mySharedParam.structTable = nullptr;
    }
    if (_type != AttributeTypeHelper::Type::Array)
    {
        qDebug() << "TODO - reset attribute ptr for array attribute";
    }
    if (_type != AttributeTypeHelper::Type::Structure)
    {
        qDebug() << "TODO - reset structure template for structure attribute";
    }
}
void TemplateAttribute::operator=(const TemplateAttribute& _another)
{
    myAttrName = _another.myAttrName;
    mySharedParam = _another.mySharedParam;

    InitDefaultAttribute(_another.myDefaultAttribute->GetType());
    myDefaultAttribute->CopyValueFromOther(_another.myDefaultAttribute);
}

TemplateAttribute::~TemplateAttribute()
{
    Q_ASSERT(myDefaultAttribute);
    delete myDefaultAttribute;
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



void TemplateAttribute::SetNewValues(AttributeTypeHelper::Type _type, const AttributeParam& _param)
{
    mySharedParam = _param;
    if (GetType() == _type)
        return;

    InitDefaultAttribute(_type);
    ResetUselessParam(_type);
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
   file << myAttrName.toStdString();
   file << '|' << AttributeTypeHelper::TypeToString(GetType()).toStdString();
   file << '|'; mySharedParam.SaveParams_CSV(file);
   file << '|'; myDefaultAttribute->WriteValue_CSV(file);
}
