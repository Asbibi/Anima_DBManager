#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "enumerator.h"
#include "structuredb.h"
#include "templatestructure.h"
#include "sstringtable.h"

#include <QList>
#include <vector>

class DB_Manager
{
private:
    QString myProjectContentFolderPath;
    std::vector<Enumerator> enumerators;
    std::vector<StructureDB*> myStructures;
    QList<SStringTable> myStringTables;

    DB_Manager();
    DB_Manager(const DB_Manager&) = delete;
    ~DB_Manager();

    int GetIndexFromStringTableName(const QString& _tableName) const;

public:
    static DB_Manager& GetDB_Manager();

    const QString& GetProjectContentFolderPath() const { return myProjectContentFolderPath; }

    int GetEnumCount() const;
    const Enumerator* GetEnum(int _index) const;
    void AddEnum(const Enumerator& _enum);
    void RemoveEnum(int _index);

    int GetStructuresCount() const;
    const StructureDB* GetStructureTable(int index) const;
    StructureDB* GetStructureTable(int index);
    void CreateStructureDB(const TemplateStructure& _structureTemplate);

    int GetStringTableCount() const;
    const SStringTable* GetStringTable(int _index) const;
    const SStringTable* GetStringTable(const QString& _tableName) const;
    SStringTable* GetStringTable(int _index);
    SStringTable* GetStringTable(const QString& _tableName);
    void AddStringTable(const QString& _newTableName);
    void RemoveStringTable(int _index);
    void RemoveStringTable(const QString& _tableName);
    bool AreValidIdentifiers(const QString& _tableId, const QString& _stringId) const;
    QString GetStringForDisplay(const QString& _tableId, const QString& _stringId, bool _complete = false) const;
};

#endif // DB_MANAGER_H
