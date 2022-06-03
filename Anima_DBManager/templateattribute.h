#ifndef TEMPLATEATTRIBUTE_H
#define TEMPLATEATTRIBUTE_H

#include "attribute.h"
#include "attributeparam.h"
#include <QString>

class TemplateStructure;

class TemplateAttribute
{
    friend TemplateStructure;

private:
    QString myAttrName = "";
    AttributeParam* mySharedParam = nullptr;
    Attribute* myTemplateAttribute = nullptr;

    void DeleteData();  // ~TemplateAttribute is called by std::vector but we want to keep the same mySharedParam and myTemplateAttribute
                        // as mySharedParam is referenced elsewhere so need a method to delete those outside of desctructor
                        // will be called by friend TemplateStructure when it's time to die.

public:
    TemplateAttribute();
    TemplateAttribute(const QString& _name, const Attribute::Type _type, const AttributeParam& _sharedParam);
    ~TemplateAttribute();


    const QString& GetName() const {return myAttrName;}
    Attribute::Type GetType() const {return myTemplateAttribute->GetType();}

    void SetDefaultValue(const QString& _valueAsText);

    AttributeParam& GetSharedParamW();
    Attribute* GetTemplateAttributeW();

    Attribute* GenerateAttribute() const;
};

#endif // TEMPLATEATTRIBUTE_H
