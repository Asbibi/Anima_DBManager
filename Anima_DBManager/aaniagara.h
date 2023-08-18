#ifndef AANIAGARA_H
#define AANIAGARA_H

#include "aasset.h"

class AANiagara : public AAsset
{
public:
    AANiagara(const AttributeParam& _sharedParam);
    AANiagara(const AttributeParam& _sharedParam, QString _filePath);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Niagara; };
    virtual const QString& GetAssetClassNameForCSV()            const override;
};

#endif // AANIAGARA_H
