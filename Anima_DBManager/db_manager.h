#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "attributeparam.h"
#include "enumerator.h"
#include "structuredb.h"
#include "templatestructure.h"
#include "sstringtable.h"

#include <QString>
#include <QStandardPaths>
#include <QList>
#include <QTimer>
#include <vector>

class DB_Manager : public QObject
{
    Q_OBJECT

private:
    QString myProjectContentFolderPath;
    const QString myHomePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    bool myProjectPathIsValid = false;

    QString myAttributePrefix = "";
    QString myAttributeSuffix = "";

    bool myAutoSaveEnabled = false;
    int myAutoSaveInterval = 15;        // unit : minut
    QTimer* myAutoSaveTimer;

    SStringTable myStringTableDictionnary = SStringTable("DICTIONNARY");
    QList<AttributeParam*> myAttributeParamPtrs;
    QList<Enumerator> enumerators;
    QList<StructureDB*> myStructures;
    QList<SStringTable> myStringTables;

    QMap<AttributeTypeHelper::Type, QString> myAAssetRegex;

    DB_Manager();
    DB_Manager(const DB_Manager&) = delete;
    ~DB_Manager();

    int GetStringTableIndex(const QString& _tableName) const;
    void AddStringTablePrivate(const QString& _newTableName, int& _index);


public:
    static DB_Manager& GetDB_Manager();

    void Init();
    void Reset();

    bool SetProjectContentFolderPath(const QString& _path);
    const QString& GetProjectContentFolderPath(bool _homePathIfUnvalid = true) const;
    QString GetProjectSourceFolderPath(bool _homePathIfUnvalid = true) const;
    bool IsProjectContentFolderPathValid() const;

    const QString GetAAssetRegex(AttributeTypeHelper::Type _type) const;
    void SetAAssetRegex(AttributeTypeHelper::Type _type, const QString& _regex);

    bool SetAttributeFixsIfOk(const QString& _prefix, const QString& _suffix);
    void SetAttributePrefix(const QString& _prefix);
    void SetAttributeSuffix(const QString& _suffix);
    const QString& GetAttributePrefix() const;
    const QString& GetAttributeSuffix() const;
    QString GetAttributeFullName(const QString& _attributeName) const;

    void SetAutoSave(bool _enabled, int _intervalMinut);
    bool GetAutoSaveEnabled() const;
    int GetAutoSaveInterval() const;

    int GetEnumCount() const;
    const Enumerator* GetEnum(int _index) const;
    int GetIndexOfFirstEnumWithName(const QString& _name) const;
    int AddEnum(const Enumerator& _enum, int _index = -1);
    void MoveEnum(const int _indexFrom, const int _indexTo);
    bool CanSafelyRemoveEnum(int _index) const;
    void RemoveEnum(int _index);
    void UpdateEnum(int _index, const Enumerator& _another);
    void UpdateEnumName(int _index, const QString& _name);
    void AddValuesToEnum(int _index, const QString& _values);

    void RegisterAttributeParam(AttributeParam* _param);
    void UnregisterAttributeParam(AttributeParam* _param);

