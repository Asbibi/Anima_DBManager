#ifndef ATTRIBUTETYPE_H
#define ATTRIBUTETYPE_H

#include "attributeparam.h"
#include <QString>

class Attribute;
class AttributeParam;

namespace AttributeTypeHelper {

enum class Type
{
    Invalid,

    Bool,
    Enum,
    Float,
    Int,
    ShortString,

    TableString,
    Reference,

    Array,
    Structure,

    UAsset,
    Texture,
    Mesh,
    Niagara,
    Sound,
};

QString TypeToString(const Type _type);
Type StringToType(const QString& _typeString);
bool AreParamValid(const Type _type, const AttributeParam& _param);
bool IsAssetType(const Type _type);
Attribute* NewAttributeFromType(const Type _type, const AttributeParam& _sharedParam);

}

#endif // ATTRIBUTETYPE_H
