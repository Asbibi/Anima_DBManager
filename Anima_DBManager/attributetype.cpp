#include "attributetype.h"

#include "attributeparam.h"
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


namespace AttributeTypeHelper {

QString TypeToString(const Type _type)
{
#define CASE(type, str) case Type::type: return str;
    switch(_type)
    {
        CASE(Bool,          "Bool");
        CASE(Enum,          "Enum");
        CASE(Float,         "Float");
        CASE(Int,           "Int");
        CASE(ShortString,   "ShortString");

        CASE(TableString,   "TableString");
        CASE(Reference,     "Reference");

        CASE(Array,         "Array");
        CASE(Structure,     "Structure");

        CASE(UAsset,        "UAsset");
        CASE(Texture,       "Texture");
        CASE(Mesh,          "Mesh");
        CASE(Niagara,       "Niagara");
        CASE(Sound,         "Sound");

        default:
        CASE(Invalid,       "Unset");
    }
#undef CASE
}


Type StringToType(const QString& _typeString)
{
#define CASE(type, str) if (_typeString == str) {return Type::type;}

    CASE(Bool,          "Bool");
    CASE(Enum,          "Enum");
    CASE(Float,         "Float");
    CASE(Int,           "Int");
    CASE(ShortString,   "ShortString");

    CASE(TableString,   "TableString");
    CASE(Reference,     "Reference");

    CASE(Array,         "Array");
    CASE(Structure,     "Structure");

    CASE(UAsset,        "UAsset");
    CASE(Texture,       "Texture");
    CASE(Mesh,          "Mesh");
    CASE(Niagara,       "Niagara");
    CASE(Sound,         "Sound");

#undef CASE

    return Type::Invalid;
}


bool AreParamValid(const Type _type, const AttributeParam& _param)
{
    switch(_type)
    {
        case AttributeTypeHelper::Type::Enum:
            return _param.GetEnum();
        case AttributeTypeHelper::Type::Array:
            return _param.templateAtt;
        case AttributeTypeHelper::Type::Reference:
            return _param.structTable;
        case AttributeTypeHelper::Type::Int:
        {
            return !(!_param.ignoreMin && !_param.ignoreMax && _param.min_i > _param.max_i);
        }
        case AttributeTypeHelper::Type::Float:
        {
            return !(!_param.ignoreMin && !_param.ignoreMax && _param.min_f > _param.max_f);
        }
        default:
            break;
    }
    return true;
}


bool IsAssetType(const Type _type)
{
    return (int)_type >= (int)Type::UAsset;
}

Attribute* NewAttributeFromType(const Type _type, TemplateAttribute& _template)
{
#define CASE_INIT_TEMPLATE_WITH_CLASS(TYPE, CLASS) case AttributeTypeHelper::Type::TYPE: { return new CLASS(_template); }
#define CASE_INIT_TEMPLATE(TYPE) CASE_INIT_TEMPLATE_WITH_CLASS(TYPE, A##TYPE)

    switch (_type)
    {
        default:
        case AttributeTypeHelper::Type::Invalid:
        {
            qWarning("Ask Generation of an Attribute from Invalid Type");
            return nullptr;
        }

        CASE_INIT_TEMPLATE(Bool);
        CASE_INIT_TEMPLATE_WITH_CLASS(Enum, AEnumerator);
        CASE_INIT_TEMPLATE(Float);
        CASE_INIT_TEMPLATE(Int);
        CASE_INIT_TEMPLATE(ShortString);
        CASE_INIT_TEMPLATE(Array);
        CASE_INIT_TEMPLATE(Structure);
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
void ResetUselessAttributesForType(const Type _type, AttributeParam& _param)
{
    if (_type != AttributeTypeHelper::Type::Enum)
    {
        _param.enumeratorIndex = -1;
    }
    if (_type != AttributeTypeHelper::Type::Reference)
    {
        _param.structTable = nullptr;
    }
    if (_type != AttributeTypeHelper::Type::Array)
    {
        if (_param.templateAtt != nullptr)
        {
            delete _param.templateAtt;
            _param.templateAtt = nullptr;
        }
    }
    if (_type != AttributeTypeHelper::Type::Structure)
    {
        if (_param.templateStruct != nullptr)
        {
            delete _param.templateStruct;
            _param.templateStruct = nullptr;
        }
    }
}

}
