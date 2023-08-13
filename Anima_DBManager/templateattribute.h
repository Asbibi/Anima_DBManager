#ifndef TEMPLATEATTRIBUTE_H
#define TEMPLATEATTRIBUTE_H

#include "attributetype.h"
#include "attribute.h"
#include "attributeparam.h"
#include <QString>

class AReference;
class TemplateStructure;
class QTemplateAttributeCore;

class TemplateAttribute
{
    //friend TemplateStructure;
    friend QTemplateAttributeCore;

private:
    QString myAttrName = "";
    AttributeParam mySharedParam;
    Attribute* myDefaultAttribute = nullptr;

    void InitDefaultAttribute(AttributeTypeHelper::Type _type);
    void ResetUselessParam(AttributeTypeHelper::Type _type);

public:
    TemplateAttribute();
    TemplateAttribute(const AttributeParam& _sharedParamToCopy);
    TemplateAttribute(const QString& _name, const AttributeTypeHelper::Type _type, const AttributeParam& _sharedParamToCopy);
    TemplateAttribute(const TemplateAttribute& _another);
    void operator=(const TemplateAttribute& _another);      // !!! Pass the ownership of myDefaultAttribute from _another to this
    ~TemplateAttribute();


    const QString& GetName() const;
    AttributeTypeHelper::Type GetType() const;
    const Attribute* GetDefaultAttribute() const;
    const AttributeParam& GetSharedParam() const;
    AttributeParam& GetSharedParam();
    Attribute* GetDefaultAttributeW();
    bool HasValidSharedParam() const;

    void SetNewValues(AttributeTypeHelper::Type _type, const AttributeParam& _param);
    void SetDefaultValue(const QString& _valueAsText);
    void SetName(const QString& _name) {myAttrName = _name;}


    Attribute* GenerateAttribute() const;

    void SaveTemplate_CSV(std::ofstream& file) const;
    QString GetTemplateAsCSV() const;
    static TemplateAttribute* NewAttribute_CSV(const QString& _csvLine, QHash<AReference*, QString>& _outRefMap);
};

#endif // TEMPLATEATTRIBUTE_H
