#ifndef AAMESH_H
#define AAMESH_H

#include "aasset.h"

class AAMesh : public AAsset
{
private:
    bool isSkeletal;

public:
    AAMesh(TemplateAttribute& _template, bool _isSkeletal = true);
    AAMesh(TemplateAttribute& _template, QString _filePath, bool _isSkeletal = true);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Mesh; };
    virtual const QString& GetAssetClassNameForCSV()            const override;
    virtual const QString& GetAssetFileExtension()              const override;
    static const QString& GetStaticAssetFileExtension();

    bool GetIsSkeletal() const { return isSkeletal; }
    void SetIsSkeletal(bool _isSkeletal) { isSkeletal = _isSkeletal; }
};

#endif // AAMESH_H
