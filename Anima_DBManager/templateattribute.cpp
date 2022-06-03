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


TemplateAttribute::TemplateAttribute() :
    myAttrName(""),
    mySharedParam(nullptr),
    myTemplateAttribute(nullptr)
{}
TemplateAttribute::TemplateAttribute(const QString& _name, const Attribute::Type _type, const AttributeParam& _sharedParam) :
    myAttrName(_name),
    mySharedParam(new AttributeParam(_sharedParam))
{
    switch (_type)
    {
        default:
        case Attribute::Type::Invalid:
        {
            qFatal("Initialized a Template Attribute with INVALID type !");
            return;
        }

        case Attribute::Type::Bool:
        {
            myTemplateAttribute = new ABool();
            return;
        }
        case Attribute::Type::Enum:
        {
            myTemplateAttribute = new AEnumerator(mySharedParam);
            return;
        }
        case Attribute::Type::Float:
        {
            myTemplateAttribute = new AFloat(mySharedParam);
            return;
        }
        case Attribute::Type::Int:
        {
            myTemplateAttribute = new AInt(mySharedParam);
            return;
        }
        case Attribute::Type::ShortString:
        {
            myTemplateAttribute = new AShortString(mySharedParam);
            return;
        }

        case Attribute::Type::Array:
        {
            myTemplateAttribute = new AArray(mySharedParam);
            return;
        }
        case Attribute::Type::Structure:
        {
            //myTemplateAttribute = new AStructure(&mySharedParam);
            return;
        }
        case Attribute::Type::Reference:
        {
            myTemplateAttribute = new AReference(mySharedParam);
            return;
        }
        case Attribute::Type::TableString:
        {
            myTemplateAttribute = new ATableString();
            return;
        }

        case Attribute::Type::AnimInstance:
        {
            myTemplateAttribute = new AAAnimInstance();
            return;
        }
        case Attribute::Type::Mesh:
        {
            myTemplateAttribute = new AAMesh();
            return;
        }
        case Attribute::Type::Niagara:
        {
            myTemplateAttribute = new AANiagara();
            return;
        }
        case Attribute::Type::Sound:
        {
            myTemplateAttribute = new AASound();
            return;
        }
        case Attribute::Type::Texture:
        {
            myTemplateAttribute = new AATexture();
            return;
        }
    }
}
void TemplateAttribute::DeleteData()
{
    if (mySharedParam)
        delete mySharedParam;
    if (myTemplateAttribute)
        delete myTemplateAttribute;
}
TemplateAttribute::~TemplateAttribute()
{}




void TemplateAttribute::SetDefaultValue(const QString& _valueAsText)
{
    myTemplateAttribute->SetValueFromText(_valueAsText);
}

AttributeParam& TemplateAttribute::GetSharedParamW()
{
    return *mySharedParam;
}
Attribute* TemplateAttribute::GetTemplateAttributeW()
{
    return myTemplateAttribute;
}

Attribute* TemplateAttribute::GenerateAttribute() const
{
    if (!myTemplateAttribute)
        qFatal("GENERATING ATTRIBUTE FROM INVALID ATTRIBUTE");

    return myTemplateAttribute ? myTemplateAttribute->CreateDuplica() : nullptr;
}
