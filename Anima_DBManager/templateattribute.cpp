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

#include "aaaniminstance.h"
#include "aamesh.h"
#include "aaniagara.h"
#include "aasound.h"
#include "aatexture.h"


TemplateAttribute::TemplateAttribute(const AttributeParam& _sharedParamToCopy) :
    myAttrName(""),
    mySharedParam(*(new AttributeParam(_sharedParamToCopy))),
    myTemplateAttribute(nullptr)
{}
TemplateAttribute::TemplateAttribute(const QString& _name, const AttributeTypeHelper::Type _type, const AttributeParam& _sharedParamToCopy) :
    myAttrName(_name),
    mySharedParam(*(new AttributeParam(_sharedParamToCopy)))
{
    InitDefaultAttribute(_type);
}
TemplateAttribute::TemplateAttribute(const TemplateAttribute& _another) :
    TemplateAttribute(_another.myAttrName, _another.GetType(), _another.mySharedParam)
{
    Q_ASSERT(myTemplateAttribute && _another.myTemplateAttribute);
    myTemplateAttribute->CopyValueFromOther(_another.myTemplateAttribute);
}
void TemplateAttribute::InitDefaultAttribute(AttributeTypeHelper::Type _type)
{
    if (myTemplateAttribute != nullptr)
    {
        delete myTemplateAttribute;
        myTemplateAttribute = nullptr;
    }

#define CASE_INIT_TEMPLATE_WITH_CLASS(TYPE, CLASS) case AttributeTypeHelper::Type::TYPE: { myTemplateAttribute = new CLASS(mySharedParam); return; }
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
        CASE_INIT_TEMPLATE_WITH_CLASS(AnimInstance, AAAnimInstance);
        CASE_INIT_TEMPLATE_WITH_CLASS(Mesh, AAMesh);
        CASE_INIT_TEMPLATE_WITH_CLASS(Niagara, AANiagara);
        CASE_INIT_TEMPLATE_WITH_CLASS(Sound, AASound);
        CASE_INIT_TEMPLATE_WITH_CLASS(Texture, AATexture);
    }

#undef CASE_INIT_TEMPLATE
#undef CASE_INIT_TEMPLATE_WITH_CLASS
}
void TemplateAttribute::DeleteData()
{
    delete &mySharedParam;
    if (myTemplateAttribute)
        delete myTemplateAttribute;
}
void TemplateAttribute::operator=(const TemplateAttribute& _another)
{
    myAttrName = _another.myAttrName;
    mySharedParam = _another.mySharedParam;
    myTemplateAttribute = _another.myTemplateAttribute;
}

TemplateAttribute::~TemplateAttribute()
{}




void TemplateAttribute::SetNewValues(AttributeTypeHelper::Type _type, const AttributeParam& _param)
{
    mySharedParam = _param;
    if (GetType() == _type)
        return;

    InitDefaultAttribute(_type);
}
void TemplateAttribute::SetDefaultValue(const QString& _valueAsText)
{
    myTemplateAttribute->SetValueFromText(_valueAsText);
}



const AttributeParam& TemplateAttribute::GetSharedParam() const
{
    return mySharedParam;
}
AttributeParam& TemplateAttribute::GetSharedParam()
{
    return mySharedParam;
}
Attribute* TemplateAttribute::GetTemplateAttributeW()
{
    return myTemplateAttribute;
}


bool TemplateAttribute::HasValidSharedParam() const
{
    return AttributeTypeHelper::AreParamValid(GetType(), mySharedParam);
}


Attribute* TemplateAttribute::GenerateAttribute() const
{
    if (!myTemplateAttribute)
        qFatal("GENERATING ATTRIBUTE FROM INVALID ATTRIBUTE");

    return myTemplateAttribute ? myTemplateAttribute->CreateDuplica() : nullptr;
}
