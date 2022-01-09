#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "enumerator.h"
#include "structuretemplate.h"
#include "structure.h"
#include <vector>

class DB_Manager
{
private:
    std::vector<Enumerator> enumerators;

    DB_Manager();
    DB_Manager(const DB_Manager&) = delete;

public:
    static DB_Manager& GetDB_Manager();

    int GetEnumCount() const;
    const Enumerator* GetEnum(int _index) const;
    void AddEnum(const Enumerator& _enum);
    void RemoveEnum(int _index);
};

#endif // DB_MANAGER_H
