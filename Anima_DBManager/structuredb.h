#ifndef STRUCTUREDB_H
#define STRUCTUREDB_H

#include "structure.h"
#include "templatestructure.h"
#include <vector>
#include <QObject>

class StructureDB : public QObject
{
private:
    const TemplateStructure myTemplate;
    std::vector<Structure*> myStructures;

    bool CheckIndex(int& index) const;

public:
    StructureDB(const TemplateStructure& _structureTemplate);
    ~StructureDB();

    void AddStructureAt(int index);
    void RemoveStructureAt(int index);
    void ClearStructures();
    const std::vector<Structure*>& GetStructures() const { return myStructures; }
    // Todo : return count + change vector to QList

    const TemplateStructure& GetTemplate() const;
    const Structure* GetStructureAt(int index) const;
    int GetStructureCount() const;
    const QString& GetTemplateName() const;
    const QString GetStructureRowName(int index) const;
    const QString GetStructureRowName(const Structure* _structure) const;

    void WriteValue_CSV_Table(std::ofstream& file) const;
};

#endif // STRUCTUREDB_H
