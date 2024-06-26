#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <QObject>

#include "templatestructure.h"
#include <QJsonObject>
#include <QList>

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
    ~Structure();


    const TemplateStructure& GetTemplate() const;
    int GetAttributeCount() const;
    const Attribute* GetAttribute(int _attIndex) const;
    const Attribute* GetAttribute(const QString& _attName) const;
    Attribute* GetAttribute(int _attIndex);
    const QList<Attribute*>& GetAttributes() const;
    void SetAttributeValueFromText(int _attIndex, const QJsonValue& _value);
    void SetAttributeValueFromText(const QString& _attName, const QJsonValue& _value);
    void ReadValue_JSON(const QJsonObject& _structAsJson);
    void ReadAttributeValue_CSV(int _attIndex, const QString& _csvValue);
    void MoveAttribute(int _indexFrom, int _indexTo);
    void ResetAttributeToDefault(int _attindex);
    void FixAttributeTypeToDefault(int _attIndex);
    void AddAttribute(int _position, bool _copyFromPrevious);
    void RemoveAttribute(int _position);

    QJsonObject WriteValue_JSON_AsRow() const;
    void WriteValue_CSV_AsRow(std::ofstream& file) const;   // Used to directly save a row structure of a DataTable
    QString GetStructureAsCSV() const;                      // Used for structure as attribute of other structures
    QString GetDisplayText() const;
    bool IsOneOfMyAttributes(const Attribute* _attributePtr) const;
};

#endif // STRUCTURE_H
