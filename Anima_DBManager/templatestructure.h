#ifndef TEMPLATESTRUCTURE_H
#define TEMPLATESTRUCTURE_H

#include "attributetype.h"
#include "iconmanager.h"
#include "templateattribute.h"
#include "areference.h"
#include <QColor>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QHash>

class TemplateStructure
{
private:
    QList<TemplateAttribute*> myAttributeTemplates;

    QColor myStructColor;
    IconManager::IconType myIconType;
    QString myStructName;   // Global Denomination of the struct (ex: "Anima", "Move, "HealingItem"...)
    QString myStructAbbrev; // Abbreviation of the struct name used for the constant part of row names (ex: "A", "M", "IH"...)

    void AddAttributeTemplateInternal(TemplateAttribute* _attTemplateToCopy, const QString* _newName, int _index);

public:
    TemplateStructure();
    TemplateStructure(const QString& _structName);
    TemplateStructure(const QString& _structName, const QColor& _structColor, IconManager::IconType _iconType = IconManager::IconType::None);
    TemplateStructure(const QString& _structName, const QString& _structAbbrev, const QColor& _structColor, IconManager::IconType _iconType = IconManager::IconType::None);
    TemplateStructure(const TemplateStructure& _other);
    ~TemplateStructure();

    void SetNewValues(const TemplateStructure& _templateToCopy);

    const QColor& GetStructColor() const { return myStructColor; }
    void SetStructureColor(const QColor& _color) { myStructColor = _color; }
    IconManager::IconType GetStructIcon() const { return myIconType; }
    void SetStructureIcon(IconManager::IconType _iconType) { myIconType = _iconType; }

    const QString& GetStructName() const                            { return myStructName; }
    void RenameStructureTemplate(const QString& _newStructName)     { myStructName = _newStructName; }
    const QString& GetStructAbbrev() const                          { return myStructAbbrev; }
    void ReabbrevStructureTemplate(QString& _newStructAbbrev);


    void AddAttributeTemplate(int _index = -1);
    void AddAttributeTemplate(const AttributeTypeHelper::Type _type, const QString& _attName, int _index = -1);
    void AddAttributeTemplate(const AttributeTypeHelper::Type _type, const QString& _attName, const AttributeParam& _attParam, int _index = -1);
    void AddAttributeTemplate(const TemplateAttribute& _attTemplateToCopy, const QString* _newName = nullptr, int _index = -1);
    void AddAttributeTemplateDirectly_UNSAFE(TemplateAttribute* _attTemplateToCopy, int _index = -1);   // NOT SAFE - Only use in importation from cpp
    void RemoveAttribute(int _index);
    void RemoveAttribute(const QString& _attName);
    void MoveAttribute(int _indexFrom, int _indexTo);
    bool ChangeAttribute(int _attrIndex, const TemplateAttribute& _templateToCopy);

    void RenameAttributeTemplate(int _index, QString& _newName, bool _skipSameNameCheck = false);
    void SetAttributeDefaultValue(int _index, const QString& _value);
    void SetAttributeDefaultValue(const QString& _attName, const QString& _value);
    int GetAttributeIndex(const QString& _attName) const;

    AttributeTypeHelper::Type GetAttributeType(int _index) const;
    const QString& GetAttributeName(int _index) const;
    TemplateAttribute* GetAttributeTemplate(int _index);
    const TemplateAttribute* GetAttributeTemplate(int _index) const;
    const TemplateAttribute* GetAttributeTemplate(const QString& _attName) const;
    int GetAttributesCount() const { return (int)(myAttributeTemplates.count()); }

    const QList<TemplateAttribute*>& GetAttributes() const { return myAttributeTemplates; }
    QList<TemplateAttribute*>& GetAttributesW() { return myAttributeTemplates; }

    void SaveTemplate(QJsonArray& _templateJson) const;
    static TemplateStructure LoadTemplateNoAttribute(const QJsonObject& _templateJson);
    void LoadTemplateOnlyAttribute(const QJsonArray& _templateJson);
};

#endif // TEMPLATESTRUCTURE_H
