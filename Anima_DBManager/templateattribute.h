#ifndef TEMPLATEATTRIBUTE_H
#define TEMPLATEATTRIBUTE_H

#include "attributetype.h"
#include "attribute.h"
#include "attributeparam.h"
#include "constants.h"
#include <QJsonObject>
#include <QString>
#include <QSet>

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
    QSet<Attribute*> myAttributes;

    void DeleteDefaultAttribute();
    void InitDefaultAttribute(AttributeTypeHelper::Type _type);
    void ResetUselessParam(AttributeTypeHelper::Type _type);

    static bool IsSameArrayType(const AttributeParam& _firstParam, const AttributeParam& _secondParam);
    static bool IsSameStructType(const AttributeParam& _firstParam, const AttributeParam& _secondParam);

public:
    TemplateAttribute();
#ifdef TEST_VALUES
    TemplateAttribute(const QString& _name, const AttributeTypeHelper::Type _type, const AttributeParam& _sharedParamToCopy);
#endif
    TemplateAttribute(const TemplateAttribute& _another);
    ~TemplateAttribute();

    void RegisterAttribute(Attribute* _attr);
    void UnregisterAttribute(Attribute* _attr);

    const QString& GetName() const;
    AttributeTypeHelper::Type GetType() const;
    const Attribute* GetDefaultAttribute() const;
    const AttributeParam& GetSharedParam() const;
    AttributeParam& GetSharedParam();
    Attribute* GetDefaultAttributeW();
    bool HasValidSharedParam() const;

    bool SetNewValues(const TemplateAttribute& _templateToCopy);
    void SetDefaultValue(const QString& _valueAsText);
    void SetName(const QString& _name) {myAttrName = _name;}


    Attribute* GenerateAttribute() const;

    QJsonObject GetAsJson() const;
    static TemplateAttribute* NewAttributeFromJSON(const QJsonObject& _templateAttributeAsJson);
};

#endif // TEMPLATEATTRIBUTE_H
