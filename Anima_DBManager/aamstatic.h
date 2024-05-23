#ifndef AAMSTATIC_H
#define AAMSTATIC_H

#include "aamesh.h"

class AAMStatic : public AAMesh
{
public:
    AAMStatic(TemplateAttribute& _template);
    AAMStatic(TemplateAttribute& _template, QString _filePath);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::StaticMesh; }
    virtual const QString& GetAssetClassNameForCSV()            const override;
};

#endif // AAMSTATIC_H
