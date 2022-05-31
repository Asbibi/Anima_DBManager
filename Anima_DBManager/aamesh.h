#ifndef AAMESH_H
#define AAMESH_H

#include "aasset.h"

class AAMesh : public AAsset
{
private:
    bool isSkeletal;

public:
    AAMesh(bool _isSkeletal = true);
    AAMesh(QString _filePath, bool _isSkeletal = true);

    virtual Type GetType() const override { return Type::Mesh; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;

    bool GetIsSkeletal() const { return isSkeletal; }
    void SetIsSkeletal(bool _isSkeletal) { isSkeletal = _isSkeletal; }
};

#endif // AAMESH_H