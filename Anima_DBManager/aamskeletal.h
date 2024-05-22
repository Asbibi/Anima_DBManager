#ifndef AAMSKELETAL_H
#define AAMSKELETAL_H

#include "aamesh.h"

class AAMSkeletal : public AAMesh
{
public:
    AAMSkeletal(TemplateAttribute& _template);
    AAMSkeletal(TemplateAttribute& _template, QString _filePath);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::SkeletalMesh; }
    virtual const QString& GetAssetClassNameForCSV()            const override;
};

#endif // AAMSKELETAL_H
