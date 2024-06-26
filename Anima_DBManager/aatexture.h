#ifndef AATEXTURE_H
#define AATEXTURE_H

#include "aasset.h"
#include <QPixmap>

class AATexture : public AAsset
{
public:
    AATexture(TemplateAttribute& _template);
    AATexture(TemplateAttribute& _template, QString _filePath);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Texture; }
    virtual const QString& GetAssetClassNameForCSV()            const override;
    virtual const QString& GetAssetFileExtension()              const override;
    static const QString& GetStaticAssetFileExtension();
    static QPixmap GetPixmapFromAssetPath(const QString& _texturePath);
};

#endif // AATEXTURE_H
