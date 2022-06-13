#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "attributeparam.h"
#include "enumerator.h"
#include "structuredb.h"
#include "templatestructure.h"
#include "sstringtable.h"

#include <QString>
#include <QList>
#include <vector>

class DB_Manager : public QObject
{
    Q_OBJECT

private:
    QString myProjectContentFolderPath;
    QList<AttributeParam*> myAttributeParamPtrs;
    QList<Enumerator> enumerators;
    std::vector<StructureDB*> myStructures;
    QList<SStringTable> myStringTables;

    DB_Manager();
    DB_Manager(const DB_Manager&) = delete;
    ~DB_Manager();

    int GetIndexFromStringTableName(const QString& _tableName) const;
    void AddStringTablePrivate(const QString& _newTableName, int _index = -1);


public:
    static DB_Manager& GetDB_Manager();

    void Init();

    const QString& GetProjectContentFolderPath() const { return myProjectContentFolderPath; }

    int GetEnumCount() const;
    const Enumerator* GetEnum(int _index) const;
    void AddEnum(const Enumerator& _enum, int _index = -1);
    void MoveEnum(const int _indexFrom, const int _indexTo);
    void RemoveEnum(int _index);
    void UpdateEnum(int _index, const Enumerator& _another);
    void UpdateEnumName(int _index, const QString& _name);

    void RegisterAttributeParam(AttributeParam* _param);
    void UnregisterAttributeParam(AttributeParam* _param);

    int GetStructuresCount() const;
    const StructureDB* GetStructureTable(int index) const;
    StructureDB* GetStructureTable(int index);
    void CreateStructureDB(const TemplateStructure& _structureTemplate);

    int GetStringTableCount() const;
    const SStringTable* GetStringTable(int _index) const;
    const SStringTable* GetStringTable(const QString& _tableName) const;
    SStringTable* GetStringTable(int _index);
    SStringTable* GetStringTable(const QString& _tableName);
    void AddStringTable(const QString& _newTableName, int _index = -1);
    void DuplicateStringTable(int _index, int _indexOriginal);
    void MoveStringTable(int _indexFrom, int _indexTo);
    void RemoveStringTable(int _index);
    void RemoveStringTable(const QString& _tableName);
    void RenameStringTable(int _index, const QString& _tableName);
    bool AreValidIdentifiers(const QString& _tableId, const QString& _stringId) const;
    QString GetStringForDisplay(const QString& _tableId, const QString& _stringId, bool _complete = false) const;

signals:
    void StringTableAdded(const int _index);
    void StringTableMoved(const int _indexFrom, const int _indexTo);
    void StringTableRemoved(const int _index);
    void StringTableRenamed(const int _index, const QString& _name);

    void StructureTableListChanged();
};

#endif // DB_MANAGER_H
