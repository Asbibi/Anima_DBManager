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

    const StructureTemplate& GetTemplate() const;
    const Structure& GetStructureAt(int index) const;
    int GetStructureCount() const;
    const QString& GetTemplateName() const;
};

#endif // STRUCTUREDB_H
