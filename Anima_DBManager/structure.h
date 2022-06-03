#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "templatestructure.h"
#include <vector>
#include <QObject>

/**
 * Data Object for a Structure Object.
 * It is composed of several Attributes,
 * each one being assigned to a specific and unique name,
 * and their type can be various.
 */
class Structure : public QObject
{
private:
    std::vector<Attribute*> attributes;
    const TemplateStructure* templ;

public:
    Structure(const Structure& _other);
    Structure(const TemplateStructure& _structureTemplate);
    ~Structure();

    int GetAttributeCount() const;
    const Attribute* GetAttribute(int _attIndex) const;
    const Attribute* GetAttribute(const QString& _attName) const;
    void SetAttributeValueFromText(int _attIndex, QString _valueText);
    void SetAttributeValueFromText(const QString& _attName, QString _valueText);
    void ChangeAttribute(int _attIndex, Attribute* _newAttribute);

    void GetAttributesDisplayedText(QString& _text) const;
    void WriteValue_CSV(std::ofstream& file) const;         // Used for structure as attribute of other structures
    void WriteValue_CSV_AsRow(std::ofstream& file) const;   // Used to directly save a row structure of a DataTable
};

#endif // STRUCTURE_H
