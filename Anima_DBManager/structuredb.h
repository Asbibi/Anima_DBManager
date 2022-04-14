#ifndef STRUCTUREDB_H
#define STRUCTUREDB_H

#include "structure.h"
#include "structuretemplate.h"
#include <vector>

class StructureDB
{
private:
    const StructureTemplate templ;
    std::vector<Structure> myStructures;

    void BoundIndex(int& index) const;

public:
    StructureDB(const StructureTemplate& _structureTemplate);

    void AddStructureAt(int index);
    void RemoveStructureAt(int index);
    void ClearStructures();

    const Structure& GetStructureAt(int index) const;
    int GetStructureCount() const;
};

#endif // STRUCTUREDB_H
