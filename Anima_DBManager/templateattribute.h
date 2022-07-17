#ifndef TEMPLATEATTRIBUTE_H
#define TEMPLATEATTRIBUTE_H

#include "attributetype.h"
#include "attribute.h"
#include "attributeparam.h"
#include <QString>

class TemplateStructure;

class TemplateAttribute
{
    friend TemplateStructure;

private:
    QString myAttrName = "";
    AttributeParam& mySharedParam;               // Shared pointer due to copy/paste of this in a list induces a delete call from old owner while should still be alive
    Attribute* myTemplateAttribute = nullptr;    // Same

    void InitDefaultAttribute(AttributeTypeHelper::Type _type);
    void DeleteData();  // ~TemplateAttribute is called by std::vector but we want to keep the same mySharedParam and myTemplateAttribute
                        // as mySharedParam is referenced elsewhere so need a method to delete those outside of desctructor
                        // will be called by friend TemplateStructure when it's time to die.

public:
    TemplateAttribute(const AttributeParam& _sharedParamToCopy);
    TemplateAttribute(const QString& _name, const AttributeTypeHelper::Type _type, const AttributeParam& _sharedParamToCopy);
    TemplateAttribute(const TemplateAttribute& _another);
    void operator=(const TemplateAttribute& _another);
    ~TemplateAttribute();


    const QString& GetName() const {return myAttrName;}
    AttributeTypeHelper::Type GetType() const {return myTemplateAttribute->GetType();}
    const Attribute* GetDefaultAttr() const {return myTemplateAttribute;}

    void SetNewValues(AttributeTypeHelper::Type _type, const AttributeParam& _param);
    void SetDefaultValue(const QString& _valueAsText);

    const AttributeParam& GetSharedParam() const;
    AttributeParam& GetSharedParam();
    Attribute* GetTemplateAttributeW();
    bool HasValidSharedParam() const;

    Attribute* GenerateAttribute() const;
};

#endif // TEMPLATEATTRIBUTE_H
