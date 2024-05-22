#ifndef AACLASS_H
#define AACLASS_H

#include "aasset.h"

class AAClass : public AAsset
{
public:
    AAClass(TemplateAttribute& _template);
    AAClass(TemplateAttribute& _template, QString _filePath);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Class; }
    virtual const QString& GetAssetClassNameForCSV()            const override;
};

#endif // AACLASS_H
