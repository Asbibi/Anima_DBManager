#ifndef STRUCTURETEMPLATE_H
#define STRUCTURETEMPLATE_H


#include "attribute.h"
#include <vector>

class StructureTemplate
{
private:
    std::vector<QString> attributesNames;
    std::vector<const Attribute*> attributeTemplates;

    QString structName;
    QColor structColor;

    int GetAttributeIndex(const QString& att_Name) const;

public:
    StructureTemplate(QString _structName, QColor _structColor);
    ~StructureTemplate();

    void AddAttribute(QString att_Name, Attribute* att);
    void RemoveAttribute(const int& att_Index);
    void RemoveAttribute(const QString& att_Name);
    const Attribute* GetAttribute(const int& att_Index) const;
    const Attribute* GetAttribute(const QString& att_Name) const;
    const std::vector<const Attribute*>& GetAttributes() const;
};

#endif // STRUCTURETEMPLATE_H
