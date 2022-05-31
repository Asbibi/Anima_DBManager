#ifndef AAANIMINSTANCE_H
#define AAANIMINSTANCE_H

#include "aasset.h"

class AAAnimInstance : public AAsset
{
public:
    AAAnimInstance();
    AAAnimInstance(QString _filePath);

    virtual Type GetType() const override { return Type::AnimInstance; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
};

#endif // AAANIMINSTANCE_H