    int GetStructuresCount() const;
    const StructureDB* GetStructureTable(int index) const;
    StructureDB* GetStructureTable(int index);
    const StructureDB* GetStructureTable(const QString& _SDBName) const;
    StructureDB* GetStructureTable(const QString& _SDBName);
    int GetStructureTableIndex(const QString& _structName) const;
    void AddStructureDB(const TemplateStructure& _structureTemplate, int _index = -1);
    void DuplicateStructureDB(int _index, int _indexOriginal);
    void MoveStructureDB(int _indexFrom, int _indexTo);
    void RemoveStructureDB(int _index);
    void RemoveStructureDB(const QString& _tableName);
    void RenameStructureDB(int _index, const QString& _tableName);
    void ChangeStructureDBAbbrev(int _index, QString& _abbrev);
    void ChangeStructureDBIconType(int _index, IconManager::IconType _iconType);
    void ChangeStructureDBIconColor(int _index, const QColor& _abbrev);
    void MoveStructureAttribute(int _tableIndex, int _indexFrom, int _indexTo);
    void MoveStructureAttribute(const QString& _tableName, int _indexFrom, int _indexTo);
    void RenameStructureAttribute(int _tableIndex, int _attributeIndex, QString& _attributeName);
    void RenameStructureAttribute(const QString& _tableName, int _attributeIndex, QString& _attributeName);
    void ResetAttributesToDefaultValue(int _tableIndex, int _attributeIndex);
    void ResetAttributesToDefaultValue(const QString& _tableName, int _attributeIndex);
    void ChangeAttributeTemplate(int _tableIndex, int _attrIndex, const TemplateAttribute& _templateToCopy, bool _needResetValue);
    void ChangeAttributeTemplate(const QString& _tableName, int _attrIndex, const TemplateAttribute& _templateToCopy, bool _needResetValue);
    void AddAttributeTemplate(int _tableIndex, int _attrIndex, bool _copyFromPrevious);
    void AddAttributeTemplate(const QString& _tableName, int _attrIndex, bool _copyFromPrevious);
    void RemoveAttributeTemplate(int _tableIndex, int _attrIndex);
    void RemoveAttributeTemplate(const QString& _tableName, int _attrIndex);
    void SetAttributeTemplatesFromJSON(int _tableIndex, const QJsonArray& _attributesAsJson);
    void AddStructureRow(const int _tableIndex, const int _position);
    void DuplicateStructureRow(const int _tableIndex, const int _position, const int _originalIndex);
    void RemoveStructureRow(const int _tableIndex, const int _position);
    void MoveStructureRow(const int _tableIndex, const int _positionFrom, int& _positionTo);
    void SetStructureRowCount(const int _tableIndex, const int _count);
    void UpdateAAssetIsDirty();

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
    void RenameStringTable(int _index, QString& _tableName);
    void SetStringTableItemCount(const int _index, const int _count);
    bool AreValidIdentifiers(const QString& _tableId, const QString& _stringId) const;
    QString GetStringForDisplay(const QString& _tableId, const QString& _stringId, bool _complete = false) const;

    const SStringTable* GetDictionnary() const;
    SStringTable* GetDictionnary();
    void ReplaceDictionnaryWithLastStringTable();

    void AskFocusOnStringItem(const int _tableIndex, const int _stringIndex, const int _languageIndex = -1, const bool _forceFocus = false);
    void AskUpdateOnStringTable(const int _tableIndex);
    void AskUpdateOnStringPanel(const int _tableIndex);
    void AskFocusOnStructItem(const int _tableIndex, const int _structIndex, const int _attrIndex = 0, const bool _forceFocus = false);
    void AskFocusOnStructPanel(const int _tableIndex, const int _itemIndex);
    void AskFocusOnStructPanel(const QString& _tableName, const int _itemIndex);

signals:
    void StringTableAdded(const int _index);
    void StringTableMoved(const int _indexFrom, const int _indexTo);
    void StringTableRemoved(const int _index);
    void StringTableRenamed(const int _index, const QString& _name);
    void StringTableChanged(const int _tableIndex);     // For edits done in table widget
    void StringItemFocus(const int _tableIndex, const int _index, const int _languageIndex, const bool _forceFocus);
    void StringItemChanged(const int _tableIndex);      // For edits done in panel widget

    void StructTableAdded(const int _index);
    void StructTableMoved(const int _indexFrom, const int _indexTo);
    void StructTableRemoved(const int _index);
    void StructTableRenamed(const int _index, const QString& _name);
    void StructTableIconChanged(const int _index, const QIcon& _icon);
    void StructTableFocus(const int _tableIndex, const int _itemIndex);   // For edits or selection done in table widget
    void StructItemFocus(const int _tableIndex, const int _index, const int _attrIndex, const bool _forceFocus);
    void StructItemChanged(const int _tableIndex);      // For edits done in panel widget
    void StructAttributeNameChanged(const int _tableIndex);

    void ResetView();
    void DictionnaryChanged();

    void AcknowledgeChange();
    void AutoSaveFeedback(bool _showFeedback);

public slots:
    void AutoSave();
};

#endif // DB_MANAGER_H
