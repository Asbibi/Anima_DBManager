#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "structuretemplate.h"
#include <vector>

/**
 * Data Object for a Structure Object. It is composed of several Attributes, each one being assigned to a specific and unique name, and their type can be varying.
 */
class Structure
{
private:
    std::vector<Attribute*> attributes;
    const StructureTemplate& templ;

public:
    Structure(const Structure& _structure);
    Structure(const StructureTemplate& _structureTemplate);
    ~Structure();

    int GetAttributeCount() const;
    const Attribute* GetAttribute(int _attIndex) const;
    const Attribute* GetAttribute(const QString& _attName) const;
    void SetAttributeValueFromText(int _attIndex, QString _valueText);
    void SetAttributeValueFromText(const QString& _attName, QString _valueText);
    void ChangeAttribute(int _attIndex, Attribute* _newAttribute);

    void GetAttributesDisplayedText(QString& _text) const;
    void WriteValue_CSV(std::ofstream& file) const;         // Used for structure as attribute of other structures
    void WriteValue_CSV_AsRow(std::ofstream& file, int index) const;   // Used to directly save a row structure of a DataTable
};

#endif // STRUCTURE_H
