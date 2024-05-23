#ifndef ATTRIBUTETYPE_H
#define ATTRIBUTETYPE_H

#include <QString>

struct AttributeParam;
class Attribute;
class TemplateAttribute;

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
    SkeletalMesh,
    StaticMesh,
    Niagara,
    Sound,
    Class
};

QString TypeToString(const Type _type);
Type StringToType(const QString& _typeString);
bool AreParamValid(const Type _type, const AttributeParam& _param);
bool IsAssetType(const Type _type);
Attribute* NewAttributeFromType(const Type _type, TemplateAttribute& _template);
void ResetUselessParamsForType(const Type _type, AttributeParam& _param);
bool ShouldBeWrappedInQuoteInCSV(const Type _type);
const QString csvDoubleQuoteWrapper = "\"\"%1\"\"";

}

#endif // ATTRIBUTETYPE_H
