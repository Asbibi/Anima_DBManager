#ifndef TEMPLATESTRUCTURE_H
#define TEMPLATESTRUCTURE_H

#include "attributetype.h"
#include "templateattribute.h"
#include <QColor>
#include <QList>

class TemplateStructure
{
private:
    QList<TemplateAttribute> myAttributeTemplates;

    QColor myStructColor;
    QString myStructName;   // Global Denomination of the struct (ex: "Anima", "Move, "HealingItem"...)
    QString myStructAbbrev; // Abbreviation of the struct name used for the constant part of row names (ex: "A", "M", "IH"...)

public:
    TemplateStructure(const QString& _structName, const QColor& _structColor);
    TemplateStructure(const QString& _structName, const QString& _structAbbrev, const QColor& _structColor);
    TemplateStructure(const TemplateStructure& _other);
    void operator=(const TemplateStructure& _another);
    // operator= !!!!!

    const QColor& GetStructColor() const { return myStructColor; }

    const QString& GetStructName() const                            { return myStructName; }
    void RenameStructureTemplate(const QString& _newStructName)     { myStructName = _newStructName; }
    const QString& GetStructAbbrev() const                          { return myStructAbbrev; }
    void ReabbrevStructureTemplate(const QString& _newStructAbbrev) { myStructAbbrev = _newStructAbbrev; }


    void AddAttributeTemplate(const AttributeTypeHelper::Type _type, const QString& _attName, const AttributeParam& _attParam, int _index = -1);
    void AddAttributeTemplate(const TemplateAttribute& _attTemplate, const QString* _newName = nullptr, int _index = -1);
    void RemoveAttribute(const int& _index);
    void RemoveAttribute(const QString& _attName);
    void MoveAttribute(int _indexFrom, int _indexTo);

    void RenameAttributeTemplate(int _index, QString& _newName);
    void SetAttributeDefaultValue(int _index, const QString& _value);
    void SetAttributeDefaultValue(const QString& _attName, const QString& _value);
    int GetAttributeIndex(const QString& _attName) const;

    AttributeTypeHelper::Type GetAttributeType(int _index) const;
    const QString& GetAttributeName(int _index) const;
    TemplateAttribute* GetAttributeTemplate(int _index);
    const TemplateAttribute* GetAttributeTemplate(int _index) const;
    const TemplateAttribute* GetAttributeTemplate(const QString& _attName) const;
    int GetAttributesCount() const { return (int)(myAttributeTemplates.count()); }

    const QList<TemplateAttribute>& GetAttributes() const { return myAttributeTemplates; }
};

#endif // TEMPLATESTRUCTURE_H
