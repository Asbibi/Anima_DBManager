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

public:
    Structure(const StructureTemplate& structureTemplate);
    ~Structure();

    const Attribute* GetAttribute(const int& att_Index) const;
    void ChangeAttribute(const int& att_Index, Attribute* newAttribute);
};

#endif // STRUCTURE_H
