#ifndef AANIAGARA_H
#define AANIAGARA_H

#include "aasset.h"

class AANiagara : public AAsset
{
public:
    AANiagara(TemplateAttribute& _template);
    AANiagara(TemplateAttribute& _template, QString _filePath);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Niagara; };
    virtual const QString& GetAssetClassNameForCSV()            const override;
};

#endif // AANIAGARA_H
