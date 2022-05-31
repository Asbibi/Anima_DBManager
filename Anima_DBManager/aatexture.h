#ifndef AATEXTURE_H
#define AATEXTURE_H

#include "aasset.h"

class AATexture : public AAsset
{
public:
    AATexture();
    AATexture(QString _filePath);

    virtual Type GetType() const override { return Type::Texture; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
};

#endif // AATEXTURE_H
