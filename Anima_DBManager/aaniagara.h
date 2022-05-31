#ifndef AANIAGARA_H
#define AANIAGARA_H

#include "aasset.h"

class AANiagara : public AAsset
{
public:
    AANiagara();
    AANiagara(QString _filePath);

    virtual Type GetType() const override { return Type::Niagara; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
};

#endif // AANIAGARA_H
