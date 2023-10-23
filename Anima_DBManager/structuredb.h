#ifndef STRUCTUREDB_H
#define STRUCTUREDB_H

#include <QObject>

#include "structure.h"
#include "templatestructure.h"
#include "areference.h"
#include <QList>
#include <QHash>
#include <QJsonArray>

class QTemplateStructure;

class StructureDB : public QObject
{
private:
    friend QTemplateStructure;

    TemplateStructure myTemplate;
    QList<Structure*> myStructures;

    bool CheckIndex(int& index) const;

public:
    StructureDB(const TemplateStructure& _structureTemplate);
    StructureDB(const StructureDB& _another);
    ~StructureDB();

    void SetTemplateName(const QString& _name);
    void SetTemplateAbbrev(QString& _abbrev);
    void SetTemplateIconType(IconManager::IconType _iconType);
    void SetTemplateColor(const QColor& _color);
    void SetTemplateAttributeName(int _index, QString& _name);

    void AddStructureAt(int _index);
    void DuplicateStructureAt(int _index, int _originalIndex);
    void RemoveStructureAt(int _index);
    void MoveStructureAt(int _indexFrom, int& _indexTo);
    void SetStructureCount(int _count);
    void ClearStructures();

    void MoveAttribute(int _indexFrom, int _indexTo);
    void ResetAttributeToDefault(int _attrIndex);
    bool ChangeAttributeTemplate(int _attrIndex, const TemplateAttribute& _templateToCopy);
    void FixAttributesTypeToDefault(int _attIndex);
    void AddAttribute(int _position, bool _copyFromPrevious);
    void RemoveAttribute(int _position);
    void SetAttributesFromList(const QList<QString>& _stringList, QHash<AReference*, QString>& _outRefMap);

    bool UpdateMyAAssetIsDirty();


    const QString& GetTemplateName() const;
    const QString& GetTemplateAbbrev() const;
    QIcon GetIcon() const;
    QString GetTemplateColorString() const;
    const TemplateStructure& GetTemplate() const;
    AttributeTypeHelper::Type GetAttributeTemplateType(int _attributeIndex) const;

    const QList<Structure*>& GetStructures() const { return myStructures; }
    const Structure* GetStructureAt(int index) const;
    int GetStructureCount() const;
    const QString GetStructureRowName(int index) const;
    const QString GetStructureRowName(const Structure* _structure) const;
    int GetStructureIndex(const Structure* _structure) const;

    QJsonArray WriteValue_JSON_Table() const;
    void WriteValue_CSV_Table(std::ofstream& file) const;
    void ReadValue_JSON_Table(const QJsonArray& _structArrayJson, int _overwritePolicy);
    void ReadValue_CSV_Table(int _index, const QStringList& fields, int _overwritePolicy);
    void AddValue_CSV_TableWithDelayedReference(const QStringList& fields, QHash<AReference*, QString>& referenceMap);
};

#endif // STRUCTUREDB_H
