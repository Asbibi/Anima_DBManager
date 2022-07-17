#ifndef AAANIMINSTANCE_H
#define AAANIMINSTANCE_H

#include "aasset.h"

class AAAnimInstance : public AAsset
{
public:
    AAAnimInstance(const AttributeParam& _sharedParam);
    AAAnimInstance(const AttributeParam& _sharedParam, QString _filePath);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::AnimInstance; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
};

#endif // AAANIMINSTANCE_H
