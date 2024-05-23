#ifndef AAMESH_H
#define AAMESH_H

#include "aasset.h"
#include "constants.h"

class AAMesh : public AAsset
{
public:
    AAMesh(TemplateAttribute& _template);
    AAMesh(TemplateAttribute& _template, QString _filePath);

#ifdef USE_SPECIFIC_FILE_EXTENSION_FOR_AAMESH
    virtual const QString& GetAssetFileExtension()              const override;
    static const QString& GetStaticAssetFileExtension();
#endif
    const QString& GetAssetFileExpression() const;
};

#endif // AAMESH_H
