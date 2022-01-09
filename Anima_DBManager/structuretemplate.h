#ifndef STRUCTURETEMPLATE_H
#define STRUCTURETEMPLATE_H


#include "attribute.h"
#include <vector>

class StructureTemplate
{
private:
    std::vector<QString> attributesNames;
    std::vector<const Attribute*> attributeTemplates;

    QString structName; // Global Denomination of the struct, for row struct it's an abreviation like "A" for Anima, "IH" for ItemHeal ; for other it's just a regular name to designate the struct... Do not confound with the attribtue name that is specific ("Squikignite", "Scratch", "Apple"...)
    QColor structColor;


public:
    StructureTemplate(QString _structName, QColor _structColor);
    ~StructureTemplate();

    void AddAttribute(QString att_Name, Attribute* att);
    void RemoveAttribute(const int& att_Index);
    void RemoveAttribute(const QString& att_Name);

    int GetAttributeIndex(const QString& att_Name) const;
    const QString& GetAttributeName(int _attIndex) const;
    const Attribute* GetAttribute(int att_Index) const;
    const Attribute* GetAttribute(const QString& att_Name) const;
    const std::vector<const Attribute*>& GetAttributes() const;
    const QString& GetStructName() const;
};

#endif // STRUCTURETEMPLATE_H
