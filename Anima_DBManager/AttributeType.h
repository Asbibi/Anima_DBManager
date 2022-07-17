#ifndef ATTRIBUTETYPE_H
#define ATTRIBUTETYPE_H

#include "attributeparam.h"
#include <QString>

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

    Texture,
    Mesh,
    AnimInstance,
    Niagara,
    Sound,
};

QString TypeToString(const Type _type);
Type StringToType(const QString& _typeString);
bool AreParamValid(const Type _type, const AttributeParam& _param);

}

#endif // ATTRIBUTETYPE_H
