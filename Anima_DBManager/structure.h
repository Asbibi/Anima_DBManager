#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "templatestructure.h"
#include <QList>
#include <QObject>

/**
 * Data Object for a Structure Object.
 * It is composed of several Attributes,
 * each one being related to a TemplateAttribute
 * stored in the referenced TemplateStructure.
 */
class Structure : public QObject
{
private:
    TemplateStructure& myTemplate;
    QList<Attribute*> myAttributes;

public:
    Structure(TemplateStructure& _structureTemplate);
    Structure(const Structure& _other, bool _attrDeepCopy = false);
    Structure(const Structure& _other, TemplateStructure& _newTemplate);
    void operator=(const Structure& _other);
    ~Structure();


    int GetAttributeCount() const;
    const Attribute* GetAttribute(int _attIndex) const;
    const Attribute* GetAttribute(const QString& _attName) const;
    void SetAttributeValueFromText(int _attIndex, QString _valueText);
    void SetAttributeValueFromText(const QString& _attName, QString _valueText);
    void MoveAttribute(int _indexFrom, int _indexTo);
    void ResetAttributeToDefault(int _attindex);
    void FixAttributeTypeToDefault(int _attIndex);
    void AddAttribute(int _position, bool _copyFromPrevious);
    void RemoveAttribute(int _position);

    void GetAttributesDisplayedText(QString& _text) const;
    void WriteValue_CSV(std::ofstream& file) const;         // Used for structure as attribute of other structures
    void WriteValue_CSV_AsRow(std::ofstream& file) const;   // Used to directly save a row structure of a DataTable
    QString GetDisplayText() const;
    bool IsOneOfMyAttributes(const Attribute* _attributePtr) const;
};

#endif // STRUCTURE_H
