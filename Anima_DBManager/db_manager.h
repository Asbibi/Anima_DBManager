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
    QList<StructureDB*> myStructures;
    QList<SStringTable> myStringTables;

    DB_Manager();
    DB_Manager(const DB_Manager&) = delete;
    ~DB_Manager();

    int GetStringTableIndex(const QString& _tableName) const;
    int GetStructureTableIndex(const QString& _structName) const;
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
    const StructureDB* GetStructureTable(const QString& _SDBName) const;
    StructureDB* GetStructureTable(const QString& _SDBName);
    void AddStructureDB(const TemplateStructure& _structureTemplate, int _index = -1);
    void DuplicateStructureDB(int _index, int _indexOriginal);
    void MoveStructureDB(int _indexFrom, int _indexTo);
    void RemoveStructureDB(int _index, bool forDelete = false);
    void RemoveStructureDB(const QString& _tableName, bool forDelete = false);
    void RenameStructureDB(int _index, const QString& _tableName);
    void MoveStructureAttribute(int _tableIndex, int _indexFrom, int _indexTo);
    void MoveStructureAttribute(const QString& _tableName, int _indexFrom, int _indexTo);
    void RenameStructureAttribute(int _tableIndex, int _attributeIndex, const QString& _attributeName);
    void RenameStructureAttribute(const QString& _tableName, int _attributeIndex, const QString& _attributeName);
    void ResetAttributesToDefaultValue(int _tableIndex, int _attributeIndex);
    void ResetAttributesToDefaultValue(const QString& _tableName, int _attributeIndex);
    void ChangeAttributeTemplate(int _tableIndex, int _attrIndex, AttributeTypeHelper::Type _newType, const AttributeParam& _param, bool _needResetValue);
    void ChangeAttributeTemplate(const QString& _tableName, int _attrIndex, AttributeTypeHelper::Type _newType, const AttributeParam& _param, bool _needResetValue);

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

    void AskFocusOnStringItem(const int _tableIndex, const int _stringIndex);
    void AskUpdateOnStringTable(const int _tableIndex);
    void AskUpdateOnStringPanel(const int _tableIndex);

signals:
    void StringTableAdded(const int _index);
    void StringTableMoved(const int _indexFrom, const int _indexTo);
    void StringTableRemoved(const int _index);
    void StringTableRenamed(const int _index, const QString& _name);
    void StringTableChanged(const int _tableIndex);     // For edits done in table widget
    void StringItemFocus(const int _tableIndex, const int _index);
    void StringItemChanged(const int _tableIndex);      // For edits done in panel widget

    void StructTableAdded(const int _index);
    void StructTableMoved(const int _indexFrom, const int _indexTo);
    void StructTableRemoved(const int _index);
    void StructTableRenamed(const int _index, const QString& _name);
    void StructTableChanged(const int _tableIndex);     // For edits done in table widget
    void StructItemFocus(const int _tableIndex, const int _index);
    void StructItemChanged(const int _tableIndex);      // For edits done in panel widget
    void StructAttributeNameChanged(const int _tableIndex);
};

#endif // DB_MANAGER_H
