#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "enumerator.h"
#include "structuredb.h"
#include "structuretemplate.h"
#include <vector>

class DB_Manager
{
private:
    std::vector<Enumerator> enumerators;
    std::vector<StructureDB*> myStructures;

    DB_Manager();
    DB_Manager(const DB_Manager&) = delete;
    ~DB_Manager();

public:
    static DB_Manager& GetDB_Manager();

    int GetEnumCount() const;
    const Enumerator* GetEnum(int _index) const;
    void AddEnum(const Enumerator& _enum);
    void RemoveEnum(int _index);

    int GetStructuresCount() const;
    const StructureDB* GetStructures(int index) const;
    StructureDB* GetStructures(int index);
    void AddStructures(const StructureTemplate& _structureTemplate);
};

#endif // DB_MANAGER_H
