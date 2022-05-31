#ifndef AASOUND_H
#define AASOUND_H

#include "aasset.h"

class AASound : public AAsset
{
public:
    AASound();
    AASound(QString _filePath);

    virtual Type GetType() const override { return Type::Sound; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
};

#endif // AASOUND_H
