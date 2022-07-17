#include "attributetype.h"


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

        CASE(Texture,       "Texture");
        CASE(Mesh,          "Mesh");
        CASE(AnimInstance,  "AnimInstance");
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

    CASE(Texture,       "Texture");
    CASE(Mesh,          "Mesh");
    CASE(AnimInstance,  "AnimInstance");
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
    }
    return true;
}

}
